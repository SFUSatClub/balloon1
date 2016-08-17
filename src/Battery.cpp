#include "Battery.h"

Battery::Battery(int _batteryAnalogPin)
	: batteryAnalogPin(_batteryAnalogPin)
{
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
const char* Battery::dataToPersist() {
	toWrite[0] = '\0';
	dividerVoltage = analogRead(batteryAnalogPin);
#ifdef DEBUG
	PP(cout << "BATT2: " << dividerVoltage << endl;)
#endif
	itoa(dividerVoltage, toWrite, 10);
	return toWrite;
}

const char* Battery::getModuleName() {
	return "Battery";
}
