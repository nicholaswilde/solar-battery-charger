/*--------------------------------------------------------------
  Program:      sleep

  Description:  Put the Adafruit Feather Huzzah to a deep sleep.

  Hardware:     Adafruit Feather Huzzah with pin 16 connected
                to RST.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         24JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#define BAUD_RATE 9600 // baud rate used for Serial console
#define SLEEP_TIME 10    // sleep time (s)
#define DELAY_LOOP 5000  // loop delay time (ms)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.println("waking up");
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(DELAY_LOOP);
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY_LOOP);
  Serial.println("going to sleep");
  ESP.deepSleep(SLEEP_TIME * 1000000);
}
