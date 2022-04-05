/*-----------------------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads the voltage, current, and power of a li-ion battery and
                and upload the values to ThingSpeak.

  Hardware:     Adafruit Feather HUZZAH32 with 3.7V lipo battery, Adafruit
                Powerboost 1000 Basic, Adafruit bq24074, and Adafruit INA260.

  Software:     Developed using arduino-cli 0.21.1.

  Date:         28MAR2022

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
#include <ESP_Google_Sheet_Client.h>
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
bool doWake;
bool doDischarge;
time_t getNtpTime();
touch_pad_t touchPin;

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  setupPins();
  setupDisplay();
  setupIna260();
  doWake = (esp_sleep_get_wakeup_cause()==ESP_SLEEP_WAKEUP_EXT0);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32,LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // toggle Powerboost
  if (checkButton()) digitalWrite(BUTTON_POWERBOOST, !digitalRead(BUTTON_POWERBOOST));

  if (checkButtonB()) goToSleep();

  if (currentMillis - previousMillis < interval*1e3) return;

  previousMillis = currentMillis;

  display.clearDisplay();
  display.setCursor(0,0);

  int current = ina260.readCurrent();

  // Determine the mode by sign of current
  if (current<0){
    if (abs(current)<DISPLAY_THRESHOLD) display.oled_command(SH110X_DISPLAYOFF);
    print("Mode: ");
    println("recharge");
    doDischarge = false;
  } else {
    print("Mode: ");
    println("discharge");
    doDischarge = true;
  }
  current = abs(current);

  int voltage = ina260.readBusVoltage();
  int percentage = getPercentage(voltage);
  int power = ina260.readPower();

  if(!doDischarge && !doWake)executeRecharge(percentage, voltage, current, power);

  updateDisplay(percentage, voltage, current, power);
}

/* --------------------------------- Setup --------------------------------- */

void setupPins(){
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
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
  //GSheet.setTokenCallback(tokenStatusCallback);

  GSheet.begin(myClientEmail, myProjectId, myPrivateKey);

  String date = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);
  if (doClear && (timeStatus() != timeNotSet) && cc.shouldClearChannel(date)) cc.clearChannel(myUserAPIKey);
  display.clearDisplay();
  display.setCursor(0,0);
  delay(DELAY_SCREEN1*1e3);
  updateDisplay(percentage, voltage, current, power);
  writeToThingSpeak(percentage, voltage, current, power);
  if (doSheets) writeToSheets(percentage, voltage, current, power);
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

void writeToSheets(int percentage, int voltage, int current, int power){
  Serial.println("Sheet");
  bool ready = GSheet.ready();
  FirebaseJson response;
  char timeToDisplay[30];
  sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
  FirebaseJson valueRange;

  valueRange.add("majorDimension", "COLUMNS");
  valueRange.set("values/[0]/[0]", timeToDisplay);
  valueRange.set("values/[1]/[0]", percentage);
  valueRange.set("values/[2]/[0]", voltage);
  valueRange.set("values/[3]/[0]", current);
  valueRange.set("values/[4]/[0]", power);
  Serial.print(" Sheet: ");
  Serial.println(mySpreadsheetId);
  Serial.print(" Status: ");
  bool success = GSheet.values.append(&response, mySpreadsheetId, myRange, &valueRange);
  if (success) {
    Serial.println("success");
  } else {
    Serial.println("error");
    response.toString(Serial, true);
    Serial.println();
  }
}

void tokenStatusCallback(TokenInfo info){
  if (info.status == esp_signer_token_status_error){
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else{
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
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

int getWakeupPin(){
  switch(esp_sleep_get_touchpad_wakeup_status()){
    case 0  : return 4;
    case 1  : return 0;
    case 2  : return 2;
    case 3  : return 15;
    case 4  : return 13;
    case 5  : return 12;
    case 6  : return 14;
    case 7  : return 27;
    case 8  : return 33;
    case 9  : return 32;
    default : return -1;
  }
}

/* ------------------------------- Functions ------------------------------- */

int getPercentage(int voltage){
  if (voltage <= VOLTAGE_MIN) {
    return 0;
	} else if (voltage >= VOLTAGE_MAX) {
		return 100;
  }
  return (float)EXP_B * exp((float)EXP_A*voltage)*100;
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

bool checkButtonB() {
  static bool oldButton;
  bool but = !digitalRead(BUTTON_B);
  delay(40);  // simple debounce button
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
