#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Module.h"

class Radio: public Module {
	int restart_time_out;
	SoftwareSerial *radio_comms;
public:
	Radio(SoftwareSerial *serial, int restart_time);
	void begin();
	void tick();
	int enable();
	void disable();

	bool transmit(String *packet);
	String to_AX25(String *data);
	int systems_check();
};
