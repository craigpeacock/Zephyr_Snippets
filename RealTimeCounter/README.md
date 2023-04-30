# Zephyr Real Time Counter Example

Zephyr has support for Real-time clock and Real-time counter. 

The clock provides a broken-down time format, while the counter provides a low power counter that can be used to track time. Both rely on hardware that can be used to wake-up the system. 

The following links are to Zephyr documentation (including API): 
 * https://docs.zephyrproject.org/latest/hardware/peripherals/counter.html
 * https://docs.zephyrproject.org/latest/hardware/peripherals/rtc.html

## Output

```
*** Booting Zephyr OS build zephyr-v3.3.0-3210-g19a4602312da ***
Zephyr Real Time Counter Example
Board: nucleo_f411re
Number of counters: 1
Counter frequency: 1Hz
Wakeup Capable: No
Wakeup Source: Disabled
Successfully set alarm
Counter value 0
Counter value 1
Counter value 2
Counter value 3
Counter value 4
Counter value 5
Counter value 6
Counter value 7
Counter value 8
Counter value 9
Counter value 10
Alarm
Counter value 11
Counter value 12

```

## Build

This example has been tested on the nucleo_f411re board. To build, run:

```
west build -b nucleo_f411re
```

To flash, run:

```
west flash
```

