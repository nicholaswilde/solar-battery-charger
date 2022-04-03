#include "secrets.h"

// Change these parameters
#define BAUD_RATE 115200        // baud rate used for Serial console

// ThingSpeak fields
#define FIELD_NO_PERCENTAGE 1   // field number of battery percentage
#define FIELD_NO_CURRENT 2      // field number of battery level
#define FIELD_NO_VOLTAGE 3      // field number of battery voltage
#define FIELD_NO_POWER 4        // field number of battery power

#define SLEEP_TIME 15           // the time the Feather goes into a deep sleep (m)
#define VOLTAGE_MAX 4072        // max voltage of lipo battery (V)
#define VOLTAGE_MIN 3000        // min voltage of lipo battery (V)
#define EXP_A 0.00348           // y = B * exp(A*x)
#define EXP_B 0.000000686       // y = B * exp(A*x)

// Pins
#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_POWERBOOST 33

const int batteryCapacity = 3000; // Battery capacity in mAh
const int batteryQuantity = 2;    // Quantity of batteries

// clear the channel if it's a new day.
bool doClear = false;

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

/*----------------------------------- Wifi --------------------------------*/

// Pulled from secrets.h
const char ssid[] = SECRET_SSID; // your network SSID (name)
const char pass[] = SECRET_PASS; // your network password

/*------------------------------- ThingSpeak ------------------------------*/

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const char * myHostName = SECRET_HOSTNAME;
const char * myUserAPIKey = SECRET_USER_APIKEY;

/*------------------------------ Google Sheets -----------------------------*/

bool doSheets = false;

const char * myRange = "Sheet1!A2";

const char * myProjectId = PROJECT_ID;

//Service Account's client email
const char * myClientEmail = CLIENT_EMAIL;

const char myPrivateKey[] PROGMEM = PRIVATE_KEY;

const char * mySpreadsheetId = SPREADSHEET_ID;

/*---------------------------------------------------------------------------*/

unsigned int localPort = 8888;  // local port to listen for UDP packets
const int ledPin = LED_BUILTIN;
const long interval = 1;        // interval for updating discharge loop

#define DELAY_WIFI 5            // delay between samples (s)
#define DELAY_SCREEN1 2         // delay after screen 1 (s)
#define DELAY_SCREEN2 3         // delay after screen 2 (s)
#define INTERVAL_BLINK 100      // blink interval (ms)
#define SYNC_INTERVAL 300
