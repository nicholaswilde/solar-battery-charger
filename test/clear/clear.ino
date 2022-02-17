/*--------------------------------------------------------------
  Program:      clear

  Description:  Clear a ThingSpeak channel.

  Hardware:     Adafruit Feather Huzzah

  Software:     Developed using arduino-cli 0.21.0.

  Date:         16FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include "secrets.h"
#include <WiFiClientSecureBearSSL.h>

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 20000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)
#define THINGSPEAK_URL "api.thingspeak.com"

Ticker blinker;

const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password
const int ledPin = LED_BUILTIN;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;
const char * myUserAPIKey = SECRET_USER_APIKEY;

const uint8_t fingerprint[20] = {
  0x27, 0x18, 0x92, 0xdd, 0xa4, 0x26, 0xc3, 0x07, 0x09, 0xb9, 0x7a, 0xe6, 0xc5, 0x21, 0xb9, 0x5b, 0x48, 0xf7, 0x16, 0xe1
};

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  Serial.println("test: clear");
  pinMode(ledPin, OUTPUT);
  conntectToWifi();
}

void loop() {
 // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.println("[HTTPS] begin...");
    String url = getUrl();
    https.begin(*client, url.c_str());
    https.addHeader("Host", THINGSPEAK_URL);
    https.addHeader("content-type", "application/x-www-form-urlencoded");
    String key = getKey();
    int httpCode = https.sendRequest("DELETE", key.c_str());
    Serial.print("http code: ");
    Serial.println(httpCode);
    Serial.print("status: ");
    if (httpCode>0){
      Serial.println("success");
    } else {
      Serial.print("error ");
      Serial.println(https.errorToString(httpCode).c_str());
    }
    https.end();
  }
  delay(DELAY_LOOP);
}

String getUrl(){
  String url = String("https://api.thingspeak.com/channels/");
  url.concat(myChannelNumber);
  url.concat("/feeds.json");
  return url;
}

String getKey(){
  String key = String("api_key=");
  key.concat(myUserAPIKey);
  return key;
}

void conntectToWifi(){
  // Connect or reconnect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  Serial.print("Connecting to SSID: ");
  Serial.println(SECRET_SSID);
  blinker.attach_ms(INTERVAL_BLINK, changeState);
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
