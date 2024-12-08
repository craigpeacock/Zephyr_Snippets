/*
 * MAX31865 PT100 Temperature Sensor Example
 *
 * https://docs.zephyrproject.org/latest/build/dts/api/bindings/sensor/maxim%2Cmax31865.html
 * https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31865.pdf
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

int main(void)
{
 	// Get a device structure from a devicetree node with compatible
 	// "maxim,max31865".
	const struct device *dev = DEVICE_DT_GET_ANY(maxim_max31865);

	if (dev == NULL) {
		printk("\nError: No devicetree node found for Maxim MAX31865\n");
		return 0;
	}

	if (!device_is_ready(dev)) {
		printf("Device %s is not ready\n", dev->name);
		return 0;
	}

	printk("Found device %s. Reading sensor data:\n", dev->name);

	while (true) {
		struct sensor_value temp;

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);

		printk("%.2f degC\n", sensor_value_to_double(&temp));

		k_sleep(K_MSEC(1000));
	}
}