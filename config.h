#include "secrets.h"

// Change these parameters
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
  #define BUTTON_A  2
  #define ANALOG_PIN_NO A0      // analog pin number
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  #define BUTTON_A 15
  #define ANALOG_PIN_NO A2     // analog pin number
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

// Pulled from secrets.h
const char ssid[] = SECRET_SSID; // your network SSID (name)
const char pass[] = SECRET_PASS; // your network password
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;
const char * myUserAPIKey = SECRET_USER_APIKEY;

unsigned int localPort = 8888;  // local port to listen for UDP packets
const int ledPin = LED_BUILTIN;

#define DELAY_WIFI 5            // delay between samples (s)
#define DELAY_SCREEN1 2         // delay after screen 1 (s)
#define DELAY_SCREEN2 3         // delay after screen 2 (s)
#define INTERVAL_BLINK 100      // blink interval (ms)
#define SYNC_INTERVAL 300
