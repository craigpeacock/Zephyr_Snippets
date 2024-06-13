
#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/modbus/modbus.h>

LOG_MODULE_REGISTER(main);

static int iface;

struct modbus_iface_param client_param = {
	.mode = MODBUS_MODE_RTU,
	.rx_timeout = 50000,
	.serial = {
		.baud = 4800,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits_client = UART_CFG_STOP_BITS_1,
	},
};

int modbus_init(uint32_t baud)
{
	int ret;
	
	const char iface_name[] = {DEVICE_DT_NAME(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_modbus_serial))};

	client_param.serial.baud = baud;

	LOG_INF("Initialising Modbus to %d baud", client_param.serial.baud);

	iface = modbus_iface_get_by_name(iface_name);

	ret = modbus_init_client(iface, client_param);
	if (ret != 0) {
		LOG_ERR("Modbus RTU client initialization failed");
	}
}

int modbus_read_sensor(uint8_t dev_addr, uint16_t dev_register)
{
	uint16_t reg[1] = { 0 };

	int err;

	err = modbus_read_holding_regs(iface, dev_addr, dev_register, reg, ARRAY_SIZE(reg));
	if (err != 0) {
		LOG_ERR("FC01 failed with %d", err);
		return 0;
	}
	
	return reg[0];
}

int main(void)
{
	LOG_INF("Zephyr Modbus Example\nBoard: %s", CONFIG_BOARD);

	modbus_init(4800);

	while (1) {
		LOG_INF("Data = %d", modbus_read_sensor(1, 1));
		k_msleep(1000);
	}
}
