# solar-battery-charger

[Adafruit Feather Huzzah ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/)

[Install arduino-cli](https://arduino.github.io/arduino-cli/latest/installation)

```
arduino-cli config init
arduino-cli config add board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

`cat ~/.arduino15/arduino-cli.yaml`

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
