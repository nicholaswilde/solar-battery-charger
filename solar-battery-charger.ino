/*--------------------------------------------------------------
  Program:      solar-battery-charger

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a li-ion battery and
                upload the value to ThingSpeak.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0 or Adafruit Huzzah32
                with 3.7V lipo battery only.

  Software:     Developed using arduino-cli 0.21.0.

  Date:         26FEB2022

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
#include <WiFiUdp.h>
#include <Timezone.h>           // https://github.com/JChristensen/Timezone
#include <Ticker.h>
#include "secrets.h"
#include "src/Battery.h"
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

// Change these parameters
#if defined(ESP8266)
  #define ANALOG_PIN_NO A0      // analog pin number
#elif defined(ESP32)
  #define ANALOG_PIN_NO A13     // analog pin number
#endif
#define BAUD_RATE 115200        // baud rate used for Serial console
#define R1 1000000              // resistor 1 on the voltage divider (Ω)
#define R2 220000               // resistor 2 on the voltage divider (Ω)
#define FIELD_NO_PERCENTAGE 1   // field number of battery percentage
#define FIELD_NO_LEVEL 2        // field number of battery level
#define FIELD_NO_VOLTAGE 3      // field number of battery voltage
#define SLEEP_TIME 15           // the time the Feather goes into a deep sleep (m)
#define VOLTAGE_MAX 4.2         // max voltage of lipo battery (V)
#define VOLTAGE_MIN 2.64        // min voltage of lipo battery (V)

#if defined(ESP8266)
  #define BUTTON_A  0
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  #define BUTTON_A 15
#endif

// clear the channel if it's a new day.
bool doClear = false;
bool doDischarge = false;

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

//const int timeZone = 0;   // UTC
//const int timeZone = 1;   // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)
const int timeZone = -8;  // Pacific Standard Time (USA)

/* -------------------------------------------------------------------------- */

// Don't have to change these
#define DELAY_WIFI 5            // delay between samples (s)
#define DELAY_SCREEN1 2         // delay after screen 1 (s)
#define DELAY_SCREEN2 3         // delay after screen 2 (s)
#define INTERVAL_BLINK 100      // blink interval (ms)
#define SYNC_INTERVAL 300

// Pulled from secrets.h
const char ssid[] = SECRET_SSID; // your network SSID (name)
const char pass[] = SECRET_PASS; // your network password
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;
const char * myUserAPIKey = SECRET_USER_APIKEY;

unsigned int localPort = 8888;  // local port to listen for UDP packets
const int ledPin = LED_BUILTIN;

// calculate the voltage divider ratio
float resistor_ratio=(float)R2/((float)R1+(float)R2);

// calculate the min battery number using the resistor ratio
int battery_min=(float)resistor_ratio*(float)VOLTAGE_MIN*1024;
//int battery_min=580;

// calculate the max battery number using the resistor ratio
int battery_max=(float)resistor_ratio*(float)VOLTAGE_MAX*1024;
//int battery_max=774;

const int dst = timeZone+1;

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule myDST = {"PDT", Second, Sun, Mar, 2, 60*dst};      // Daylight time = UTC - 7 hours
TimeChangeRule mySTD = {"PST", First, Sun, Nov, 2, 60*timeZone};  // Standard time = UTC - 8 hours
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev

WiFiClient client;
Ticker blinker;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Battery bat;

WiFiUDP Udp;
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

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
    Udp.begin(localPort);
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
  if (doClear && shouldClearChannel()) clearChannel();

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

// Get the current date
String getCurrentDate(){
  char timeToDisplay[20];
  sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
  return extractDate(String(timeToDisplay));
}

// Format: 2017-01-12
String getCreatedAt(){
  String date = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);
  date = adjustDate(date);
  return extractDate(date);
}

// Adjust date and time using Timezone
String adjustDate(String date){
  // https://stackoverflow.com/a/11213640/1061279
  struct tm tm;
  strptime(date.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm);
  time_t t = mktime(&tm);
  time_t local = myTZ.toLocal(t, &tcr);
  char timeToDisplay[20];
  sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(local), month(local), day(local), hour(local), minute(local), second(local));
  Serial.println(timeToDisplay);
  return String(timeToDisplay);
}

// Extract date from a date and itme format
String extractTime(String val){
  int index = val.indexOf("T");
  String time = val.substring(1, index);
  return time;
}

// Extract date from a date and itme format
String extractDate(String val){
  int index = val.indexOf("T");
  String date = val.substring(0, index);
  return date;
}

bool shouldClearChannel(){
  Serial.println("Dates:");
  String currentDate = getCurrentDate();
  Serial.print(" Current: ");
  Serial.println(currentDate);

  String createdAt = getCreatedAt();
  Serial.print(" Created at: ");
  Serial.println(createdAt);

  if (strcmp(createdAt.c_str(), currentDate.c_str()) == 0) {
    Serial.println(" Same date. Aborting");
    return false;
  } else if (createdAt == "") {
    Serial.println(" Already cleared. Aborting");
    return false;
  }
  return true;
}

void clearChannel(){

  HTTPClient https;

  Serial.println("[HTTPS] begin...");
  String url = getUrl();
  https.begin(client, url.c_str());
  https.addHeader("Host", THINGSPEAK_URL);
  https.addHeader("content-type", "application/x-www-form-urlencoded");
  String key = getKey();
  int httpCode = https.sendRequest("DELETE", key.c_str());
  Serial.print("http code: ");
  Serial.println(httpCode);
  Serial.print("status: ");
  if (httpCode>0){
    Serial.println("success");
  } else {
    Serial.print("error ");
    Serial.println(https.errorToString(httpCode).c_str());
  }
  https.end();
  delay(15000);
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

String getUrl(){
  String url = String("http://api.thingspeak.com/channels/");
  url.concat(myChannelNumber);
  url.concat("/feeds.json");
  return url;
}

String getKey(){
  String key = String("api_key=");
  key.concat(myUserAPIKey);
  return key;
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

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime(){
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmitting NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receiving NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
