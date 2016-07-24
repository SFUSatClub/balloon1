#include <Arduino.h>
#include "Module.h"
#include "GPS.h"

class Radio: public Module {
	GPS *gps;
	HardwareSerial *radio_comms;
public:
	Radio(HardwareSerial *serial, GPS *gps);
	void begin();
	void tick();
	int enable();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();
	scheduling_freq getSchedulingFreq();

	bool forwardAPRSToUno(const char *data_msg);
	int systems_check();
};
