
/*--------------------------------------------------------------
  Program:      date

  Description:  Connect to an NTP server and get the date.

  Hardware:     Adafruit Feather Huzzah.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         31JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "secrets.h"

#define BAUD_RATE 115200        // baud rate used for Serial console
#define INTERVAL_BLINK 100      // blink interval (ms)
#define DELAY_WIFI 5            // delay between samples (s)
#define SYNC_INTERVAL 300
#define SLEEP_TIME 1            // sleep time (m)

const char ssid[] = SECRET_SSID;  //  your network SSID (name)
const char pass[] = SECRET_PASS;  // your network password
const char * myHostName = SECRET_HOSTNAME;

unsigned int localPort = 8888;  // local port to listen for UDP packets
const int ledPin = LED_BUILTIN;

Ticker blinker;
WiFiUDP Udp;
time_t getNtpTime();

void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

//const int timeZone = 1;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial);
  delay(250);
  pinMode(ledPin, OUTPUT);
  WiFi.hostname(myHostName);
  Serial.println();
  Serial.println("test: date");
  conntectToWifi();
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("Waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(SYNC_INTERVAL);
}

void loop() {
  digitalWrite(ledPin, LOW);
  digitalClockDisplay();
  goToSleep();
}

void goToSleep(){
  Serial.print("Going to sleep for ");
  Serial.print(SLEEP_TIME);
  Serial.println(" minutes");
  ESP.deepSleep(SLEEP_TIME * 60 * 1e6);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print("date: ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void changeState(){
  digitalWrite(ledPin, !(digitalRead(ledPin)));
}

void conntectToWifi(){
  blinker.attach_ms(INTERVAL_BLINK, changeState);
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
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
