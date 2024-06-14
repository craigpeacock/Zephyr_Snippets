# Zephyr Modbus RTU Client Example

* [Zephyr Modbus RTU client Documentation](https://docs.zephyrproject.org/latest/samples/subsys/modbus/rtu_client/README.html)

It has been noted this framework doesn't work well under load, resulting in CRC mismatches & random RX timeouts etc. Discussion can be found [here.](https://github.com/zephyrproject-rtos/zephyr/pull/74029) I have observed the same behaviour.

## Device Tree

This example comes with a app.overlay file describing the RS-485 hardware. The UART interface is found via the "zephyr,modbus-serial" statement. The DE (Driver Enable) & RE (Receiver Enable) lines are configured using de-gpio & re-gpio properties.

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

## KConfig Options

Failure to include CONFIG_UART_INTERRUPT_DRIVEN=y can result in timeouts:

```
[00:00:22.473,937] <wrn> modbus: Client wait-for-RX timeout
[00:00:22.473,968] <err> main: FC01 failed with -116
```

## Work Function

Use #define USE_WORK_FUNCTION to select if modbus_read_sensor() is called from main or from a work function.

If called from main, the routine works correctly:

```
[00:00:00.382,904] <inf> main: Zephyr Modbus Example
Board: lemon_iot_lte_nrf9160
[00:00:00.382,904] <inf> main: Initialising Modbus to 4800 baud
[00:00:00.382,965] <inf> modbus_serial: RTU timeout 8020 us
[00:00:00.476,226] <inf> main: Data = 158, 180
[00:00:01.572,326] <inf> main: Data = 158, 180
[00:00:02.666,473] <inf> main: Data = 158, 180
[00:00:03.761,688] <inf> main: Data = 158, 180
[00:00:04.856,018] <inf> main: Data = 158, 180
[00:00:05.952,239] <inf> main: Data = 158, 180
[00:00:07.046,356] <inf> main: Data = 158, 180
[00:00:08.139,739] <inf> main: Data = 158, 180
[00:00:09.232,940] <inf> main: Data = 158, 180
[00:00:10.326,354] <inf> main: Data = 158, 180
```

If called from a work function, the routine is erratic:

```
[00:00:00.370,422] <inf> main: Zephyr Modbus Example
Board: lemon_iot_lte_nrf9160
[00:00:00.370,422] <inf> main: Initialising Modbus to 4800 baud
[00:00:00.370,483] <inf> modbus_serial: RTU timeout 8020 us
[00:00:02.420,776] <wrn> modbus: Client wait-for-RX timeout
[00:00:02.420,806] <err> main: FC01 failed with -116
[00:00:02.470,916] <wrn> modbus: Client wait-for-RX timeout
[00:00:02.470,947] <err> main: FC01 failed with -116
[00:00:02.470,947] <inf> main: Data = 0, 0
[00:00:03.521,179] <wrn> modbus: Client wait-for-RX timeout
[00:00:03.521,209] <err> main: FC01 failed with -116
[00:00:03.521,209] <inf> main: Data = 181, 0
[00:00:04.571,441] <wrn> modbus: Client wait-for-RX timeout
[00:00:04.571,472] <err> main: FC01 failed with -116
[00:00:04.621,582] <wrn> modbus: Client wait-for-RX timeout
[00:00:04.621,612] <err> main: FC01 failed with -116
[00:00:04.621,612] <inf> main: Data = 0, 0
[00:00:05.671,844] <wrn> modbus: Client wait-for-RX timeout
[00:00:05.671,875] <err> main: FC01 failed with -116
[00:00:05.671,875] <inf> main: Data = 180, 0
[00:00:06.722,106] <wrn> modbus: Client wait-for-RX timeout
[00:00:06.722,137] <err> main: FC01 failed with -116
[00:00:06.772,247] <wrn> modbus: Client wait-for-RX timeout
[00:00:06.772,277] <err> main: FC01 failed with -116
[00:00:06.772,277] <inf> main: Data = 0, 0
[00:00:07.822,509] <wrn> modbus: Client wait-for-RX timeout
[00:00:07.822,540] <err> main: FC01 failed with -116
[00:00:07.822,540] <inf> main: Data = 180, 0
[00:00:08.872,772] <wrn> modbus: Client wait-for-RX timeout
[00:00:08.872,802] <err> main: FC01 failed with -116
[00:00:08.922,912] <wrn> modbus: Client wait-for-RX timeout
[00:00:08.922,943] <err> main: FC01 failed with -116
[00:00:08.922,943] <inf> main: Data = 0, 0
[00:00:09.973,175] <wrn> modbus: Client wait-for-RX timeout
[00:00:09.973,205] <err> main: FC01 failed with -116
[00:00:09.973,205] <inf> main: Data = 180, 0
[00:00:11.023,529] <wrn> modbus: Client wait-for-RX timeout
[00:00:11.023,559] <err> main: FC01 failed with -116
[00:00:11.073,669] <wrn> modbus: Client wait-for-RX timeout
[00:00:11.073,699] <err> main: FC01 failed with -116
[00:00:11.073,699] <inf> main: Data = 0, 0
[00:00:12.123,931] <wrn> modbus: Client wait-for-RX timeout
[00:00:12.123,962] <err> main: FC01 failed with -116
[00:00:12.123,962] <inf> main: Data = 180, 0
```