/*
 * Example MLX90641 Code
 * Using Library from Melexis
 * https://github.com/melexis/mlx90641-library
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "MLX90641_API.h"
#include "MLX90641_I2C_Driver.h"
#include "gpio.h"

LOG_MODULE_REGISTER(main);

#define TA_SHIFT 5  // For a MLX90641 in the open air the shift is -5°C (Page 15, Rev 1.4 Driver Manual)

int main(void)
{
	int ret;
	uint8_t addr = 0x33;
	static uint16_t mlx90641_eeData[832];
	static uint16_t mlx90641_frameData[242];
	paramsMLX90641 mlx90641_param;	// pointer to the MCU memory location where the
									// already extracted parameters for the MLX90641 device are stored

	static float mlx90641To[192];
	float tr = 0;
	float emissivity = 0.95;

	float TempAmb;
	float TempObj;

	LOG_INF("Zephyr MLX90641 (16x12) Thermal Sensor Array Example");
	LOG_INF("Board: %s", CONFIG_BOARD);

	gpio_init();
	k_sleep(K_MSEC(100));
	MLX90641_I2CInit();

	// Download and extract parameters from EEPROM:
	ret = MLX90641_DumpEE(addr, mlx90641_eeData);
	if (ret != 0) {
		LOG_ERR("Error %d dumping MLX90641 EEPROM", ret);
		return(-1);
	}

#ifdef DUMP_EEPROM
	// Dump EEPROM
	for (int i = 0; i <= 64; i++) {
		LOG_INF("0x%04X = 0x%04X", i + 0x2400, mlx90641_eeData[i]);
		k_sleep(K_MSEC(10));
	}
#endif

	ret = MLX90641_ExtractParameters(mlx90641_eeData, &mlx90641_param);
	if (ret != 0) {
		LOG_ERR("Error %d extracting parameters from MLX90641", ret);
		return(-1);
	}

	// Trigger Measurement:
	ret = MLX90641_TriggerMeasurement(addr);
	if (ret != 0) {
		LOG_ERR("Error %d triggering measurement", ret);
	} else {
		k_msleep(1000);
	}

	// Get Frame Data:
	ret = MLX90641_GetFrameData(addr, mlx90641_frameData);
	if (ret != 0) {
		LOG_ERR("Error getting frame data from MLX90641");
		return(-1);
	}

	// Get Sensor Power Supply Voltage:
	LOG_INF("VDD = %.02fV", MLX90641_GetVdd(mlx90641_frameData, &mlx90641_param));

	// Get Ambient Temperature of Sensor:
	TempAmb = MLX90641_GetTa(mlx90641_frameData, &mlx90641_param);

	// Reflected Temperature:
	// The sensor ambient temperature could be used, but some shift depending on the
	// enclosure might be needed. For a MLX90641 in the open air the shift is -5°C (TA_SHIFT)
	tr = TempAmb - TA_SHIFT; //reflected temperature based on the sensor ambient temperature

	// Calculate Array Temperature of Object:
	MLX90641_CalculateTo(mlx90641_frameData, &mlx90641_param, emissivity, tr, mlx90641To);

	// Display Temperatures
	for(int row = 0; row < 12; row++) {
		int i = row * 16;
		LOG_INF("TO%02d, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f, %2.2f",
			row, 
			mlx90641To[i + 0], mlx90641To[i + 1], mlx90641To[i + 2], mlx90641To[i + 3],
			mlx90641To[i + 4], mlx90641To[i + 5], mlx90641To[i + 6], mlx90641To[i + 7],
			mlx90641To[i + 8], mlx90641To[i + 9], mlx90641To[i + 10], mlx90641To[i + 11],
			mlx90641To[i + 12], mlx90641To[i + 13], mlx90641To[i + 14], mlx90641To[i + 15]);
		};

	// Average the Array Values
	float average = 0;
	for (int i = 0; i < 192; i++) {
		average = average + mlx90641To[i];
	}
	TempObj =  average / 192;

	LOG_INF("Averaged Object Temp %.02lf degC, Ambient Temp %.02lf degC", TempObj, TempAmb);

	while (1) {
		k_sleep(K_SECONDS(10));
	}
}

