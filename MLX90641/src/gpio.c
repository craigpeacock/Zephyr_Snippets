/*
 * Zephyr GPIO Example
 * Demonstrates basic general purpose input-output (GPIO) support.
 * Device Tree settings can be found in app.overlay file.
 * 
 * Zephyr GPIO API documentation can be found at 
 * https://docs.zephyrproject.org/latest/hardware/peripherals/gpio.html
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

 #include <stdio.h>
 #include <string.h>
 #include <zephyr/device.h>
 #include <zephyr/kernel.h>
 #include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec gpio_pwr_enable = GPIO_DT_SPEC_GET(DT_ALIAS(pwr_en), gpios);

void gpio_init(void)
{
	int ret;

	ret = gpio_pin_configure_dt(&gpio_pwr_enable, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	gpio_pin_set_dt(&gpio_pwr_enable, 1);
}