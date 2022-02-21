# Design

## :desktop_computer: Hardware
- 15X [Energizer LED Solar pathway Lights][1]
- 1X [Adafruit Feather Huzzah ESP8266][2]
- 1X [Adafruit 128x64 OLED FeatherWing][3]
- 2X [IMREN 3.7v 18650 Rechargeable Battery 3000mAh][4]
- 1X [NITECORE UMS2 Charger][5]
- 1X [HiLetgo TP4056 Type-C USB 5V 1A 18650 Lithium Battery Charger Module][6]
- 1X [DC-DC 0.9V-5V to USB 5V Boost Step-up Power Supply Module Booster Circuit Board][7]
- 1X [18650 2 Battery Holder 7.4V][8]
- 1X [1N4007 Schottky Diode][9]
- 1X [EBL 9V Li-ion Battery Charger][10]
- 1X [CT-Energy Lithium Coin Button Batteries Charger][11]
- 1X [Lithium Ion Polymer Battery - 3.7v 500mAh][12]
- 1X [220kΩ Resistor][13]
- 1X [1MΩ Resistor][13]
- 2X [JST-PH 2.0 Male and Female Connector Cable][14]
- 1X [DPDT 2 Position 6 Terminals Panel Mount Horizontal Slide Switch][15]
- 1X [400 Point Breadboard][17]

### :zap: Electronics

<figure Markdown>
  ![](./assets/images/front.jpg){width=480}
</figure>

<figure Markdown>
  ![](./assets/images/back-v4.jpg){width=480}
</figure>

<figure Markdown>
  ![](./assets/images/top-v4.jpg){width=480}
</figure>

### :sunny: Panel

<figure Markdown>
  ![](./assets/images/charger.jpg){width=480}
</figure>

<figure Markdown>
  ![](./assets/images/panel-back.jpg){width=480}
</figure>

## :robot: Services
- [IFTTT](https://ifttt.com/)
- [ThingSpeak](https://thingspeak.com/)

## :floppy_disk: Software
- [arduino-cli](https://arduino.github.io/arduino-cli/latest/installation)
- [ubuntu server](https://ubuntu.com/download/server)
- [go-task](https://github.com/go-task/task)
- [GNU Screen](https://www.gnu.org/software/screen/)
- [jq](https://stedolan.github.io/jq) (for go-task)
- [pre-commit](https://pre-commit.com/)
- [arduino-lint](https://arduino.github.io/arduino-lint/)
- [yamllint](https://github.com/adrienverge/yamllint)

## :electric_plug: Circuit

- Connect the `RST` pin to pin `16` to allow the [Feather to wake from deep sleep][16].
- Create a voltage divider and connect the `A0` analog pin to the connection point.
- Connect a DPDT switch which allows power from the solar panel and no power to the output and vice versa.
- Insert a schottky diode between the solar panel and the `TP4056` input.

![](./assets/images/circuit.png)

Circuits made with [Circuit Diagram](https://www.circuit-diagram.org/)

!!! note
    The li-ion batteries are wired in parallel even though the 2 battery holder
    come wired in series.

!!! note
    The JST pins, depending on where they are purchased, may be backwards in terms of
    the wiring. See [this link](https://docs.particle.io/tutorials/learn-more/batteries/) to determine the correct wiring according to IoT
    convention.

IoT devices must use batteries wired like this: With the key facing down and the wires toward you, the red is on the right.

<figure Markdown>
  ![](https://docs.particle.io/assets/images/battery/close-up.jpg){width=480}
</figure>

[1]: https://outdoorsolaroutlet.com/products/new-led-energizer-10pack-solar-pathway-lights-outdoor-stainless-steel-bronze
[2]: https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/
[3]: https://www.adafruit.com/product/4650
[4]: https://www.amazon.com/gp/product/B09L5M1WN6/
[5]: https://www.amazon.com/gp/product/B07JN49XYM/
[6]: https://www.amazon.com/gp/product/B07PKND8KG/
[7]: https://www.amazon.com/gp/product/B07QKYHFJB/
[8]: https://www.amazon.com/gp/product/B08B86KHB2/
[9]: https://www.amazon.com/gp/product/B07Q5FZR7X/
[10]: https://www.amazon.com/gp/product/B00HV4KFSA/
[11]: https://www.amazon.com/gp/product/B088ZB9YZT/
[12]: https://www.adafruit.com/product/1578
[13]: https://www.amazon.com/gp/product/B072BL2VX1/
[14]: https://www.amazon.com/gp/product/B07NWD5NTN/
[15]: https://www.amazon.com/gp/product/B01N6ROMW3/
[16]: https://learnarduinonow.com/2018/07/05/esp8266-deep-sleep-mode.html
[17]: https://www.amazon.com/dp/B07PCJP9DY/
