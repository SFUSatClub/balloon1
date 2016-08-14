#include "Thermal.h"

#define VCC 3.3
#define VA 3.3 // Max voltage for analog pins. 3.3 for Due, 5 for Uno

// Values obtained from the datasheet for the thermistor NTCLE100E3333JB0
// Need to be changed if abother termistor is used
#define A1T 0.003354016
#define B1T 0.0002519107
#define C1T 0.000003510939
#define D1T 0.0000001105179
#define RREF 33 // Reference resistance in kOhms

Thermal::Thermal(int pin, double R, char const* id) :
	THERMAL_PIN(pin),
	R2(R)
{
	strcpy(name, "Tmp:");
	strcat(name, id);
}

void Thermal::begin() {
	pinMode(THERMAL_PIN, OUTPUT);
}

void Thermal::tick() {
	int sensorValue = analogRead(THERMAL_PIN);
	float vout = sensorValue * VA / 1024.0;
	float thermR = R2 * (VCC / vout - 1);
	currentR = thermR;
	// Calculating temperature using Steinhart–Hart equation
	float lnRRref = log(thermR/RREF);
	float tempK = 1/(A1T+B1T*lnRRref+C1T*(lnRRref*lnRRref)+D1T*(lnRRref*lnRRref*lnRRref));
	float tempC = tempK - 273.15;
	currentTempC = tempC;
}

const char* Thermal::getModuleName() {
	return name;
}

// Data format:<resistance>,<temperature in C>
const char* Thermal::dataToPersist() {
	toWrite[0] = '\0';
	snprintf(toWrite, BUFFER_SIZE, "%.6f,%.6f", currentR, currentTempC);
	return toWrite;
}
