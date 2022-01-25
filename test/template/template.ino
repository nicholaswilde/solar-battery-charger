/*--------------------------------------------------------------
  Program:      template

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a lipo battery.
  
  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.
                
  Software:     Developed using arduino-cli 0.20.2.

  Date:         24JAN2022
 
  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#define BAUD_RATE 115200 // baud rate used for Serial console
#define DELAY_LOOP 1000  // loop delay time (ms)

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  Serial.println("test");
  delay(DELAY_LOOP);
}