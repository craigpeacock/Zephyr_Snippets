/*
 *  Copyright (c) 2023 Craig Peacock
 *  Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#define BUF_SIZE 64

static K_MEM_SLAB_DEFINE(uart_slab, BUF_SIZE, 3, 4);

K_SEM_DEFINE(uart_rx_rdy_sem, 0, 1);
uint8_t *read_ptr;
size_t read_len;

void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{

	switch (evt->type) {
	
		case UART_TX_DONE:
			printk("TX Done %d bytes\n", evt->data.tx.len);
			break;
	
		case UART_TX_ABORTED:
			break;
	
		case UART_RX_RDY:
			read_ptr = evt->data.rx.buf + evt->data.rx.offset;
			read_len = evt->data.rx.len;
			k_sem_give(&uart_rx_rdy_sem);
			break;

		case UART_RX_BUF_REQUEST:
		{
			int ret;
			uint8_t *buf;

			ret = k_mem_slab_alloc(&uart_slab, (void **)&buf, K_NO_WAIT);
			if (ret != 0) {
				printk("Failed to allocate slab\n");
			}

			ret = uart_rx_buf_rsp(dev, buf, BUF_SIZE);
			if (ret != 0) {
				printk("Failed to provide new buffer\n");
			}
			break;
		}
	
		case UART_RX_BUF_RELEASED:
			k_mem_slab_free(&uart_slab, (void **)&evt->data.rx_buf.buf);
			break;
	
		case UART_RX_DISABLED:
			printk("RX Disabled\n");
			break;
	
		default:
			break;
	}
}

void main(void)
{
	const struct device *uart_dev;
	char buffer[100];
	uint8_t *buf;
	int ret;

	printk("UART Example\nBoard: %s\n", CONFIG_BOARD);

	uart_dev = DEVICE_DT_GET(DT_ALIAS(uartlog));
	if (!uart_dev) {
		printk("uartlog DT alias not found.\n");
		return;
	}

	if (!device_is_ready(uart_dev)) {
		printk("UART device not ready.\n");
		return;
	}
 
	ret = k_mem_slab_alloc(&uart_slab, (void **)&buf, K_NO_WAIT);
	if (ret != 0) {
		printk("k_mem_slab_alloc() error (%d)\n", ret);
		return;
	}

	ret = uart_callback_set(uart_dev, uart_callback, NULL);
	if (ret != 0) {
		printk("uart_callback_set() error (%d)\n", ret);
		if (ret == -ENOTSUP) printk("ASYNC API not enabled\n");
		return;
	}

	ret = uart_rx_enable(uart_dev, buf, BUF_SIZE, 100000);
	if ((ret != 0) & (ret != -EBUSY)) {
		printk("uart_rx_enable() error (%d)\n", ret);
	}

	sprintf(buffer, "Hello World\r\n");
	ret = uart_tx(uart_dev, buffer, strlen(buffer), 0);
	if (ret != 0 ) {
		printk("uart_tx() error (%d)\n", ret);
	}

	do {

		if (k_sem_take(&uart_rx_rdy_sem, K_NO_WAIT) != 0) {
			//printk("k_sem_take error\n");
		} else {
			//printk("RX %d bytes: ", read_len);
			
			for (int i = 0; i < read_len; i++) {
				if (*(read_ptr + i) == 0x0D) {
					printk("\n");
				} else {
					printk("%c", *(read_ptr + i) );
				}
			}
		}

	} while(1);
}
