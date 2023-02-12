
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/random/rand32.h>

void main(void)
{
	uint8_t app_key[16];

	printk("Zephyr Random Number Generator Example \nBoard: %s\n", CONFIG_BOARD);

	while(1) {
		
		// Return a 32-bit random value that should pass general randomness tests: 
		//uint32_t random = sys_rand32_get();
		//printk("%d\n",random);
		
		// Fill the destination buffer with random data values that should pass general randomness tests:
		//sys_rand_get(app_key, 16);

		// Fill the destination buffer with cryptographically secure random data values:
		sys_csrand_get(app_key, 16);

		for (int i = 0; i < 16; i++)
			printk("%02X ",app_key[i]);
		printk("\n");

		k_sleep(K_MSEC(500));
	}
}
