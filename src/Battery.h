#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "Module.h"

class Battery: public Module {
	int batteryAnalogPin;
	int dividerVoltage;
public:
	Battery(int batteryAnalogPin);

	void begin();
	void tick();

	const char* flushPersistBuffer();
	const char* getModuleName();
};

#endif // BATTERY_H
