#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "Module.h"

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  false

class GPS: public Module {
	Adafruit_GPS *gpsImpl;
	HardwareSerial *serial;
	uint32_t timer;
	char time[16];
	char toWrite[100];
public: 
	GPS(HardwareSerial *);
	void begin();
	void tick();
	int enable();
	void disable();

	float getLatitude();
	float getLongitude();
	float getAltitude();
	float getSpeed();
	uint32_t getGPSEpoch();
	const char* getTime();

	scheduling_freq getSchedulingFreq();
	const char* dataToPersist();
	const char* getModuleName();
};

#endif // GPS_H
