# oled

Test the [Adafruit 128x64 OLED FeatherWing](https://www.adafruit.com/product/4650) display.

## Sketch

The sketch can be founder [here](https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/oled).

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

WIP

### Circuit

![](../assets/images/circuit-oled.png)

Circuit made with [Circuit Diagram](https://www.circuit-diagram.org/)

### Code

```shell title="Install Adafruit SH110X library"
arduino-cli lib install "Adafruit SH110X"
```

```shell title="Install Adafruit GFX Library"
arduino-cli lib install "Adafruit GFX Library"
```

### Output

```shell title="Serial Output"
128x64 OLED FeatherWing test
OLED begun
Button test
```

The `AAAABBBBBBCCCC` is printed when the A, B, & C buttons on the OLED screen are pressed.

```shell title="OLED Output"
Connecting to SSID
'adafruit':connected!
IP: 10.0.1.23
Sending val #0
AAAABBBBBBCCCC
```

## Analysis

WIP

## Conclusion

WIP

## References
- https://learn.adafruit.com/adafruit-128x64-oled-featherwing/arduino-code
- https://learn.adafruit.com/adafruit-gfx-graphics-library
