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

