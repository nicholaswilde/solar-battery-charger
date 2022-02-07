# adjusttime

WIP

## Sketch

Sketch can be found [here](https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/adjusttime).

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

WIP

### Circuit

![](../assets/images/circuit-blink.png)

Circuit made with [Circuit Diagram](https://www.circuit-diagram.org/)

### Code

```shell title="Install the time library"
arduino-cli lib install time
```

```shell title="Install the Timezone_Generic library"
arduino-cli lib install Timezone_Generic
```

See [section below](#trouble-installing-timezone_generic-library) if having trouble installing [Timezone_Generic](https://www.arduino.cc/reference/en/libraries/timezone_generic/).

### Output

WIP

## Analysis

WIP

## Conclusion

WIP

## Troubleshooting

### Trouble installing Timezone_Generic library

Manually install by [changing config](https://arduino.github.io/arduino-cli/0.20/configuration/#configuration-keys) of `enable_unsafe_install` to true

Download the [latest release](https://github.com/khoih-prog/Timezone_Generic/releases/latest) zip file.

```shell
arduino-cli lib install --zip-path v1.9.1.zip
```

## References
- https://github.com/khoih-prog/Timezone_Generic/blob/523402eee8e74a6499431b59afd81d12cfce032d/examples/tzTest/tzTest.ino
- https://forum.arduino.cc/t/loading-my-p-c-time-and-date-into-the-arduino-during-the-programing/586028/5
