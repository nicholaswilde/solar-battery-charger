# sunset

Get the sunrise and sunset times from a website.

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

WIP

### Circuit

![](./images/circuit.png)

Circuit made with [Circuit Diagram](https://www.circuit-diagram.org/)

### Code

Install library

```shell
arduino-cli lib install ArduinoJson
```

Update GPS coordinates.

```c++
const char * lat = "33.60";
const char * lng = "-117.68";
```

### Output

All dates are returned as UTC.

```shell

test: sunset
Connecting to SSID: MySSID
.....connected
IP Address: 192.168.1.77
Hostname: Feather
host: http://api.sunrise-sunset.org/json?&date=today&formatted=0&lat=33.60&lng=-117.68
HTTP Response code: 200
Sunrise: 2022-02-04T14:43:05+00:00
Sunset: 2022-02-05T01:26:11+00:00
host: http://api.sunrise-sunset.org/json?&date=today&formatted=0&lat=33.60&lng=-117.68
HTTP Response code: 200
Sunrise: 2022-02-04T14:43:05+00:00
Sunset: 2022-02-05T01:26:11+00:00
...
```

## Analysis

WIP

## Conclusion

WIP

## Troubleshooting

WIP

## References
- https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/#http-get-2
- https://sunrise-sunset.org/api
- https://arduinojson.org/v6/example/parser/
