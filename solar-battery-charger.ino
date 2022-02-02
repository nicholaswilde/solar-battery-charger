/*--------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a lipo battery and
                upload the value to ThingSpeak.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         01FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "secrets.h"
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

#define NUM_SAMPLES 10          // number of analog samples to take per reading
#define BAUD_RATE 115200        // baud rate used for Serial console
#define ANALOG_PIN_NO A0        // analog pin number
#define R1 1000000              // resistor 1 on the voltage divider
#define R2 220000               // resistor 2 on the voltage divider
#define VOLTAGE_MAX 4.2         // max voltage of lipo battery
#define VOLTAGE_MIN 2.88        // min voltage of lipo battery
#define DELAY_SAMPLE 10         // delay between samples
#define DELAY_WIFI 5            // delay between samples (s)
#define SLEEP_TIME 15           // sleep time (m)
#define FIELD_NO_PERCENTAGE 1   // field number of battery percentage
#define FIELD_NO_LEVEL 2        // field number of battery percentage
#define INTERVAL_BLINK 100      // blink interval (ms)

WiFiClient  client;
Ticker blinker;

const char ssid[] = SECRET_SSID;      // your network SSID (name)
const char pass[] = SECRET_PASS;      // your network password
const int ledPin = LED_BUILTIN;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
int level = 0;                  // calculated battery level
int percentage = 0;             // calculated battery percentage
int voltage = 0;                // calculated battery voltage
int battery_min = 0;            // min battery level
int battery_max = 0;            // max battery level
float resistor_ratio = 0;       // resistor ratio

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();

  // calculate the voltage divider ratio
  resistor_ratio=(float)R2/((float)R1+(float)R2);

  // calculate the min battery number using the resistor ratio
  battery_min=(float)resistor_ratio*(float)VOLTAGE_MIN*1024;
  // battery_min=580;

  // calculate the max battery number using the resistor ratio
  battery_max=(float)resistor_ratio*(float)VOLTAGE_MAX*1024;
  // battery_max=774;

  ThingSpeak.begin(client);
  pinMode(ledPin, OUTPUT);
  blinker.attach_ms(INTERVAL_BLINK, changeState);
  conntectToWifi();
}

void loop() {

  sum = getSum();

  level = getBatteryLevel(sum);

  percentage = getBatteryPercentage(level);

  voltage = getBatteryVoltage(level);

  writeToThingSpeak(percentage, level);

  goToSleep();
}



void conntectToWifi(){
  // Connect or reconnect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  Serial.print("Connecting to SSID: ");
  Serial.println(SECRET_SSID);
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

int getSum(){
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(ANALOG_PIN_NO);
    sample_count++;
    delay(DELAY_SAMPLE);
  }
  return sum;
}

int getBatteryLevel(int sum){
  // calculate the average level
  level = (float)sum / (float)NUM_SAMPLES;
  Serial.print("Battery level: ");
  Serial.println(level);
  return level;
}

int getBatteryPercentage(int level){
  // convert battery level to percent
  percentage = map(level, battery_min, battery_max, 0, 100);
  Serial.print("Battery percentage: ");
  Serial.print(percentage);
  Serial.println("%");
  return percentage;
}

int getBatteryVoltage(int level){
  // convert battery level to voltage
  voltage = map(level, battery_min, battery_max, VOLTAGE_MIN, VOLTAGE_MAX);
  Serial.print("Battery voltage: ");
  Serial.print(voltage);
  Serial.println("VDC");
  return voltage;
}

void writeToThingSpeak(int percentage, int level){
  ThingSpeak.setField(FIELD_NO_PERCENTAGE, percentage);
  ThingSpeak.setField(FIELD_NO_LEVEL, level);
  ThingSpeak.setStatus(String("Charging"));

  Serial.print("Channel number: ");
  Serial.println(myChannelNumber);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200){
    Serial.println("Channel update successful");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void goToSleep(){
  Serial.print("Going to sleep for ");
  Serial.print(SLEEP_TIME);
  Serial.println(" minutes");
  ESP.deepSleep(SLEEP_TIME * 60 * 1e6);
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}
