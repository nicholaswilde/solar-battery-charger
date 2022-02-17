/*--------------------------------------------------------------
  Program:      https

  Description:  Connect to a secure site and print the response.

  Hardware:     Adafruit Feather Huzzah

  Software:     Developed using arduino-cli 0.21.0.

  Date:         15FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "secrets.h"
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const uint8_t fingerprint[20] = {
  0xfc, 0xa7, 0xe7, 0xa0, 0x63, 0xfa, 0xa5, 0xbb, 0xd2, 0xd9, 0x40, 0x42, 0x41, 0x62, 0xc5, 0xd8, 0x2e, 0x92, 0x90, 0x9f
};

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 10000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)

Ticker blinker;

const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password
const int ledPin = LED_BUILTIN;
const char * myHostName = SECRET_HOSTNAME;
const char * host = "https://jigsaw.w3.org/HTTP/connection.html";

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  Serial.println("test: https");
  pinMode(ledPin, OUTPUT);
  blinker.attach_ms(INTERVAL_BLINK, changeState);
  conntectToWifi();
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, host)) {

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  Serial.println("Wait 10s before next round...");

  delay(DELAY_LOOP);
}

void conntectToWifi(){
  // Connect or reconnect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  Serial.print("Connecting to SSID: ");
  Serial.println(SECRET_SSID);
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
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

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}
