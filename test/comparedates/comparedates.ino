/*--------------------------------------------------------------
  Program:      comparedates

  Description:  Compares the createdAt date from Thingspeak and
                the current date at UTC.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         01FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "secrets.h"
#include <stdio.h>
#include "ThingSpeak.h"         // always include thingspeak header file after other header files and custom macros

#define BAUD_RATE 115200        // baud rate used for Serial console
#define SLEEP_TIME 10           // loop delay time (ms)
#define ANALOG_PIN_NO A0        // analog pin number
#define DELAY_WIFI 5            // delay between samples (s)
#define INTERVAL_BLINK 100      // blink interval (ms)
#define SYNC_INTERVAL 300

WiFiClient client;
Ticker blinker;
WiFiUDP Udp;
time_t getNtpTime();

const char ssid[] = SECRET_SSID;  // your network SSID (name)
const char pass[] = SECRET_PASS;  // your network password
const int ledPin = LED_BUILTIN;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;

const int timeZone = 0;           // UTC
unsigned int localPort = 8888;    // local port to listen for UDP packets

void sendNTPpacket(IPAddress &address);

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  delay(250);
  Serial.println();
  Serial.println("test: comparedates");
  pinMode(ledPin, OUTPUT);
  ThingSpeak.begin(client);
  conntectToWifi();
  setupTime();
}

void loop() {
  String currentDate = getCurrentDate();
  Serial.println(currentDate);
  String createdAt = getCreatedAt();
  Serial.println(createdAt);

  if (strcmp(createdAt.c_str(), currentDate.c_str()) < 0) {
		Serial.println(createdAt + " is older than " + currentDate);
	} else if (strcmp(createdAt.c_str(), currentDate.c_str()) == 0) {
		Serial.println(createdAt + " is the same day as " + currentDate);
  } else {
		Serial.println(createdAt + " is newer than " + currentDate);
	}
  Serial.println("going to sleep");
  ESP.deepSleep(SLEEP_TIME * 1e6);
}

/*---------------------------------------------------------------------------*/

void setupTime(){
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("Waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(SYNC_INTERVAL);
}

void conntectToWifi(){
  // Connect or reconnect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myHostName);
  Serial.print("Connecting to SSID: ");
  Serial.println(SECRET_SSID);
  blinker.attach_ms(INTERVAL_BLINK, changeState);
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

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}

/*-------- Functions ----------*/

// Format: 2017-01-12 13:22:54
String getCreatedAt(){
  Serial.print("CreatedAt: ");
  String s = ThingSpeak.readCreatedAt(myChannelNumber, myWriteAPIKey);
  return getDate(s);
}

String getCurrentDate(){
  char timeToDisplay[20];
  sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
  Serial.print("CurrentDate: ");
  String s = convertToString(timeToDisplay,sizeof(timeToDisplay) / sizeof(char));
  return getDate(s);
}

// https://www.geeksforgeeks.org/convert-character-array-to-string-in-c/
String convertToString(char* a, int size){
  int i;
  String s = "";
  for (i = 0; i < size; i++) {
    s = s + a[i];
  }
  return s;
}

String getDate(String s){
  int i = s.indexOf("T");
  return s.substring(0, i);
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
