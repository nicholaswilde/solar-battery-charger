/*-----------------------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads the voltage, current, and power of a li-ion battery and
                and upload the values to ThingSpeak.

  Hardware:     Adafruit Feather HUZZAH32 with 3.7V lipo battery, Adafruit
                Powerboost 1000 Basic, Adafruit bq24074, and Adafruit INA260.

  Software:     Developed using arduino-cli 0.21.1.

  Date:         16MAR2022

  Author:       Nicholas Wilde 0x08b7d7a3
-----------------------------------------------------------------------------*/

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

#define Threshold 40 /* Greater the value, more the sensitivity */
unsigned long previousMillis = 0;
time_t getNtpTime();
touch_pad_t touchPin;



void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  setupPins();
  setupDisplay();
  setupIna260();
  print_wakeup_reason();
  print_wakeup_touchpad();
  //Setup interrupt on Touch Pad 3 (GPIO15)
  touchAttachInterrupt(T3, callback, Threshold);
  // configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();
}

void loop() {
  unsigned long currentMillis = millis();

  // toggle Powerboost
  if (checkButton()) digitalWrite(BUTTON_POWERBOOST, !digitalRead(BUTTON_POWERBOOST));

  if (currentMillis - previousMillis >= interval*1e3) {
    previousMillis = currentMillis;

    display.clearDisplay();
    display.setCursor(0,0);

    int current = ina260.readCurrent();

    print("Mode: ");

    // Determine the mode by sign of current
    if (current<0){
      println("recharge");
      current = -1*current;
      doDischarge = false;
    } else {
      println("discharge");
      doDischarge = true;
    }

    int voltage = ina260.readBusVoltage();
    int percentage;
    int power = ina260.readPower();

    if(!doDischarge)executeRecharge(power, voltage, current, power);

    updateDisplay(percentage, voltage, current, power);
  }
}

/* --------------------------------- Setup --------------------------------- */

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

/* ------------------------------------------------------------------------- */

void executeRecharge(int percentage, int voltage, int current, int power){
  connectToWifi();
  ThingSpeak.begin(client);
  cc.begin(localPort, client, timeZone);
  setSyncProvider(getNtpTime);
  setSyncInterval(SYNC_INTERVAL);
  String date = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);
  if (doClear && cc.shouldClearChannel(date)) cc.clearChannel(myUserAPIKey);
  display.clearDisplay();
  display.setCursor(0,0);
  delay(DELAY_SCREEN1*1e3);
  updateDisplay(percentage, voltage, current, power);
  writeToThingSpeak(percentage, voltage, current, power);
  goToSleep();
}

void updateDisplay(int percentage, int voltage, int current, int power){
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

void connectToWifi(){
  // Connect or reconnect to WiFi
  int tries = 0;
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
    display.display();
    delay(DELAY_WIFI*1e3);
    tries++;
    if (tries >= 10) break;
  }
  // goto sleep if couldn't connect
  if (WiFi.status() != WL_CONNECTED){
    println();
    println("Could not connect");
    display.display();
    goToSleep();
  }
  blinker.detach();
  println();
  println("Connected!");
  print("IP: ");
  println(WiFi.localIP().toString().c_str());
  print("Hostname: ");
  println(WiFi.getHostname());
  display.display();
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
  display.display();
  delay(DELAY_SCREEN2 * 1e3);
  display.oled_command(SH110X_DISPLAYOFF);
  ESP.deepSleep(SLEEP_TIME * 60 * 1e6);
}

void callback(){
  //placeholder callback function
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default :
      Serial.print("Wakeup was not caused by deep sleep: ");
      Serial.println(wakeup_reason);
      break;
  }
}

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
void print_wakeup_touchpad(){
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin){
    case 0  : Serial.println("Touch detected on GPIO 4"); break;
    case 1  : Serial.println("Touch detected on GPIO 0"); break;
    case 2  : Serial.println("Touch detected on GPIO 2"); break;
    case 3  : Serial.println("Touch detected on GPIO 15"); break;
    case 4  : Serial.println("Touch detected on GPIO 13"); break;
    case 5  : Serial.println("Touch detected on GPIO 12"); break;
    case 6  : Serial.println("Touch detected on GPIO 14"); break;
    case 7  : Serial.println("Touch detected on GPIO 27"); break;
    case 8  : Serial.println("Touch detected on GPIO 33"); break;
    case 9  : Serial.println("Touch detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}

/* ------------------------------- Functions ------------------------------- */

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

time_t getNtpTime(){
  return cc.getNtpTime();
}

/* --------------------------------- Print --------------------------------- */

void print(const char* value){
  Serial.print(value);
  display.print(value);
}

void println(){
  Serial.println();
  display.println();
}

void println(const char* value){
  Serial.println(value);
  display.println(value);
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}
