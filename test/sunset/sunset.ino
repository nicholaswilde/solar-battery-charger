/*--------------------------------------------------------------
  Program:      sunset

  Description:  Get the sunset time from a website.

  Hardware:     Adafruit Feather Huzzah.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         03FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"

// values are specified in secrets.h
const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password

const char* host = "http://api.sunrise-sunset.org/json?lat=33.6053681&lng=-117.6552182&date=today";

int value = 0;

const char* sensorReadingsArr[3];

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  HTTPClient http;
  http.begin(client, host);

  http.addHeader("Content-Type","application/json");
  int httpResponseCode = http.GET();

   String payload = "{}";

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    JSONVar myObject = JSON.parse(payload);
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    Serial.print("JSON object = ");
    Serial.println(myObject["results"]["sunrise"]);
    //Serial.println(myObject2);
    JSONVar keys = myObject["results"].keys();
    for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject["results"][keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        sensorReadingsArr[i] = (const char*)myObject["results"][keys[i]];
      }
      Serial.print("1 = ");
      Serial.println(sensorReadingsArr[0]);
      Serial.print("2 = ");
      Serial.println(sensorReadingsArr[1]);
      Serial.print("3 = ");
      Serial.println(sensorReadingsArr[2]);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  Serial.println();
  Serial.println("closing connection");
  delay(500);
}
