# adjusttime

Sketch to verify operation of Timezone library.

## Sketch

Sketch can be found [here](https://github.com/nicholaswilde/solar-battery-charger/tree/main/test/adjusttime).

## Hypothesis

WIP

## Assumptions

WIP

## Procedure

WIP

### Circuit

![](../assets/images/circuit-blink.png)

Circuit made with [Circuit Diagram](https://www.circuit-diagram.org/)

### Code

```shell title="Install the time library"
arduino-cli lib install time
```

```shell title="Install the Timezone_Generic library"
arduino-cli lib install Timezone_Generic
```

See [here](../../troubleshooting#trouble-installing-timezone_generic-library) if having trouble installing [Timezone_Generic](https://www.arduino.cc/reference/en/libraries/timezone_generic/).

### Output

```shell title="Typical Output"
Start TZTest on ESP8266_ADAFRUIT_HUZZAH
Timezone_Generic v1.9.1

-------- Apr-2018 time change --------
                                      13:59:57 Sat 31 Mar 2018 UTC =  2:59:57 Sun  1 Apr 2018 NZDT
13:59:58 Sat 31 Mar 2018 UTC =  2:59:58 Sun  1 Apr 2018 NZDT
13:59:59 Sat 31 Mar 2018 UTC =  2:59:59 Sun  1 Apr 2018 NZDT
14: 0: 0 Sat 31 Mar 2018 UTC =  2: 0: 0 Sun  1 Apr 2018 NZST
14: 0: 1 Sat 31 Mar 2018 UTC =  2: 0: 1 Sun  1 Apr 2018 NZST
14: 0: 2 Sat 31 Mar 2018 UTC =  2: 0: 2 Sun  1 Apr 2018 NZST

-------- Apr-2018 time change --------
                                      13:59:57 Sat 31 Mar 2018 UTC =  2:59:57 Sun  1 Apr 2018 NZDT
13:59:58 Sat 31 Mar 2018 UTC =  2:59:58 Sun  1 Apr 2018 NZDT
13:59:59 Sat 31 Mar 2018 UTC =  2:59:59 Sun  1 Apr 2018 NZDT
14: 0: 0 Sat 31 Mar 2018 UTC =  2: 0: 0 Sun  1 Apr 2018 NZST
14: 0: 1 Sat 31 Mar 2018 UTC =  2: 0: 1 Sun  1 Apr 2018 NZST
14: 0: 2 Sat 31 Mar 2018 UTC =  2: 0: 2 Sun  1 Apr 2018 NZST

-------- Sep-2018 time change --------
                                      13:59:57 Sat 29 Sep 2018 UTC =  1:59:57 Sun 30 Sep 2018 NZST
13:59:58 Sat 29 Sep 2018 UTC =  1:59:58 Sun 30 Sep 2018 NZST
13:59:59 Sat 29 Sep 2018 UTC =  1:59:59 Sun 30 Sep 2018 NZST
14: 0: 0 Sat 29 Sep 2018 UTC =  3: 0: 0 Sun 30 Sep 2018 NZDT
14: 0: 1 Sat 29 Sep 2018 UTC =  3: 0: 1 Sun 30 Sep 2018 NZDT
14: 0: 2 Sat 29 Sep 2018 UTC =  3: 0: 2 Sun 30 Sep 2018 NZDT

-------- Apr-2019 time change --------
                                      13:59:57 Sat  6 Apr 2019 UTC =  2:59:57 Sun  7 Apr 2019 NZDT
13:59:58 Sat  6 Apr 2019 UTC =  2:59:58 Sun  7 Apr 2019 NZDT
13:59:59 Sat  6 Apr 2019 UTC =  2:59:59 Sun  7 Apr 2019 NZDT
14: 0: 0 Sat  6 Apr 2019 UTC =  2: 0: 0 Sun  7 Apr 2019 NZST
14: 0: 1 Sat  6 Apr 2019 UTC =  2: 0: 1 Sun  7 Apr 2019 NZST
14: 0: 2 Sat  6 Apr 2019 UTC =  2: 0: 2 Sun  7 Apr 2019 NZST

-------- Sep-2019 time change --------
                                      13:59:57 Sat 28 Sep 2019 UTC =  1:59:57 Sun 29 Sep 2019 NZST
13:59:58 Sat 28 Sep 2019 UTC =  1:59:58 Sun 29 Sep 2019 NZST
13:59:59 Sat 28 Sep 2019 UTC =  1:59:59 Sun 29 Sep 2019 NZST
14: 0: 0 Sat 28 Sep 2019 UTC =  3: 0: 0 Sun 29 Sep 2019 NZDT
14: 0: 1 Sat 28 Sep 2019 UTC =  3: 0: 1 Sun 29 Sep 2019 NZDT
14: 0: 2 Sat 28 Sep 2019 UTC =  3: 0: 2 Sun 29 Sep 2019 NZDT

-------- Apr-2020 time change --------
                                      13:59:57 Sat  4 Apr 2020 UTC =  2:59:57 Sun  5 Apr 2020 NZDT
13:59:58 Sat  4 Apr 2020 UTC =  2:59:58 Sun  5 Apr 2020 NZDT
13:59:59 Sat  4 Apr 2020 UTC =  2:59:59 Sun  5 Apr 2020 NZDT
14: 0: 0 Sat  4 Apr 2020 UTC =  2: 0: 0 Sun  5 Apr 2020 NZST
14: 0: 1 Sat  4 Apr 2020 UTC =  2: 0: 1 Sun  5 Apr 2020 NZST
14: 0: 2 Sat  4 Apr 2020 UTC =  2: 0: 2 Sun  5 Apr 2020 NZST

-------- Sep-2020 time change --------
                                      13:59:57 Sat 26 Sep 2020 UTC =  1:59:57 Sun 27 Sep 2020 NZST
13:59:58 Sat 26 Sep 2020 UTC =  1:59:58 Sun 27 Sep 2020 NZST
13:59:59 Sat 26 Sep 2020 UTC =  1:59:59 Sun 27 Sep 2020 NZST
14: 0: 0 Sat 26 Sep 2020 UTC =  3: 0: 0 Sun 27 Sep 2020 NZDT
14: 0: 1 Sat 26 Sep 2020 UTC =  3: 0: 1 Sun 27 Sep 2020 NZDT
14: 0: 2 Sat 26 Sep 2020 UTC =  3: 0: 2 Sun 27 Sep 2020 NZDT

-------- Mar-2018 time change --------
                                       6:59:57 Sun 11 Mar 2018 UTC =  1:59:57 Sun 11 Mar 2018 EST
 6:59:58 Sun 11 Mar 2018 UTC =  1:59:58 Sun 11 Mar 2018 EST
 6:59:59 Sun 11 Mar 2018 UTC =  1:59:59 Sun 11 Mar 2018 EST
 7: 0: 0 Sun 11 Mar 2018 UTC =  3: 0: 0 Sun 11 Mar 2018 EDT
 7: 0: 1 Sun 11 Mar 2018 UTC =  3: 0: 1 Sun 11 Mar 2018 EDT
 7: 0: 2 Sun 11 Mar 2018 UTC =  3: 0: 2 Sun 11 Mar 2018 EDT

-------- Nov-2018 time change --------
                                       5:59:57 Sun  4 Nov 2018 UTC =  1:59:57 Sun  4 Nov 2018 EDT
 5:59:58 Sun  4 Nov 2018 UTC =  1:59:58 Sun  4 Nov 2018 EDT
 5:59:59 Sun  4 Nov 2018 UTC =  1:59:59 Sun  4 Nov 2018 EDT
 6: 0: 0 Sun  4 Nov 2018 UTC =  1: 0: 0 Sun  4 Nov 2018 EST
 6: 0: 1 Sun  4 Nov 2018 UTC =  1: 0: 1 Sun  4 Nov 2018 EST
 6: 0: 2 Sun  4 Nov 2018 UTC =  1: 0: 2 Sun  4 Nov 2018 EST

-------- Mar-2019 time change --------
                                       6:59:57 Sun 10 Mar 2019 UTC =  1:59:57 Sun 10 Mar 2019 EST
 6:59:58 Sun 10 Mar 2019 UTC =  1:59:58 Sun 10 Mar 2019 EST
 6:59:59 Sun 10 Mar 2019 UTC =  1:59:59 Sun 10 Mar 2019 EST
 7: 0: 0 Sun 10 Mar 2019 UTC =  3: 0: 0 Sun 10 Mar 2019 EDT
 7: 0: 1 Sun 10 Mar 2019 UTC =  3: 0: 1 Sun 10 Mar 2019 EDT
 7: 0: 2 Sun 10 Mar 2019 UTC =  3: 0: 2 Sun 10 Mar 2019 EDT

-------- Nov-2019 time change --------
                                       5:59:57 Sun  3 Nov 2019 UTC =  1:59:57 Sun  3 Nov 2019 EDT
 5:59:58 Sun  3 Nov 2019 UTC =  1:59:58 Sun  3 Nov 2019 EDT
 5:59:59 Sun  3 Nov 2019 UTC =  1:59:59 Sun  3 Nov 2019 EDT
 6: 0: 0 Sun  3 Nov 2019 UTC =  1: 0: 0 Sun  3 Nov 2019 EST
 6: 0: 1 Sun  3 Nov 2019 UTC =  1: 0: 1 Sun  3 Nov 2019 EST
 6: 0: 2 Sun  3 Nov 2019 UTC =  1: 0: 2 Sun  3 Nov 2019 EST

-------- Mar-2020 time change --------
                                       6:59:57 Sun  8 Mar 2020 UTC =  1:59:57 Sun  8 Mar 2020 EST
 6:59:58 Sun  8 Mar 2020 UTC =  1:59:58 Sun  8 Mar 2020 EST
 6:59:59 Sun  8 Mar 2020 UTC =  1:59:59 Sun  8 Mar 2020 EST
 7: 0: 0 Sun  8 Mar 2020 UTC =  3: 0: 0 Sun  8 Mar 2020 EDT
 7: 0: 1 Sun  8 Mar 2020 UTC =  3: 0: 1 Sun  8 Mar 2020 EDT
 7: 0: 2 Sun  8 Mar 2020 UTC =  3: 0: 2 Sun  8 Mar 2020 EDT

-------- Nov-2020 time change --------
                                       5:59:57 Sun  1 Nov 2020 UTC =  1:59:57 Sun  1 Nov 2020 EDT
 5:59:58 Sun  1 Nov 2020 UTC =  1:59:58 Sun  1 Nov 2020 EDT
 5:59:59 Sun  1 Nov 2020 UTC =  1:59:59 Sun  1 Nov 2020 EDT
 6: 0: 0 Sun  1 Nov 2020 UTC =  1: 0: 0 Sun  1 Nov 2020 EST
 6: 0: 1 Sun  1 Nov 2020 UTC =  1: 0: 1 Sun  1 Nov 2020 EST
 6: 0: 2 Sun  1 Nov 2020 UTC =  1: 0: 2 Sun  1 Nov 2020 EST
...
```

## Analysis

WIP

## Conclusion

WIP

## References
- https://github.com/khoih-prog/Timezone_Generic/blob/523402eee8e74a6499431b59afd81d12cfce032d/examples/tzTest/tzTest.ino
- https://forum.arduino.cc/t/loading-my-p-c-time-and-date-into-the-arduino-during-the-programing/586028/5
