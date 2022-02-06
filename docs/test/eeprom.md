# eeprom

Stores values read from analog input 0 into
the EEPROM. These values will stay in the
EEPROM when the board is turned off and may
be retrieved later by another sketch.

## Sketch

The sketch can be found [here](https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/eeprom).

## Hypothesis

A variable value is lost when the Feather goes into a deep sleep
 but can be stored and retrieved in the EEPROM.

## Assumptions

WIP

## Procedure

### Circuit

Same circuit as [battery](../battery).

![](../battery/images/circuit.png)

Circuit made with [Circuit Diagram](https://www.circuit-diagram.org/)

### Code

The Feather takes a reading of the `A0` pin, divides the value by `4`, prints the value, then saves it to the `EEPROM`.

The Feather then goes into a deep sleep for a period of time.

On next bootup, the same variable is printed to the console and the `EEPROM` saved values are retrieved and printed to the console.

### Output

The `Retrieved pin value` of the current reading should match the `Write pin value` of the
previous reading.

The `Retrieved didNotify` of the current reading should match the `Write didNotify` of the
previous reading.

The `Variable value` should be `0` when the Feather wakes up again because the
variable is not retained.

```shell
rll��|�l�|�l�b|����r�b�b��nn�lnn���bp��lrlrlp�n��l��bn�|���b��nn�l��l`�nn�lnr���nrr�`p�n�r������bn�|�b��nn��l`�nn�lnr���nrl`r��nrl`���ll`�n�l
eeprom
Retrieved pin value: 239
Variable value: 0
Retrieved didNotify: 1
Write pin value: 240
Write didNotify: 0
EEPROM successfully committed
rll��|�l�|�l�b|����r�b�b��nn�lnn���bp��lrlrlp�n��l��bn�|���b��nn�l��l`�nn�lnr���nrr�`p�n�r������bn�|�b��nn��l`�nn�lnr���nrl`r��nrl`���ll`�n�l
eeprom
Retrieved pin value: 240
Variable value: 0
Retrieved didNotify: 0
Write pin value: 240
Write didNotify: 1
EEPROM successfully committed
...
```

## Analysis

Everything worked as expected.

The original variable was reset to `0` after waking up from a deep sleep.

The value was able to be successfully stored and retrieved in the `EEPROM`
before and after the deep sleep respectively.

## Conclusion

If a sketch uses the deep sleep mode and a previous values is to be used again,
 it firsts needs to be saved to the EEPROM and then retrieved on the next boot.

## Troubleshooting

WIP

## References
- https://github.com/esp8266/Arduino/blob/9f536e68f8f3aa3ffae26493dd0a31e1957d2e81/libraries/EEPROM/examples/eeprom_write/eeprom_write.ino
- https://github.com/esp8266/Arduino/blob/9f536e68f8f3aa3ffae26493dd0a31e1957d2e81/libraries/EEPROM/examples/eeprom_read/eeprom_read.ino
- https://arduino.stackexchange.com/a/33363
- https://forum.arduino.cc/t/two-newbie-questions-status-and-printing-a-boolean/77672/3
