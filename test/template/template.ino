/*--------------------------------------------------------------
  Program:      ${NAME}

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a lipo battery.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli ${VERSION}.

  Date:         ${DATE}

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <Ticker.h> 
#include "secrets.h"

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)

WiFiClient  client;
Ticker blinker;

char ssid[] = SECRET_SSID;      // your network SSID (name)
char pass[] = SECRET_PASS;      // your network password

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

unsigned long previousMillis = 0;    // will store last time LED was updated
const long interval = 100;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(BAUD_RATE);
  
  WiFi.mode(WIFI_STA);

  WiFi.hostname(myHostName);
  
  blinker.attach_ms(INTERVAL_BLINK, changeState);

  Serial.println();
}

void loop() {
  Serial.println("${NAME}");
  int val = analogRead(ANALOG_PIN_NO)
  Serial.println(val);
  delay(DELAY_LOOP);
}

void changeState(){
  digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
}
