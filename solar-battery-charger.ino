/*--------------------------------------------------------------
  Program:      solar-battery-charger
  
  Description:  Reports the battery level of the charged li-ion
                battery and notifies when the battery is
                finished charging
  
  Hardware:     Adafruit Feather Huzzah, TP4056, 5V boost board,
                18650 li-ion batteries, 7.5V 0.5A solar panel
                
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
