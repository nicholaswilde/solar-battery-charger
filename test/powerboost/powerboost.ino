/*--------------------------------------------------------------
  Program:      powerboost

  Description:  Turn on and off an Adafruit Powerboost using a
                Feather.

  Hardware:     Adafruit Feather Huzzah32 with the Bat pin
                connected to the Bat pin of the Adafruit
                Powerboost 1000 Basic and pin 14 of the Feather
                connected to the EN pin of the Powerboost. All
                GNDs are connected together and the a 10kΩ
                resistor between the Powerboost EN pin and GND.

  Software:     Developed using arduino-cli 0.21.1.

  Date:         07MAR2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define PIN_OUTPUT 14

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  Serial.println("test: powerboost");
  pinMode(PIN_OUTPUT, OUTPUT);
}

void loop() {
  changeState();
  int val=digitalRead(PIN_OUTPUT);
  Serial.println(val);
  delay(DELAY_LOOP);
}

void changeState(){
  digitalWrite(PIN_OUTPUT, !(digitalRead(PIN_OUTPUT)));
}
