# Solar Battery Charger

## Hardware
- [Adafruit Feather Huzzah ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/)

## Installation
[Install arduino-cli](https://arduino.github.io/arduino-cli/latest/installation)

```
arduino-cli config init
arduino-cli config add board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
cat ~/.arduino15/arduino-cli.yaml
```

```
board_manager:
  additional_urls:
    - https://arduino.esp8266.com/stable/package_esp8266com_index.json
...
```

```
arduino-cli core update-index
arduino-cli board search huzzah
```

```
Board Name                      FQBN                   Platform ID
Adafruit Feather HUZZAH ESP8266 esp8266:esp8266:huzzah esp8266:esp8266
```

```
# Compile
arduino-cli compile -b esp8266:esp8266:huzzah wifi.ino --verbose
# Be sure to compiile your code before uploading it!
# Upload
arduino-cli upload -p /dev/ttyUSB0 -b esp8266:esp8266:huzzah wifi.ino --verbose
# Monitor serial connection
screen /dev/ttyUSB0 115200
```
## Tests
- [battery](./test/battery)
- [blink](./test/blink)
- [wifi](./test/wifi)

## Todo
- [ ] Digital IO test with optocoupler on TP4056 red and blue LEDs to monitor 
- [ ] Test to upload to thinkspeak

## References
- https://www.seeedstudio.com/blog/2019/10/09/voltage-dividers-everything-you-need-to-know/
- https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-huzzah-esp8266.pdf
- http://web.mit.edu/evt/summary_battery_specifications.pdf
- https://dronebotworkshop.com/dc-volt-current/#Voltage_References
- https://learn.adafruit.com/using-ifttt-with-adafruit-io/arduino-code-2
- https://learn.adafruit.com/using-ifttt-with-adafruit-io/wiring#battery-tracking
