#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "MLX90641_API.h"
#include "MLX90641_I2C_Driver.h"
#include "MLX90641_Helper.h"

LOG_MODULE_REGISTER(help);

void MLX90641_display_resolution(uint8_t addr)
{
	int ret = MLX90641_GetCurResolution(addr);
	switch(ret) {
		case 0x00:
			LOG_INF("Resoluton: 16-bit");
			break;

		case 0x01:
			LOG_INF("Resoluton: 17-bit");
			break;

		case 0x02:
			LOG_INF("Resoluton: 18-bit (default)");
			break;

		case 0x03:
			LOG_INF("Resoluton: 19-bit");
			break;

		default:
			LOG_ERR("Resolution: Unknown");
			break;
	}
}

void MLX90641_display_refresh_rate(uint8_t addr)
{
	int ret = MLX90641_GetRefreshRate(addr);
	switch(ret) {
		case 0x00:
			LOG_INF("Refresh Rate: 0.5Hz");
			break;

		case 0x01:
			LOG_INF("Refresh Rate: 1Hz");
			break;

		case 0x02:
			LOG_INF("Refresh Rate: 2Hz (default)");
			break;

		case 0x03:
			LOG_INF("Refresh Rate: 4Hz");
			break;

		case 0x04:
			LOG_INF("Refresh Rate: 8Hz");
			break;

		case 0x05:
			LOG_INF("Refresh Rate: 16Hz");
			break;

		case 0x06:
			LOG_INF("Refresh Rate: 32Hz");
			break;

		case 0x07:
			LOG_INF("Refresh Rate: 64Hz");
			break;

		default:
			LOG_ERR("Refresh Rate: Unknown");
			break;
	}
}