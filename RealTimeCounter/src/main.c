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
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>

#define ALARM_CHANNEL_ID 0

static void alarm_callback(const struct device *counter_dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	printk("Alarm\n");
}

void set_alarm(const struct device *dev)
{
	struct counter_alarm_cfg alarm_cfg;

	alarm_cfg.flags = 0;
	alarm_cfg.ticks = 10;
	alarm_cfg.callback = alarm_callback;
	alarm_cfg.user_data = &alarm_cfg;

	if (counter_set_channel_alarm(dev, ALARM_CHANNEL_ID, &alarm_cfg) == 0) 
	{
		printk("Successfully set alarm\n");
	}
}


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
	printk("Wakeup Capable: %s\n", pm_device_wakeup_is_capable(counter_dev) ? "Yes" : "No");
	printk("Wakeup Source: %s\n", pm_device_wakeup_is_enabled(counter_dev) ? "Enabled" : "Disabled");

	counter_start(counter_dev);
	
	set_alarm(counter_dev);

	while (1) {
		if (counter_get_value(counter_dev, &ticks) == 0) {
			printk("Counter value %d\n", ticks); 
		}
		k_sleep(K_SECONDS(1));
	}

	counter_stop(counter_dev);
}
