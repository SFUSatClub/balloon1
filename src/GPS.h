<<<<<<< HEAD
=======
#ifndef GPS_H
#define GPS_H

>>>>>>> Steven
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "Module.h"

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
<<<<<<< HEAD
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  true
=======
// Set to 'true' if you want to debug and listen to the raw GPS sentences.
#define GPSECHO  false
>>>>>>> Steven

class GPS: public Module {
	Adafruit_GPS *gpsImpl;
	HardwareSerial *serial;
	uint32_t timer;
<<<<<<< HEAD
public: 
	GPS(HardwareSerial *);
	void begin();
	void tick();
	int enable();
	void disable();

	const char* dataToPersist();
	const char* getModuleName();
};
=======
	char time[16];
	char toWrite[100];
public:
	GPS(HardwareSerial *);
	void begin();
	void tick();

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
>>>>>>> Steven
