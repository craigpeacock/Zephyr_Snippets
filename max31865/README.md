# MAX31865 PT100/PT1000 Temperature Sensor Example

Simple example Zephyr code to obtain read the temperature from a [Maxim MAX31865](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31865.pdf) using Zephyr's built in sensor subsystem.

The MAX31865 is set up in the device tree:

```
	max31865: max31865@0 {
		compatible = "maxim,max31865";
		reg = <0>;
		spi-max-frequency = <1000000>;
		resistance-at-zero = <100>;
		resistance-reference = <430>;
		maxim,3-wire;
		filter-50hz;
		spi-cpha;
	};

``` 
DTS Binding Documentation can be found at
* https://docs.zephyrproject.org/latest/build/dts/api/bindings/sensor/maxim%2Cmax31865.html

The MAX31865 supports SPI Mode 1 & 3. From the datasheet, "The MAX31865 automatically accommodates either clock polarity by sampling SCLK when CS becomes active to determine the polarity of the inactive clock." CPHA must be set to 1 as the MAX31865 uses the falling edge to clock in data.  