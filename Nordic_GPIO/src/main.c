
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <hal/nrf_gpio.h>

#define GPIO_PIN 21

void main(void)
{
	nrf_gpio_cfg(NRF_GPIO_PIN_MAP(0, GPIO_PIN), NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, GPIO_PIN));

	while (1) {
		printk("Hello World\n");
		nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, GPIO_PIN));
		k_msleep(500);
		nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, GPIO_PIN));
		k_msleep(500);
	}
}
