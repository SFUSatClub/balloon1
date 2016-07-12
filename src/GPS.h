#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "Module.h"

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  true

class GPS: public Module {
	Adafruit_GPS *gpsImpl;
	SoftwareSerial *serial;
	uint32_t timer;
public: 
	GPS(SoftwareSerial *);
	void begin();
	void tick();
	int enable();
	void disable();
};
