#ifndef _ClearChannel_H_
#define _ClearChannel_H_

#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <HTTPClient.h>

#define THINGSPEAK_URL "api.thingspeak.com"

WiFiUDP Udp;
void sendNTPpacket(IPAddress &address);
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

class ClearChannel{
  private:
    int _timeZone;
    bool _debug=false;
    WiFiClient _client;

    String _getUrl(){
      String url = String("http://api.thingspeak.com/channels/");
      url.concat(myChannelNumber);
      url.concat("/feeds.json");
      return url;
    }

    String _getKey(const char* userAPIKey){
      String key = String("api_key=");
      key.concat(userAPIKey);
      return key;
    }

    // send an NTP request to the time server at the given address
    void _sendNTPpacket(IPAddress &address) {
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

    String _getCurrentDate(){
      char timeToDisplay[30];
      sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
      return _extractDate(String(timeToDisplay));
    }

    // Extract date from a date and itme format
    String _extractDate(String val){
      int index = val.indexOf("T");
      String date = val.substring(0, index);
      return date;
    }

    // Adjust date and time using Timezone
    String _adjustDate(String date){
      const int dst = _timeZone+1;
      // US Eastern Time Zone (New York, Detroit)
      TimeChangeRule myDST = {"PDT", Second, Sun, Mar, 2, 60*dst};      // Daylight time = UTC - 7 hours
      TimeChangeRule mySTD = {"PST", First, Sun, Nov, 2, 60*_timeZone};  // Standard time = UTC - 8 hours
      Timezone myTZ(myDST, mySTD);
      // https://stackoverflow.com/a/11213640/1061279
      struct tm tm;
      strptime(date.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm);
      time_t t = mktime(&tm);
      TimeChangeRule *tcr;
      time_t local = myTZ.toLocal(t, &tcr);
      char timeToDisplay[30];
      sprintf(timeToDisplay, "%02d-%02d-%02dT%02d:%02d:%02dZ", year(local), month(local), day(local), hour(local), minute(local), second(local));
      return String(timeToDisplay);
    }

  public:
    void begin(unsigned int localPort, WiFiClient client, const int timeZone){
      Udp.begin(localPort);
      _client=client;
      _timeZone=timeZone;
    }

    // Default constructor
	  ClearChannel(){};
    //~ClearChannel();
    time_t getNtpTime(){
      IPAddress ntpServerIP; // NTP server's ip address
      while (Udp.parsePacket() > 0) ; // discard any previously received packets
      Serial.println("Transmitting NTP Request");
      // get a random server from the pool
      WiFi.hostByName(ntpServerName, ntpServerIP);
      Serial.print(ntpServerName);
      Serial.print(": ");
      Serial.println(ntpServerIP);
      _sendNTPpacket(ntpServerIP);
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

    void clearChannel(const char* userAPIKey){
      HTTPClient https;
      Serial.println("[HTTPS] begin...");
      String url = _getUrl();
      https.begin(_client, url.c_str());
      https.addHeader("Host", THINGSPEAK_URL);
      https.addHeader("content-type", "application/x-www-form-urlencoded");
      String key = _getKey(userAPIKey);
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

    bool shouldClearChannel(String createdAt){
      Serial.println("Dates:");
      String currentDate = _getCurrentDate();
      Serial.print(" Current: ");
      Serial.println(currentDate);
      createdAt = _adjustDate(createdAt);
      createdAt = _extractDate(createdAt);
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
};
#endif // _ClearChannel_H_
