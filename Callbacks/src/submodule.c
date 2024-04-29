#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "submodule.h"

LOG_MODULE_REGISTER(submodule);

MODULE_CALLBACK module_callback;

struct k_timer timer1;

void timer1_callback(struct k_timer *timer_id)
{
	static struct MODULE_DATA data;

	data.variable_1++;
	data.variable_2 = 4;
	data.variable_3 = 3;

	if (module_callback != NULL) {
		module_callback(&data);
	}
}

void submodule_init(void)
{
	// Initialise our module
	k_timer_init(&timer1, timer1_callback, NULL);
	k_timer_start(&timer1, K_MSEC(1000), K_MSEC(1000));
}

void submodule_register_callback(MODULE_CALLBACK callback_function_address)
{
	module_callback = (void *)callback_function_address;
}

void submodule_deregister_callback(void)
{
	module_callback = NULL;
}
