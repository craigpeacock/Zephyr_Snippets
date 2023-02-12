/*
 * Copyright (c) 2023 Craig Peacock
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/console/console.h>

void printk_key(uint8_t key[], uint8_t len)
{
	for (int i = 0; i < len; i++)
		printk("%02X ", key[i]);
	printk("\n");
}

uint8_t parse_str(uint8_t *string, uint8_t **fields, uint8_t max_fields)
{
	uint8_t i = 0;

	if (*string == 0) 
		return 0;
	
	fields[i++] = string;

	while((*string != 0) && (i <= max_fields)) {
		if (*string == ' ') {
			*string = '\0';		
			fields[i++] = string + 1;
		}
		string++;
	}

	return(i);
}

void main(void)
{
	uint8_t *buf;
	uint8_t app_key[16] = { 0 };
	int ret;

	console_getline_init();

	printk("Zephyr Console scanf Example\nBoard: %s\n", CONFIG_BOARD);

	do {

		printk("Enter App Key: ");
		buf = console_getline();
		printk("Raw String Entered: %s\n", buf);

#ifdef SCANF
		// sscanf expects unsigned int * for %X
		uint8_t k[16];
		ret = sscanf(buf, "%02X %02X %02X %02X %02X %02X %02X %02X", &k[0], &k[1], &k[2], &k[3], &k[4], &k[5], &k[6], &k[7]);
		printk("sscanf ret = %d\n", ret);

		if (ret) {
		 	for (int i = 0; i < ret; i++)
		 		printk("%02X ", k[i]);
		}
		printk("\n");
#endif

		uint8_t maxdigit = 16;
		uint8_t *ptrhex[maxdigit];

		ret = parse_str(buf, ptrhex, 16);
		printk("Parsed %d values\n", ret);

		for (int i = 0; i < ret; i++) {
			app_key[i] = (strtol(ptrhex[i], NULL, 16) & 0xFF);
		}		

		printk("New Key: ");
		printk_key(app_key, 16);

		printk("\n\n");

	} while(1);

}
