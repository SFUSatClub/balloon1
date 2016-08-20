#include "Battery.h"

Battery::Battery(int _batteryAnalogPin)
	: batteryAnalogPin(_batteryAnalogPin)
{
	allocatePersistBuffer(32);
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
	dividerVoltage = analogRead(batteryAnalogPin);
#ifdef DEBUG
	PP(cout << "BATT2: " << dividerVoltage << endl;)
#endif
	snprintf(persistBuffer, persistBufferSize, "%d\n", dividerVoltage);
	return persistBuffer;
}

const char* Battery::getModuleName() {
	return "Battery";
}
