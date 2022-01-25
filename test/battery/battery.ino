/*--------------------------------------------------------------
  Program:      battery

  Description:  Reads value on analog input A0 and calculates
                the the percent level of a lipo battery.
  
  Hardware:     Adafruit Feather Huzzah with voltage divider on
                A0.
                
  Software:     Developed using Arduino 1.0.5 software
                Should be compatible with Arduino 1.0 +

  Date:         24JAN2022
 
  Author:       Nicholas Wilde
--------------------------------------------------------------*/

// number of analog samples to take per reading
#define NUM_SAMPLES 10
#define BAUD_RATE 115200
#define ANALOG_PIN_NO A0
#define R1 1000000
#define R2 220000
#define VOLTAGE_MAX 4.2
#define VOLTAGE_MIN 3.14
#define DELAY_LOOP 1000
#define DELAY_SAMPLE 10

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
int level = 0;                  // calculated battery level
int battery_min = 0;            // min battery level
int battery_max = 0;            // max battery level
float resistor_ratio = 0;       // resistor ratio

void setup() {
  Serial.begin(BAUD_RATE);
  resistor_ratio=(float)R2/((float)R1+(float)R2);
  battery_min=(float)resistor_ratio*(float)VOLTAGE_MIN*1024;
  // battery_min=580;
  battery_max=(float)resistor_ratio*(float)VOLTAGE_MAX*1024;
  // battery_max=774;
}

void loop() {
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(ANALOG_PIN_NO);
    sample_count++;
    delay(DELAY_SAMPLE);
  }
  // https://learn.adafruit.com/using-ifttt-with-adafruit-io/arduino-code-2
  // read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // our 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).
  level = (float)sum / (float)NUM_SAMPLES;
  // convert battery level to percent
  level = map(level, battery_min, battery_max, 0, 100);
  Serial.print("Battery level: ");
  Serial.print(level);
  Serial.println("%");
  delay(DELAY_LOOP);
}
