# wifimanager

A basic test of using the WiFiManager to enter credentials.

## Sketch

The sketch can be found [here][1].

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

Install the WiFiManager library

Compile and run the test.

Connect to to the Feather via WiFi. The SSID is auto generated
by the type of Feather. My test was `ESP32_F0CE877C`.

Enter `192.168.4.1` in the browser.

Enter in the WiFii crednetials of the AP the Feather should connect
to.

### Circuit

Using the Adafruit Feather HUZZAH32.

![](./images/circuit-wifimanager.png)

Circuit made with [Circuit Diagram][2].

### Code

```shell title="Install WiFiManager library"
arduino-cli lib install WiFiManager
```

### Output

```shell
*wm:[1] AutoConnect
*wm:[2] ESP32 event handler enabled 
*wm:[2] Connecting as wifi client...
*wm:[2] setSTAConfig static ip not set, skipping 
*wm:[1] Connecting to SAVED AP: MySSID
*wm:[1] connectTimeout not set, ESP waitForConnectResult... 
*wm:[2] Connection result: WL_CONNECTED
*wm:[1] AutoConnect: SUCCESS
*wm:[2] Connected in 2719 ms
*wm:[1] STA IP Address: 192.168.186.85
connected...yeey :)
```

## Analysis

WIP

## Conclusion

WIP

## References
- https://github.com/tzapu/WiFiManager/blob/e6d21b84364524f474f05b1fddf97541bd174e7a/examples/Basic/Basic.ino

[1]: https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/wifimanager
[2]: https://www.circuit-diagram.org/
[3]: 
