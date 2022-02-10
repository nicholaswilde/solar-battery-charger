/*--------------------------------------------------------------
  Program:      oled

  Description:  Test the Adafruit 128x64 OLED FeatherWing
                display

  Hardware:     Adafruit Feather Huzzah with Adafruit 128x64
                OLED FeatherWing

  Software:     Developed using arduino-cli 0.20.2.

  Date:         07FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "secrets.h"

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)

// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
  #define BUTTON_A  0
  #define BUTTON_B 16
  #define BUTTON_C  2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
#elif defined(TEENSYDUINO)
  #define BUTTON_A  4
  #define BUTTON_B  3
  #define BUTTON_C  8
#elif defined(ARDUINO_NRF52832_FEATHER)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840, esp32-s2 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5
#endif

const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password
const int ledPin = LED_BUILTIN;

const char * myHostName = SECRET_HOSTNAME;

WiFiClient  client;
Ticker blinker;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  delay(100);
  Serial.println();
  pinMode(ledPin, OUTPUT);
  setupDisplay();
  Serial.println("test: oled");
  display.println("test: oled");
  conntectToWifi();
  display.display();
}

void loop() {
  delay(10);
  yield();
  display.display();
}

void print(char* value){
  Serial.print(value);
  display.print(value);
}

void println(char* value){
  Serial.println(value);
  display.print(value);
}

void setupDisplay(){
  display.begin(0x3C, true); // Address 0x3C default

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
}

void conntectToWifi(){
  // Connect or reconnect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  Serial.print("SSID: ");
  display.print("SSID: ");
  Serial.println(SECRET_SSID);
  display.println(SECRET_SSID);
  Serial.print("Connecting");
  display.print("Connecting");
  display.display();
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    Serial.print(".");
    display.print(".");
    display.display();
    delay(DELAY_WIFI*1e3);
  }
  blinker.detach();
  Serial.println();
  display.println();
  Serial.println("Connected!");
  display.println("Connected!");
  display.display();
  Serial.print("IP: ");
  display.print("IP: ");
  Serial.println(WiFi.localIP());
  display.println(WiFi.localIP());
  Serial.print("Hostname: ");
  display.print("Hostname: ");
  Serial.println(WiFi.hostname());
  display.println(WiFi.hostname());
  display.display();
}
