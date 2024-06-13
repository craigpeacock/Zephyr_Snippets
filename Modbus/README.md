# Zephyr Modbus RTU Client Example

* [Zephyr Modbus RTU client](https://docs.zephyrproject.org/latest/samples/subsys/modbus/rtu_client/README.html)

It has been noted this framework doesn't work well under load, resulting in CRC mismatches etc. Discussion can be found [here.](https://github.com/zephyrproject-rtos/zephyr/pull/74029)

This example comes with a app.overlay file describing the RS-485 hardware. The UART interface is found via the "zephyr,modbus-serial" statement. The DE (Data Enable) & RE (Receive Enable) lines are configured using de-gpio & re-gpio properties. 

```
&uart1 {
	status = "okay";
	current-speed = <9600>;
	modbus0 {
		compatible = "zephyr,modbus-serial";
		status = "okay";
		de-gpios = <&gpio0 7 GPIO_ACTIVE_HIGH>;
		re-gpios = <&gpio0 6 GPIO_ACTIVE_LOW>;
	};
};
```