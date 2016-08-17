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
	static const int BUFFER_SIZE = 100;
	char toWrite[BUFFER_SIZE];
public:
	GPS(HardwareSerial *);
	void begin();
	void tick();

	float getLatitude();
	float getLongitude();
	float getAltitude();
	float getSpeed();
	bool getFix();
	uint8_t getFixQuality();
	uint8_t getSats();
	uint32_t getGPSEpoch();
	const char* getTime();

	const char* dataToPersist();
	const char* getModuleName();
};

#endif // GPS_H
