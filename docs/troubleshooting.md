# :hammer_and_wrench: Troubleshooting

### Error during build: platform not installed

```shell
Error during build: platform not installed
```

```shell title="Fix"
arduino-cli core install esp8266:esp8266
```

### The internal LED is dim and the console is blank when connected to a computer via USB

Fix: The Feather needs to be reset. Press the physical `RESET` button.

### Count not open port [Errno 16] Device or resource busy

```shell
serial.serialutil.SerialException: [Errno 16] could not open port /dev/ttyUSB0: [Errno 16] Device or resource busy: '/dev/ttyUSB0'
Error during Upload: Failed uploading: uploading error: exit status 1
```

Fix: The serial port is open and reading the output of the Feather. Close the connection

### Trouble installing Timezone_Generic library

Manually install by [changing config](https://arduino.github.io/arduino-cli/0.20/configuration/#configuration-keys) of `enable_unsafe_install` to true

Download the [latest release](https://github.com/khoih-prog/Timezone_Generic/releases/latest) zip file.

```shell
arduino-cli lib install --zip-path v1.9.1.zip
```

### Error during build: Missing FQBN (Fully Qualified Board Name)

```shell
Error during build: Missing FQBN (Fully Qualified Board Name)
```

Fix: Add the board (`-b`) parameter to compile or upload or add a [`sketch.json`](https://nicholaswilde.io/solar-battery-charger/configuration/#metadata).
