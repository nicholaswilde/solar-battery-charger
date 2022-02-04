/*--------------------------------------------------------------
  Program:      sunset

  Description:  Get the sunrise and sunset times from a website.

  Hardware:     Adafruit Feather Huzzah.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         03FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include "secrets.h"

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)

WiFiClient  client;
Ticker blinker;

const char * lat = "33.60";
const char * lng = "-117.68";

// values are specified in secrets.h
const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password

const int ledPin = LED_BUILTIN;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

char* host = "http://api.sunrise-sunset.org/json?&date=today&formatted=0";

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  delay(100);
  Serial.println();
  Serial.println("test: sunset");
  pinMode(ledPin, OUTPUT);
  conntectToWifi();
}

void loop() {
  String payload = httpGETRequest(host);
  String sunrise = jsonParse(payload, "sunrise");
  String sunset = jsonParse(payload, "sunset");
  Serial.print("Sunrise: ");
  Serial.println(sunrise);
  Serial.print("Sunset: ");
  Serial.println(sunset);
  delay(5000);
}

String jsonParse(String payload, const char* value){
  // Use arduinojson.org/v6/assistant to compute the capacity
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, payload);
  // Test if parsing succeeds.
  String sunset;
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  } else {
    sunset = (String)doc["results"][value];
  }
  return sunset;
}

String httpGETRequest(const char* host) {
  HTTPClient http;
  char buffer[100]; // long eough for the combined text
  strcpy(buffer, host);
  strcat(buffer, "&lat=");
  strcat(buffer, lat);
  strcat(buffer, "&lng=");
  strcat(buffer, lng);
  Serial.print("host: ");
  Serial.println(buffer);
  // Your IP address with path or Domain name with URL path 
  http.begin(client, buffer);
  http.addHeader("Content-Type","application/json");
  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
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
