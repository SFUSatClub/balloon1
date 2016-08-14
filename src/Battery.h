#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "Module.h"

class Battery: public Module {
	int batteryAnalogPin;
	int dividerVoltage;
	static const int TO_WRITE_BUFFER_SIZE = 16;
	char toWrite[TO_WRITE_BUFFER_SIZE];
public:
	Battery(int batteryAnalogPin);

	void begin();
	void tick();

	const char* dataToPersist();
	const char* getModuleName();
};

#endif // BATTERY_H
