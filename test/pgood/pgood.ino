/*--------------------------------------------------------------
  Program:      pgood

  Description:  Reads the state of the PGOOD pin on the bq24074.

  Hardware:     Adafruit Feather Huzzah plugged into lipo port
                on Adafruit bq24074, pin 14 on Feather plugged
                into PGOOD pin of bq24074.

  Software:     Developed using arduino-cli 0.21.0.

  Date:         13FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <Ticker.h>

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 2000         // loop delay time (ms)
#define INTERVAL_BLINK 100      // blink interval (ms)

Ticker blinker;

const int inputPin = 14;

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  Serial.println("test: pgood");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inputPin, INPUT);
}

void loop() {
  int val = !digitalRead(inputPin);
  Serial.println("Digital Read:");
  Serial.print(" Pin: ");
  Serial.println(inputPin);
  Serial.print(" Value: ");
  Serial.println(val);
  if(val) {
    blinker.attach_ms(INTERVAL_BLINK, changeState);
  }else{
    blinker.detach();
  }
  delay(DELAY_LOOP);
}

void changeState(){
  digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
}
