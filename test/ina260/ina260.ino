/*--------------------------------------------------------------
  Program:      ina260

  Description:  Test the voltage, current, and power readings
                of the Adafruit INA260 High or Low Side Voltage,
                Current, Power Sensor.

  Hardware:     Adafruit Feather HUZZAH32 with 3.7V lipo battery,
                Adafruit 128x64 OLED, Adafruit Powerboost 1000
                Basic, and Adafruit bq24074 solar charger.

  Software:     Developed using arduino-cli 0.21.1

  Date:         12MAR2022

  Author:       Nicholas Wilde 0x08b7d7a3
--------------------------------------------------------------*/

#include <Adafruit_INA260.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define VOLTAGE_MAX 4.2         // max voltage of lipo battery (V)
#define VOLTAGE_MIN 2.64        // min voltage of lipo battery (V)

const long interval = 1;
unsigned long previousMillis = 0;

const int batteryCapacity = 3000; // Battery capacity in mAh
const int batteryQuantity = 2;    // Quantity of batteries

Adafruit_INA260 ina260 = Adafruit_INA260();
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void setup() {
  Serial.begin(115200);
  // Wait until serial port is opened
  while (!Serial) { delay(10); }
  setupDisplay();
  setupPins();
  Serial.println("Adafruit INA260 Test");
  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");
  ina260.setAveragingCount(INA260_COUNT_16);

}

void loop() {
  String mode;

  if (checkButton()) digitalWrite(33, !digitalRead(33));

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval*1e3) {
    display.clearDisplay();
    display.setCursor(0,0);
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    String temp;
    int current = ina260.readCurrent();
    // Determine the mode
    if (current<0){
      mode = "recharge";
      current = -1*current;
    } else {
      mode = "discharge";
    }

    Serial.print("Mode:      ");
    display.print("Mode:      ");
    Serial.println(mode);
    display.println(mode);

    temp = formatValue(current);
    Serial.print("Current:   ");
    display.print("Current:   ");
    Serial.print(temp);
    display.print(temp);
    Serial.println("A");
    display.println("A");

    int voltage = ina260.readBusVoltage();
    temp = formatValue(voltage);
    Serial.print("Voltage:   ");
    display.print("Voltage:   ");
    Serial.print(temp);
    display.print(temp);
    Serial.println("V");
    display.println("V");

    int power = ina260.readPower();
    temp = formatValue(power);
    Serial.print("Power:     ");
    display.print("Power:     ");
    Serial.print(temp);
    display.print(temp);
    Serial.println("W");
    display.println("W");

    batteryTime(current, voltage, mode);

    display.display();
    Serial.println();
  }
}

void setupDisplay(){
  display.begin(0x3C, true); // Address 0x3C default

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
}

void batteryTime(int current, int voltage, String mode){
  float timeTotal = (float)(batteryQuantity*batteryCapacity)/current;
  float timeLeft;
  if (mode == "discharge") {
    timeLeft = (float)map(voltage, VOLTAGE_MIN*1000, VOLTAGE_MAX*1000, 0, timeTotal*100)/100;
  } else {
    timeLeft = (float)map(voltage, VOLTAGE_MIN*1000, VOLTAGE_MAX*1000, timeTotal*100, 0)/100;
  }
  int hours = timeLeft;
  int minutes = (float)(timeLeft-hours)*60;
  Serial.print("Time left: ");
  display.print("Time left: ");
  Serial.print(hours);
  display.print(hours);
  Serial.print(":");
  display.print(":");
  if (minutes<10){
    Serial.print("0");
    display.print("0");
  }
  Serial.println(minutes);
  display.println(minutes);
}

bool checkButton() {
	static bool oldButton;
	bool but = !digitalRead(15);
	delay(40);	// simple debounce button
	if( but != oldButton){
		oldButton = but;
		return true && !oldButton;
	}
	return false;
}

void setupPins(){
  pinMode(15, INPUT_PULLUP);
  pinMode(33, OUTPUT);
}

String formatValue(int val){
  String buffer;
  if (val < 0) val = val*-1;
  if (val >= 1000){
    float volts = (float)val/1000;
    buffer = String(volts) + " ";
  } else {
    buffer = String(val) + " m";
  }
  return buffer;
}
