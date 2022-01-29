/*--------------------------------------------------------------
  Program:      smoothed

  Description:  Smooth out sensor readings using the Smoothed
                library.

  Hardware:     Adafruit Feather Huzzah with 3.7V lipo battery
                and voltage divider on A0.

  Software:     Developed using arduino-cli 0.20.2.

  Date:         29JAN2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <Smoothed.h>	// Include the library

#define BAUD_RATE 115200 // baud rate used for Serial console
#define DELAY_LOOP 1000  // loop delay time (ms)
#define SENSOR_PIN A0

// Create two instances of the class to use. 
Smoothed <float> mySensor; 
Smoothed <float> mySensor2;

void setup() {
  Serial.begin(BAUD_RATE);
  mySensor.begin(SMOOTHED_AVERAGE, 10);
  mySensor2.begin(SMOOTHED_EXPONENTIAL, 10);
}

void loop() {
  Serial.println("Smoothed");
  // Read the value from the sensor
  float currentSensorValue = analogRead(SENSOR_PIN);

  // Add the new value to both sensor value stores
  mySensor.add(currentSensorValue);
  mySensor2.add(currentSensorValue);

  // Get the smoothed values
  float smoothedSensorValueAvg = mySensor.get();
  float smoothedSensorValueExp = mySensor2.get();

  // Output the smoothed values to the serial stream. Open the Arduino IDE Serial plotter to see the effects of the smoothing methods.
  Serial.print(currentSensorValue); Serial.print("\t"); Serial.print(smoothedSensorValueAvg); Serial.print("\t"); Serial.println(smoothedSensorValueExp);	

  // If needed we can also return the last stored value which will be unsmoothed
  float lastValueStoredAvg = mySensor.getLast();
  float lastValueStoredExp = mySensor2.getLast();

  delay(DELAY_LOOP);
}

