#include <Arduino.h>
#include "Module.h"
#include "GPS.h"

class Radio: public Module {
	GPS *gps;
	HardwareSerial *radio_comms;
	uint32_t currForwardCount = 0;

	int numModules;
	Module **modules;
	static const uint16_t BUFFER_UNO_SIZE = 100;
	static const uint32_t BUFFER_WRITE_SIZE = 99;
	char buffer[BUFFER_WRITE_SIZE];
public:
	Radio(HardwareSerial *serial, GPS *gps);
	void begin();
	void tick();

	const char* getModuleName();
	const char* dataToPersist();
	scheduling_freq getSchedulingFreq();

	void registerModules(Module *modules[], int numModules);
	bool forwardAPRSToUno(const char *data_msg);
	int systems_check();
};
