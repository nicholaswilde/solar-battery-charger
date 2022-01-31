/*--------------------------------------------------------------
  Program:      debounce

  Description:  Read a pushbutton, filtering noise.

  Hardware:     Adafruit Feather Huzzah with pushbutton and pull
                up resistor.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         31JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#define BAUD_RATE 115200        // baud rate used for Serial console

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 0;    // the number of the pushbutton pin
const int ledPin = LED_BUILTIN;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState = 0;         // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(BAUD_RATE);
  while(! Serial);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);
}

void loop() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(ledPin, ledState);

  lastButtonState = reading;
}
