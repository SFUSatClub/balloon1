#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"
#include <SFE_BMP180.h>
#include <Wire.h>

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.

class Barometer: public Module { 
	SFE_BMP180 *baromImpl;
	float altitude, temp, pressure, p0, a;
	HardwareSerial *Serial;

	char toWrite[200];             //max buffer size may have to change depending on data

public:
	Barometer(HardwareSerial *);
	void begin();
	void tick();
	int enable();
	void disable();

	const char* dataToPersist();
	const char* getModuleName();
};
