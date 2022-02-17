# clear

Clear a ThingSpeak channel.

## Sketch

The sketch can be found [here][1].

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

### Circuit

![](./images/circuit-clear.png)

Circuit made with [Circuit Diagram][2].

### Code

Add `SECRET_USER_APIKEY` to `secrets.h`. This can be obtained from the
[ThingSpeak profile page][3].

Update `fingerprint` in the sketch with the SHA-1 fingerprint in the sketch if
it is out of date. The recent fingerprint can be obtained by
[looking the certificate in Google Chrome][4].

### Output

```shell
test: clear
Connecting to SSID: MySSID
.connected
IP Address: 192.168.1.77
Hostname: Feather
[HTTPS] begin...
http code: 200
status: success
```

## Analysis

WIP

## Conclusion

WIP

## References
- https://www.mathworks.com/help/thingspeak/clearchannel.html

[1]: https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/clear
[2]: https://www.circuit-diagram.org/
[3]: https://thingspeak.com/account/profile
[4]: https://www.a2hosting.com/kb/security/ssl/a2-hostings-ssl-certificate-fingerprints
