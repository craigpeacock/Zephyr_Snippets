/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "MLX90641_I2C_Driver.h"

LOG_MODULE_REGISTER(mlx90641);

const struct device *dev;

void MLX90641_I2CInit()
{
	dev = DEVICE_DT_GET(DT_ALIAS(i2cbus1));
	if (!dev) {
		LOG_ERR("I2C1: Device driver not found");
	} else {
		LOG_INF("I2C1 bus found, configuring speed");
		i2c_configure(dev, I2C_SPEED_SET(I2C_SPEED_STANDARD));
	}
}

int MLX90641_I2CGeneralReset(void)
{
	int ack;
	char cmd[2] = {0x00,0x06};

	ack = i2c_write(dev, &cmd[1], 1, 0x00);

	if (ack != 0x00)
	{
		return -1;
	}
	
	k_sleep(K_USEC(50));
	
	return 0;
}

int MLX90641_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
	uint8_t sa;
	int ack = 0;
	int cnt = 0;
	int i = 0;
	char cmd[2] = {0,0};
	char i2cData[1664] = {0};
	uint16_t *p;
	
	p = data;
	sa = (slaveAddr << 1);
	cmd[0] = startAddress >> 8;
	cmd[1] = startAddress & 0x00FF;
	
	ack = i2c_write_read(dev, slaveAddr, cmd, 2, i2cData, 2*nMemAddressRead);
	if (ack != 0x00)
	{
		return -1;
	}

	for(cnt = 0; cnt < nMemAddressRead; cnt++)
	{
		i = cnt << 1;
		*p++ = (uint16_t)i2cData[i]*256 + (uint16_t)i2cData[i+1];
	}
	
	return 0;
} 

void MLX90641_I2CFreqSet(int freq)
{
	//i2c.frequency(1000*freq);
}

int MLX90641_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
	uint8_t sa;
	int ack = 0;
	char cmd[4] = {0,0,0,0};
	static uint16_t dataCheck;

	sa = (slaveAddr << 1);
	cmd[0] = writeAddress >> 8;
	cmd[1] = writeAddress & 0x00FF;
	cmd[2] = data >> 8;
	cmd[3] = data & 0x00FF;

	ack = i2c_write(dev, cmd, 4, slaveAddr);

	if (ack != 0x00)
	{
		return -1;
	}

	MLX90641_I2CRead(slaveAddr, writeAddress, 1, &dataCheck);
	
	if ( dataCheck != data)
	{
		return -2;
	}
	
	return 0;
}