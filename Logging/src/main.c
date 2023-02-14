
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// If log level not specifed, use default log level:
LOG_MODULE_REGISTER(logging);
// LOG_MODULE_REGISTER(logging, LOG_LEVEL_DBG);

void main(void)
{
	printk("Zephyr Logging Example\nBoard: %s\n", CONFIG_BOARD);

	while (1) {

		LOG_ERR("This is an error");
		LOG_INF("This is informational");
		LOG_WRN("This is a warning");
		LOG_DBG("This is for debugging");
		k_msleep(1000);
	}
}
