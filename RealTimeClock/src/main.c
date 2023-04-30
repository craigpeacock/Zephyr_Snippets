/*
 * Real Time Clock (RTC) Example
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
#include <zephyr/drivers/rtc.h>

static void alarm_callback(const struct device *dev, uint16_t id, void *user_data)
{
	printk("Alarm\n");
}

void main(void)
{
	const struct device *const rtc_dev = DEVICE_DT_GET(DT_ALIAS(rtc));

	int err = -1;
	uint16_t mask = 0;
	struct rtc_time rtctime;

	printk("Zephyr Real Time Clock Example\nBoard: %s\n", CONFIG_BOARD);

	if (!device_is_ready(rtc_dev)) {
			printk("device not ready.\n");
			return;
	}

	err = rtc_alarm_set_callback(rtc_dev, 0, alarm_callback, NULL);
	if (err != 0) {
		if (err == -ENOTSUP) printk("rtc_alarm_set_callback not supported by hardware\n");
		else printk("Error in rtc_alarm_set_callback(), %d\n",err);
	}

	err = rtc_alarm_get_supported_fields(rtc_dev, 0, &mask);
	if (err == 0) {
		printk("Mask = 0x%02X\nSupported fields:\n", mask);
		if (mask & RTC_ALARM_TIME_MASK_SECOND) printk("Seconds\n");
		if (mask & RTC_ALARM_TIME_MASK_MINUTE) printk("Minutes\n");
		if (mask & RTC_ALARM_TIME_MASK_HOUR)   printk("Hours\n");
		if (mask & RTC_ALARM_TIME_MASK_MONTHDAY) printk("Monthday\n");
		if (mask & RTC_ALARM_TIME_MASK_MONTH) printk("Month\n");
		if (mask & RTC_ALARM_TIME_MASK_YEAR) printk("Year\n");
		if (mask & RTC_ALARM_TIME_MASK_WEEKDAY) printk("Weekday\n");
		if (mask & RTC_ALARM_TIME_MASK_YEARDAY)printk("Yearday\n");
		if (mask & RTC_ALARM_TIME_MASK_NSEC) printk("NanoSeconds\n");
	} else {
		printk("Error in rtc_alarm_get_supported_fields(), %d\n",err);
	}
	printk("\n");

	rtctime.tm_year = 2023;
	rtctime.tm_mon = 4;
	rtctime.tm_mday = 30;

	rtctime.tm_hour = 12;
	rtctime.tm_min = 54;
	rtctime.tm_sec = 23;

	mask = RTC_ALARM_TIME_MASK_SECOND | RTC_ALARM_TIME_MASK_MINUTE | RTC_ALARM_TIME_MASK_HOUR | RTC_ALARM_TIME_MASK_YEARDAY | RTC_ALARM_TIME_MASK_MONTH | RTC_ALARM_TIME_MASK_MONTHDAY;

	err = rtc_alarm_set_time(rtc_dev, 0, mask, &rtctime);
	if (err != 0) {
		if (err == -ENOSYS) printk("rtc_alarm_set_time() function not implemented\n");
		else printk("Error in rtc_alarm_set_time(), %d\n",err);
	}

	err = rtc_set_time(rtc_dev, &rtctime);
	if (err != 0) {
		printk("Error in rtc_set_time(), %d\n",err);
	}

	while (1) {
		if (rtc_get_time(rtc_dev, &rtctime) == 0) {
			printk("%02d:%02d:%02d\n", rtctime.tm_hour, rtctime.tm_min, rtctime.tm_sec);
		}
		k_sleep(K_SECONDS(10));
	}
}
