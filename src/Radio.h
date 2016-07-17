#include <Arduino.h>
#include "Module.h"

class Radio: public Module {
	int restart_time_out;
	HardwareSerial *radio_comms;
public:
	Radio(HardwareSerial *serial, int restart_time);
	void begin();
	void tick();
	int enable();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

	bool transmit(String *packet);
	String to_AX25(String *data);
	int systems_check();
};
