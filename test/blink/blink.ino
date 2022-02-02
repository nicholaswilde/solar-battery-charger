/*--------------------------------------------------------------
  Program:      blink

  Description:  Blink the built in LED without a delay.

  Hardware:     Adafruit Feather Huzzah

  Software:     Developed using arduino-cli ${VERSION}.

  Date:         02FEB2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

const int ledPin =  LED_BUILTIN;  // the number of the LED pin
int ledState = LOW;               // ledState used to set the LED
unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 100;        // interval at which to blink (milliseconds)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  blink();
}

void blink(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}
