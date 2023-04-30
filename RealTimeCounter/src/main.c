/*
 * Real Time Counter (RTC) Example
 *
 * Zephyr has support for Real-time clock and Real-time counter. The clock provides a broken-down 
 * time format, while the counter provides a low power counter that can be used to track time
 * https://docs.zephyrproject.org/latest/hardware/peripherals/counter.html
 * https://docs.zephyrproject.org/latest/hardware/peripherals/rtc.html
 *
 * Copyright (c) 2023 Craig Peacock
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h>

#define ALARM_CHANNEL_ID 0

void main(void)
{
	const struct device *const counter_dev = DEVICE_DT_GET(DT_INST(0, st_stm32_rtc));
	uint32_t ticks;

	printk("Zephyr Real Time Counter Example\nBoard: %s\n", CONFIG_BOARD);

	if (!device_is_ready(counter_dev)) {
			printk("Counter device not ready.\n");
			return;
	}

	printk("Number of counters: %d\n", counter_get_num_of_channels(counter_dev));
	printk("Counter frequency: %dHz\n", counter_get_frequency(counter_dev));

	counter_start(counter_dev);

	while (1) {
		if (counter_get_value(counter_dev, &ticks) == 0) {
			printk("Counter value %d\n", ticks); 
		}
		k_sleep(K_SECONDS(1));
	}

	counter_stop(counter_dev);
}
