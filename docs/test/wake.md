# wake

This code displays how to use deep sleep with a touch as a wake up
source and how to store data in RTC memory to use it over reboots.

## Sketch

The sketch can be found [here][1].

## Hypothesis

The ESP32 can determine which method was used to wake it.

The ESP32 can determine which button was pressed to wake it.

## Assumptions

WIP

## Procedure

### Circuit

![](./images/circuit-wake.png)

Circuit made with [Circuit Diagram][2].

### Code

```shell
task test:compile-upload NAME=wake
```

### Output

```shell
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:x0xx,hd)drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0018,len:4
load:0x3fff001c,len:812
load:0x40078000,len:0
load:0x40078000,len:11392
entry 0x40078a9c
Boot number: 4
Wakeup caused by touchpad
Touch detected on GPIO 15
Going to sleep now
```

## Analysis

WIP

## Conclusion

WIP

## References
- https://github.com/RuiSantosdotme/ESP32-Course/blob/fdc20f81abea74e976b9a3334a18deebac496d6b/code/DeepSleep/TouchWakeUp/TouchWakeUp.ino
- https://randomnerdtutorials.com/esp32-touch-wake-up-deep-sleep/

[1]: https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/wake
[2]: https://www.circuit-diagram.org/
