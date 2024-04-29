#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "submodule.h"

LOG_MODULE_REGISTER(callback);

void my_callback(struct MODULE_DATA *data)
{
	LOG_INF("Variable 1: %d, Variable 2: %d, Variable 3: %d", data->variable_1, data->variable_2, data->variable_3);
}

int main(void)
{
	LOG_INF("Zephyr Callback Example");
	LOG_INF("Board: %s", CONFIG_BOARD);

	submodule_init();
	submodule_register_callback(my_callback);
}
