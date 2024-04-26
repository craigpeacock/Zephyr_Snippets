/*
 * Zephyr Fatal Error Handling Example
 * 
 * Copyright 2024 (c) Craig Peacock
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/fatal.h>
#include <zephyr/arch/cpu.h>

LOG_MODULE_REGISTER(fatalexample);

void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf)
{
	LOG_ERR("Fatal Error. Rebooting...\n\r");
	LOG_PANIC();
	sys_arch_reboot(0);
}

int main(void)
{
	printk("Zephyr Fatal Error Handling Example \nBoard: %s\n", CONFIG_BOARD);

	while (1) {
		printk("Hello World\n");
		k_msleep(1000);
		//uint32_t foo = 0x55AA;
		//__ASSERT(foo == 0x55A0, "Invalid value of foo, got 0x%x", foo);
		k_panic();
	}
}
