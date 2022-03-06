/*--------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a li-ion battery and
                upload the value to ThingSpeak.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0 or Adafruit Huzzah32
                with 3.7V lipo battery only.

  Software:     Developed using arduino-cli 0.21.1.

  Date:         06MAR2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif
#include <TimeLib.h>
#include <Timezone.h>           // https://github.com/JChristensen/Timezone
#include <Ticker.h>
#include "secrets.h"
#include "config.h"
#include "src/Battery.h"
#include "src/ClearChannel.h"
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

WiFiClient client;
Ticker blinker;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Battery bat;
ClearChannel cc;

time_t getNtpTime();

void setup() {
  bat.begin(ANALOG_PIN_NO, VOLTAGE_MIN, VOLTAGE_MAX, R1, R2);
  Serial.begin(BAUD_RATE);
  while(!Serial);
  pinMode(BUTTON_A, INPUT_PULLUP);
  Serial.println();
  pinMode(ledPin, OUTPUT);
  setupDisplay();
  display.clearDisplay();
  display.setCursor(0,0);
  print("Choose mode");
  delay(1000);
  print(".");
  delay(1000);
  print(".");
  delay(1000);
  println(".");
  doDischarge = !digitalRead(BUTTON_A);
  if(!doDischarge){
    println("Mode: recharge");
    conntectToWifi();
    ThingSpeak.begin(client);
    cc.begin(localPort, client, timeZone);
    setSyncProvider(getNtpTime);
    setSyncInterval(SYNC_INTERVAL);
  } else {
    println("Mode: discharge");
  }
  delay(DELAY_SCREEN1*1e3);
  display.clearDisplay();
  display.setCursor(0,0);
}

void loop() {
  String date = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);

  if (doClear && cc.shouldClearChannel(date)) cc.clearChannel(myUserAPIKey);

  int level = bat.level();

  int percentage = bat.percentage();

  float voltage = bat.voltage();

  updateDisplay(level, percentage, voltage);
  if(!doDischarge){
    writeToThingSpeak(percentage, level, voltage);
    goToSleep();
  }
  delay(1000);
}

/* -------------------------- */

void updateDisplay(int level, int percentage, float voltage){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Battery:");
  Serial.println("Battery:");
  display.println(String(" Level: ") + String(level));
  Serial.println(String(" Level: ") + String(level));
  display.println(String(" Percentage: ") + String(percentage) + "%");
  Serial.println(String(" Percentage: ") + String(percentage) + "%");
  display.println(String(" Voltage: ") + String(voltage) + "V");
  Serial.println(String(" Voltage: ") + String(voltage) + "V");
  display.display();
}

void setupDisplay(){
  display.begin(0x3C, true); // Address 0x3C default

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
  print("SSID: ");
  println(SECRET_SSID);
  print("Connecting");
  display.display();
  blinker.attach_ms(INTERVAL_BLINK, changeState);
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    print(".");
    delay(DELAY_WIFI*1e3);
  }
  blinker.detach();
  println();
  println("Connected!");
  print("IP: ");
  println(WiFi.localIP().toString().c_str());
  print("Hostname: ");
  #if defined(ESP8266)
    println(WiFi.hostname().c_str());
  #elif defined(ESP32)
    println(WiFi.getHostname());
  #endif
}

void writeToThingSpeak(int percentage, int level, float voltage){
  ThingSpeak.setField(FIELD_NO_PERCENTAGE, percentage);
  ThingSpeak.setField(FIELD_NO_LEVEL, level);
  ThingSpeak.setField(FIELD_NO_VOLTAGE, voltage);
  println("Channel: ");
  print(" Number: ");
  println(String(myChannelNumber).c_str());
  print(" Status: ");
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200){
    println("success");
  } else {
    print("error ");
    println(String(x).c_str());
  }
  display.display();
}

void goToSleep(){
  print("Sleep time: ");
  print(String(SLEEP_TIME).c_str());
  println("m");
  delay(DELAY_SCREEN2 * 1e3);
  display.oled_command(SH110X_DISPLAYOFF);
  ESP.deepSleep(SLEEP_TIME * 60 * 1e6);
}

void print(const char* value){
  Serial.print(value);
  display.print(value);
  display.display();
}

void println(){
  Serial.println();
  display.println();
  display.display();
}

void println(const char* value){
  Serial.println(value);
  display.println(value);
  display.display();
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}

time_t getNtpTime(){
  return cc.getNtpTime();
}
