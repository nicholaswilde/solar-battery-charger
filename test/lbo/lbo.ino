/*--------------------------------------------------------------
  Program:      lbo

  Description:  Monitor the LBO pin on Adafruit Powerboost
                using a Feather.

  Hardware:     Adafruit Feather Huzzah32 with the Bat pin
                connected to the Bat pin of the Adafruit
                Powerboost 1000 Basic and pin 14 of the Feather
                connected to the EN pin of the Powerboost. All
                GNDs are connected together and a diode
                between the Powerboost LBO pin and the Feather pin 14.

  Software:     Developed using arduino-cli 0.21.1.

  Date:         08MAR2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#define BAUD_RATE 115200        // baud rate used for Serial console
#define DELAY_LOOP 1000         // loop delay time (ms)
#define PIN_INPUT 14

void setup() {
  Serial.begin(BAUD_RATE);
  while(!Serial);
  Serial.println();
  Serial.println("test: lbo");
  pinMode(PIN_INPUT, INPUT_PULLUP);
}

void loop() {
  int val=!digitalRead(PIN_INPUT);
  Serial.println(val);
  delay(DELAY_LOOP);
}
