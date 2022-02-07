/*--------------------------------------------------------------
  Program:      wifi

  Description:  A quick wifi test that pings a website.

  Hardware:     Adafruit Feather Huzzah and wifi AP

  Software:     Developed using arduino-cli 0.20.2.

  Date:         24JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include "secrets.h"

// values are specified in secrets.h
// const char* ssid     = "yourssid";
// const char* password = "yourpassword";

const char ssid[] = SECRET_SSID; // your network SSID (name)
const char pass[] = SECRET_PASS; // your network password

const char* host = "wifitest.adafruit.com";

int value = 0;

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
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
