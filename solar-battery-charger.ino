/*--------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a li-ion battery and
                upload the value to ThingSpeak.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         01FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "secrets.h"
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

// Change these parameters
#define ANALOG_PIN_NO A0        // analog pin number
#define BAUD_RATE 115200        // baud rate used for Serial console
#define R1 1000000              // resistor 1 on the voltage divider (Ω)
#define R2 220000               // resistor 2 on the voltage divider (Ω)
#define FIELD_NO_PERCENTAGE 1   // field number of battery percentage
#define FIELD_NO_LEVEL 2        // field number of battery level
#define FIELD_NO_VOLTAGE 3      // field number of battery voltage
#define SLEEP_TIME 1           // the time the Feather goes into a deep sleep (m)
#define VOLTAGE_MAX 4.2         // max voltage of lipo battery (V)
#define VOLTAGE_MIN 2.64        // min voltage of lipo battery (V)

// Don't have to change these
#define DELAY_SAMPLE 10         // delay between samples (ms)
#define DELAY_WIFI 5            // delay between samples (s)
#define DELAY_SCREEN1 2         // delay after screen 1 (s)
#define DELAY_SCREEN2 3         // delay after screen 2 (s)
#define NUM_SAMPLES 10          // number of analog samples to take per reading
#define INTERVAL_BLINK 100      // blink interval (ms)


#define OLED_DISPLAYOFF 0xAE    // turn OLED off
#define OLED_DISPLAYONN 0xAF

WiFiClient client;
Ticker blinker;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// Pulled from secrets.h
const char ssid[] = SECRET_SSID; // your network SSID (name)
const char pass[] = SECRET_PASS; // your network password
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

const int ledPin = LED_BUILTIN;

int battery_min = 0;            // min battery level
int battery_max = 0;            // max battery level

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();

  // calculate the voltage divider ratio
  float resistor_ratio=(float)R2/((float)R1+(float)R2);

  // calculate the min battery number using the resistor ratio
  battery_min=(float)resistor_ratio*(float)VOLTAGE_MIN*1024;
  // battery_min=580;

  // calculate the max battery number using the resistor ratio
  battery_max=(float)resistor_ratio*(float)VOLTAGE_MAX*1024;
  // battery_max=774;

  ThingSpeak.begin(client);
  pinMode(ledPin, OUTPUT);
  setupDisplay();
  conntectToWifi();
  delay(DELAY_SCREEN1*1e3);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  Serial.println("Battery:");
  display.println("Battery:");
  int level = getBatteryLevel();

  int percentage = getBatteryPercentage(level);

  float voltage = getBatteryVoltage(level);

  writeToThingSpeak(percentage, level, voltage);
  goToSleep();
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
  Serial.print("SSID: ");
  display.print("SSID: ");
  Serial.println(SECRET_SSID);
  display.println(SECRET_SSID);
  Serial.print("Connecting");
  display.print("Connecting");
  display.display();
  blinker.attach_ms(INTERVAL_BLINK, changeState);
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

int getSum(){
  // take a number of analog samples and add them up
  int sum;
  unsigned char sample_count;
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(ANALOG_PIN_NO);
    sample_count++;
    delay(DELAY_SAMPLE);
  }
  return sum;
}

int getBatteryLevel(){
  // calculate the average level
  int sum = getSum();
  int level = (float)sum / (float)NUM_SAMPLES;
  Serial.print(" Level: ");
  display.print(" Level: ");
  Serial.println(level);
  display.println(level);
  display.display();
  return level;
}

int getBatteryPercentage(int level){
  // convert battery level to percent
  int percentage = map(level, battery_min, battery_max, 0, 100);
  Serial.print(" Percentage: ");
  display.print(" Percentage: ");
  Serial.print(percentage);
  display.print(percentage);
  Serial.println("%");
  display.println("%");
  display.display();
  return percentage;
}

float getBatteryVoltage(int level){
  // convert battery level to voltage
  float voltage = (float)map(level, battery_min, battery_max, VOLTAGE_MIN*100, VOLTAGE_MAX*100)/100;
  Serial.print(" Voltage: ");
  display.print(" Voltage: ");
  Serial.print(voltage);
  display.print(voltage);
  Serial.println("V");
  display.println("V");
  display.display();
  return voltage;
}

void writeToThingSpeak(int percentage, int level, float voltage){
  ThingSpeak.setField(FIELD_NO_PERCENTAGE, percentage);
  ThingSpeak.setField(FIELD_NO_LEVEL, level);
  ThingSpeak.setField(FIELD_NO_VOLTAGE, voltage);

  Serial.println("Channel: ");
  display.println("Channel: ");
  Serial.print(" Number: ");
  display.print(" Number: ");
  Serial.println(myChannelNumber);
  display.println(myChannelNumber);
  Serial.print(" Status: ");
  display.print(" Status: ");
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200){
    Serial.println("success");
    display.println("success");
  } else {
    Serial.println("error " +String(x));
    display.println("error " + String(x));
  }
  display.display();
}

void goToSleep(){
  Serial.print("Sleep time: ");
  display.print("Sleep time: ");
  Serial.print(SLEEP_TIME);
  display.print(SLEEP_TIME);
  Serial.println("m");
  display.println("m");
  display.display();
  delay(DELAY_SCREEN2 * 1e3);
  display.oled_command(OLED_DISPLAYOFF);
  ESP.deepSleep(SLEEP_TIME * 60 * 1e6);
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}
