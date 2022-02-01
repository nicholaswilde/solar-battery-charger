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
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)

WiFiClient  client;
Ticker blinker;

const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password
const int ledPin = LED_BUILTIN;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

void setup() {
  Serial.begin(BAUD_RATE);
  while(! Serial);
  Serial.println();
  Serial.println("test: createdat");
  ThingSpeak.begin(client);
  blinker.attach_ms(INTERVAL_BLINK, changeState);
  connectToWifi();
}

void loop() {
  String val = getCreatedAt();
  Serial.println(val);
  delay(DELAY_LOOP);
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}

void connectToWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  // Connect or reconnect to WiFi
  Serial.print("Connecting to SSID: ");
  Serial.println(SECRET_SSID);
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    Serial.print(".");
    delay(DELAY_WIFI*1e3);
  }
  blinker.detach();
  Serial.println("connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.hostname());
}

// Format: 2017-01-12 13:22:54
String getCreatedAt(){
  return ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);
}