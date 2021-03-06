/*--------------------------------------------------------------
  Program:      thingspeak

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a lipo battery and
                upload the value to ThingSpeak.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         27JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define NUM_SAMPLES 10   // number of analog samples to take per reading
#define BAUD_RATE 115200 // baud rate used for Serial console
#define ANALOG_PIN_NO A0 // analog pin number
#define R1 1000000       // resistor 1 on the voltage divider
#define R2 220000        // resistor 2 on the voltage divider
#define VOLTAGE_MAX 4.2  // max voltage of lipo battery
#define VOLTAGE_MIN 3.14 // min voltage of lipo battery
#define DELAY_LOOP 20000  // delay for loop routine
#define DELAY_SAMPLE 10  // delay between samples

WiFiClient  client;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
int level = 0;                  // calculated battery level
int battery_min = 0;            // min battery level
int battery_max = 0;            // max battery level
float resistor_ratio = 0;       // resistor ratio

void setup() {
  Serial.begin(BAUD_RATE);

  // calculate the voltage divider ratio
  resistor_ratio=(float)R2/((float)R1+(float)R2);

  // calculate the min battery number using the resistor ratio
  battery_min=(float)resistor_ratio*(float)VOLTAGE_MIN*1024;
  // battery_min=580;
  // calculate the max battery number using the resistor ratio
  battery_max=(float)resistor_ratio*(float)VOLTAGE_MAX*1024;
  // battery_max=774;

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(ANALOG_PIN_NO);
    sample_count++;
    delay(DELAY_SAMPLE);
  }
  digitalWrite(LED_BUILTIN, LOW);
  level = (float)sum / (float)NUM_SAMPLES;
  // convert battery level to percent
  level = map(level, battery_min, battery_max, 0, 100);
  Serial.print("Battery level: ");
  Serial.print(level);
  Serial.println("%");
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, level, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DELAY_LOOP);
}
