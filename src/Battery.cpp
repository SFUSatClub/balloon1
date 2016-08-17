#include "Battery.h"

Battery::Battery(int _batteryAnalogPin)
	: batteryAnalogPin(_batteryAnalogPin)
{
	allocatePersistBuffer(16);
}

void Battery::tick() {
	dividerVoltage = analogRead(batteryAnalogPin);
#ifdef DEBUG
	PP(cout << "BATT: " << dividerVoltage << endl;)
#endif
}

void Battery::begin() {
}

// format: <battery voltage as measured by 100k divider>
const char* Battery::flushPersistBuffer() {
	persistBuffer[0] = '\0';
	dividerVoltage = analogRead(batteryAnalogPin);
#ifdef DEBUG
	PP(cout << "BATT2: " << dividerVoltage << endl;)
#endif
	itoa(dividerVoltage, persistBuffer, 10);
	return persistBuffer;
}

const char* Battery::getModuleName() {
	return "Battery";
}
