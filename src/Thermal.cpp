#include <Thermal.h>
#include <Arduino.h>
#include <Wire.h>

#define VCC 3.3
#define VA 3.3 // Max voltage for analog pins. 3.3 for Due, 5 for Uno

Thermal::Thermal(int pin) : THERMAL_PIN(pin) {

}

void Thermal::begin() {
	pinMode(THERMAL_PIN, OUTPUT);
}

void Thermal::tick() {
	int sensorValue = analogRead(THERMAL_PIN);
	float vout = sensorValue * VA / 1024.0;
	float thermR = 9.93 * (VCC / vout - 1);
  currentR = thermR;
}

int Thermal::enable() {
  return 0;
}

const char* Thermal::getModuleName() {
  return "Therml Sensor";
}

// Data format:<current resistance>
const char* Thermal::dataToPersist() {
	toWrite[0] = '\0';
  snprintf(toWrite, 100, "%.6f", currentR);
	return toWrite;
}
