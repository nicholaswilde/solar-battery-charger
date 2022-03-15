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
#include <Adafruit_INA260.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>           // https://github.com/JChristensen/Timezone
#include <Ticker.h>
#include "secrets.h"
#include "config.h"
#include "src/ClearChannel.h"
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

WiFiClient client;
Ticker blinker;
Adafruit_INA260 ina260 = Adafruit_INA260();
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
ClearChannel cc;

unsigned long previousMillis = 0;
time_t getNtpTime();

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  setupPins();
  setupDisplay();
  setupIna260();
  doDischarge = chooseMode();
  //doDischarge = true;
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

  unsigned long currentMillis = millis();

  if (checkButton()) digitalWrite(BUTTON_POWERBOOST, !digitalRead(BUTTON_POWERBOOST));

  if (currentMillis - previousMillis >= interval*1e3) {

    previousMillis = currentMillis;

    //String date = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);

    //if (doClear && cc.shouldClearChannel(date)) cc.clearChannel(myUserAPIKey);

    int voltage = ina260.readBusVoltage();

    int percentage;

    int current = ina260.readCurrent();

    int power = ina260.readPower();

    updateDisplay(percentage, voltage, current, power);

    if(!doDischarge){
      writeToThingSpeak(percentage, voltage, current, power);
      goToSleep();
    }
  }
}

/* -------------------------- */

bool chooseMode(){
  return (ina260.readCurrent()>=0);
}

String formatValue(int val){
  String buffer;
  if (val < 0) val = val*-1;
  if (val >= 1000){
    float volts = (float)val/1000;
    buffer = String(volts) + " ";
  } else {
    buffer = String(val) + " m";
  }
  return buffer;
}

void setupPins(){
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_POWERBOOST, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void setupIna260(){
  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");
  ina260.setAveragingCount(INA260_COUNT_16);
}

void updateDisplay(int percentage, int voltage, int current, int power){
  display.clearDisplay();
  display.setCursor(0,0);
  String temp;
  display.println("Battery:");
  Serial.println("Battery:");
  display.println(String(" Percentage: ") + String(percentage) + "%");
  Serial.println(String(" Percentage: ") + String(percentage) + "%");
  temp = formatValue(voltage);
  display.println(String(" Voltage: ") + temp + "V");
  Serial.println(String(" Voltage: ") + temp + "V");
  temp = formatValue(current);
  display.println(String(" Current: ") + temp + "A");
  Serial.println(String(" Current: ") + temp + "A");
  temp = formatValue(power);
  display.println(String(" Power: ") + temp + "W");
  Serial.println(String(" Power: ") + temp + "W");
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
  println(WiFi.getHostname());
}

bool checkButton() {
	static bool oldButton;
	bool but = !digitalRead(BUTTON_A);
	delay(40);	// simple debounce button
	if( but != oldButton){
		oldButton = but;
		return true && !oldButton;
	}
	return false;
}

void writeToThingSpeak(int percentage, int voltage, int current, int power){
  ThingSpeak.setField(FIELD_NO_PERCENTAGE, percentage);
  ThingSpeak.setField(FIELD_NO_VOLTAGE, voltage);
  ThingSpeak.setField(FIELD_NO_CURRENT, current);
  ThingSpeak.setField(FIELD_NO_POWER, power);
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
