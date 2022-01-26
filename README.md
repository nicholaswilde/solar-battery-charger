# :battery: Solar Battery Charger

A solar battery charger to charge lithium ion (li-ion) 18650, 9V, and coin batteries and Ni-MH AA and AAA batteries.

![](./images/charger.jpg)

## :goal_net:&nbsp; Objectives
- Recharge li-ion 18650, 9V, and coin batteries via a solar panel.
- Recharge Ni-MH AA and AAA batteries indirectly via a solar panel.
- Make the li-ion battery pack portable to easily recharge other devices. 
- Receive notificatinos when the li-ion batteries are fully charged.
- Monitor the rate of charging via an online IOT service such as [ThingSpeak](https://thingspeak.com/) or [Adafruit IO](https://io.adafruit.com/). 
- Learn about solar cells, panels, li-ion, and [Ni-MH batteries](https://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery).

## :framed_picture:&nbsp; Background

I recently replaced my solar landscape lights with hardwired ones and so I had a bunch of solar cells that I didn't
know what to do with. I decided to salvage the solar cells to build myself a solar battery charger since I try to
use as many rechargable batteries as possible as well as learn about solar panels and rechargable batteries. The
problem with recharging Ni-MH batteries is that they're difficult to monitor and prevent overcharging. Therefore,
I decided to create a lithium ion solar charger that then can be used to charge Ni-MH batteries via existing USB
chargers.

## :bulb:&nbsp; Concept

The concept is to create a solar panel and circuit to charge 18650 li-ion batteries and then use the charged li-ion
batteries to charge Ni-MH batteries via existing USB chargers. The li-ion batteries are to be disconnectable from
the solar panel to make more portable to recharge other devices and batteries.

Monitoring is handled by an Adafruit Feather Huzzah (Feather) that has built in wifi support which makes it easy to
monitor remotely as well as collect data. The board will monitor when the li-ion battery is charging and when
charging is complete. It can also send notifications when the charge is complete via IFTTT.

## Current Limitations

As of the time of this writing, charging the li-ion batteries and using the USB port need to happen separately.
Therefore, charging the li-ion batteries and charging other things with the USB port can't happen at the same time.

## :desktop_computer:&nbsp; Hardware
- 15X [Energizer LED Solar pathway Lights](https://outdoorsolaroutlet.com/products/new-led-energizer-10pack-solar-pathway-lights-outdoor-stainless-steel-bronze)
- 1X [Adafruit Feather Huzzah ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/)
- 2X [IMREN 3.7v 18650 Rechargeable Battery 3000mAh](https://www.amazon.com/gp/product/B09L5M1WN6/)
- 1X [NITECORE UMS2 Charger](https://www.amazon.com/gp/product/B07JN49XYM/)
- 1X [HiLetgo TP4056 Type-C USB 5V 1A 18650 Lithium Battery Charger Module](https://www.amazon.com/gp/product/B07PKND8KG/)
- 1X [DC-DC 0.9V-5V to USB 5V Boost Step-up Power Supply Module Booster Circuit Board](https://www.amazon.com/gp/product/B07QKYHFJB/)
- 1X [18650 2 Battery Holder 7.4V](https://www.amazon.com/gp/product/B08B86KHB2/)
- 1X [1N4007 Schottky Diode](https://www.amazon.com/gp/product/B07Q5FZR7X/)
- 1X [EBL 9V Li-ion Battery Charger](https://www.amazon.com/gp/product/B00HV4KFSA/)
- 1X [CT-Energy Lithium Coin Button Batteries Charger](https://www.amazon.com/gp/product/B088ZB9YZT/)
- 1X [Lithium Ion Polymer Battery - 3.7v 500mAh](https://www.adafruit.com/product/1578)
- 1X [220kΩ Resistor](https://www.amazon.com/gp/product/B072BL2VX1/)
- 1X [1MΩ Resistor](https://www.amazon.com/gp/product/B072BL2VX1/)
- 1X [JST-PH 2.0 Male and Female Connector Cable](https://www.amazon.com/gp/product/B07NWD5NTN)

## :robot:&nbsp; Services
- [IFTTT](https://ifttt.com/)
- [ThingSpeak](https://thingspeak.com/)
- [Adafruit IO](https://io.adafruit.com/)

## :floppy_disk:&nbsp; Software
- [arduino-cli](https://arduino.github.io/arduino-cli/latest/installation)
- [ubuntu server](https://ubuntu.com/download/server)
- [go-task](https://github.com/go-task/task)
- [pre-commit](https://pre-commit.com/)
- [GNU Screen](https://www.gnu.org/software/screen/)

## :electric_plug:&nbsp; Schematics

WIP

## :gear:&nbsp; Configuration

```shell
arduino-cli config init
arduino-cli config add board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
cat ~/.arduino15/arduino-cli.yaml
```

```yaml
# ~/.arduino15/arduino-cli.yaml
board_manager:
  additional_urls:
    - https://arduino.esp8266.com/stable/package_esp8266com_index.json
...
```

```shell
arduino-cli core update-index
arduino-cli board search huzzah
```

Typical Output

```shell
Board Name                      FQBN                   Platform ID
Adafruit Feather HUZZAH ESP8266 esp8266:esp8266:huzzah esp8266:esp8266
```

Compile the sketch

From the directory of the sketch:

```shell
arduino-cli compile -b esp8266:esp8266:huzzah .
```

Typical Output

```shell
Executable segment sizes:
ICACHE : 32768           - flash instruction cache 
IROM   : 236116          - code in flash         (default or ICACHE_FLASH_ATTR) 
IRAM   : 27273   / 32768 - code in IRAM          (IRAM_ATTR, ISRs...) 
DATA   : 1496  )         - initialized variables (global, static) in RAM/HEAP 
RODATA : 908   ) / 81920 - constants             (global, static) in RAM/HEAP 
BSS    : 25680 )         - zeroed variables      (global, static) in RAM/HEAP 
Sketch uses 265793 bytes (25%) of program storage space. Maximum is 1044464 bytes.
Global variables use 28084 bytes (34%) of dynamic memory, leaving 53836 bytes for local variables. Maximum is 81920 bytes.
```

Be sure to compile your code before uploading it!

Upload the sketch to the Feather

```shell
arduino-cli upload -p /dev/ttyUSB0 -b esp8266:esp8266:huzzah .
```

Typical Output

```shell
esptool.py v3.0
Serial port /dev/ttyUSB0
Connecting........_____.
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: bc:dd:c2:2d:ce:0d
Uploading stub...
Running stub...
Stub running...
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 269952 bytes to 198163...
Wrote 269952 bytes (198163 compressed) at 0x00000000 in 17.5 seconds (effective 123.5 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```

Monitor serial connection using GNU Screen

Make sure your baud rate matches the baud rate inside of your sketches!

```shell
screen /dev/ttyUSB0 115200
```

Kill the monitoring screen by pressing Ctrl+a k y

### :key:&nbsp; Secrets

`secrets.h` is a file used to store secrets such as API tokens and wifi
credentials that aren't updated in GitHub. The file is ignored in `.gitignore`.

Secrets can be used in sketches using the following code:

```C++
#include "secrets.h"
```

### Metadata

Instead of specifying `-b` and `-p` for every command, a [sketch.json](https://arduino.github.io/arduino-cli/0.20/sketch-specification/#metadata) may be stored in the sketch directory
that sets the defaults for that sketch.

The sketch.json file may be generated by using the [arduino-cli board attach](https://arduino.github.io/arduino-cli/0.20/commands/arduino-cli_board_attach/) command.

```shell
arduino-cli board attach -b esp8266:esp8266:huzzah .
arduino-cli board attach -p /dev/ttyUSB0 .
```

Attaching the board didn't work for me because `arduino-cli` didn't recognize
the Feather attached to my port for some reason and so I had to manually add
the port.

Note: The port needs to start with the protocol, e.g. `serial://`.

The documentation doesn't show what a typical sketch.json looks like so here is one:

`sketch.json`
```json
{
  "cpu": {
    "fqbn": "esp8266:esp8266:huzzah",
    "port": "serial:///dev/ttyUSB0"
  }
}
```

## :test_tube:&nbsp; Tests
- [ ] adafruitio
- [X] [battery](./test/battery)
- [X] [blink](./test/blink)
- [ ] cell
- [ ] notify
- [ ] panel
- [X] [sleep](./test/sleep)
- [ ] thingspeak
- [ ] v1
- [ ] v2
- [X] [wifi](./test/wifi)

## :clipboard:&nbsp; Todo

### Design
- [ ] Wooden solar panel box designed as panel frame.

### Tests
- [ ] Digital IO test with optocoupler on TP4056 red and blue LEDs to monitor.
- [ ] Upload data to ThinkSpeak/Adafruit IO.
- [X] Deep sleep to preserve battery.
- [ ] Notifications for Amazon Alexa and mobile phone.
- [ ] Power Feather by charging li-ion batteries and/or solar panel using a [pmosfet](https://www.best-microcontroller-projects.com/tp4056-page2.html).
- [ ] Determine the optimal number of 18650 li-ion batteries to charge.
- [ ] Optimize solar panel.

### Tools
- [ ] Implement `go-task`.
- [ ] Implement `pre-commit`.
- [ ] Create test template.

### Documentation
- [ ] Write troubleshooting section.
- [ ] Create schematics.
- [ ] Add images.
- [ ] Cleanup `Configuration` section.
- [ ] Write `test` READMEs.
- [ ] Write up `v1`, `v2`, `cell`, and `panel` tests.

## :hammer_and_wrench:&nbsp; Troubleshooting

WIP

## :link:&nbsp; References
- https://www.seeedstudio.com/blog/2019/10/09/voltage-dividers-everything-you-need-to-know/
- https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-huzzah-esp8266.pdf
- http://web.mit.edu/evt/summary_battery_specifications.pdf
- https://dronebotworkshop.com/dc-volt-current/#Voltage_References
- https://learn.adafruit.com/using-ifttt-with-adafruit-io/arduino-code-2
- https://learn.adafruit.com/using-ifttt-with-adafruit-io/wiring#battery-tracking
- https://siytek.com/arduino-cli-raspberry-pi/
- https://www.best-microcontroller-projects.com/tp4056-page2.html
- https://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery
- https://arduino.github.io/arduino-cli/

## :balance_scale:&nbsp; License

[Apache 2.0 License](./LICENSE)

## :pencil:&nbsp; Author
This project was started in 2022 by [Nicholas Wilde](https://github.com/nicholaswilde/).
