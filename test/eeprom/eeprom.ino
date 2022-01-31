/*--------------------------------------------------------------
  Program:      eeprom

  Description:  Stores values read from analog input 0 into
                the EEPROM. These values will stay in the
                EEPROM when the board is turned off and may
                be retrieved later by another sketch.

  Hardware:     Adafruit Feather Huzzah

  Software:     Developed using arduino-cli 0.20.2.

  Date:         30JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <EEPROM.h>

#define BAUD_RATE 115200 // baud rate used for Serial console
#define DELAY_LOOP 1000  // loop delay time (ms)
#define SLEEP_TIME 10    // sleep time (s)
#define ANALOG_PIN_NO A0 // analog pin number

int addr = 0;
int val = 0;
byte value;

void setup() {
  Serial.begin(BAUD_RATE);
  EEPROM.begin(512);
  Serial.println();
  Serial.println("eeprom");
  value = EEPROM.read(addr);
  Serial.print("Retrieved value: ");
  Serial.println(value, DEC);
  Serial.print("Variable value: ");
  Serial.println(val);
}

void loop() {
  // divide by 4 becasue analog pin range is 0-1023 and byte is 0-255
  val = analogRead(ANALOG_PIN_NO) / 4;
  EEPROM.write(addr, val);
  Serial.print("Pin value: ");
  Serial.println(val);
  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }
  ESP.deepSleep(SLEEP_TIME * 1000000);
}
