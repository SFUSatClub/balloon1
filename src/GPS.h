#include <Arduino.h>
#include <Adafruit_GPS.h>

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  true


class MyGPS {
	Adafruit_GPS *gpsImpl;
	Stream *serial;
	uint32_t timer;
public: 
	MyGPS(Stream *s);
	void begin(uint16_t baud);
	void tick();
};
