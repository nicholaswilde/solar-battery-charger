/*--------------------------------------------------------------
  Program:      blink
  
  Description:  Blink the built in LED.
  
  Hardware:     Adafruit Feather Huzzah
                
  Software:     Developed using arduino-cli 0.20.2.
  
  Date:         24JAN2022
 
  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
