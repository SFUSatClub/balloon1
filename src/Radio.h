#include <Arduino.h>
#include "Module.h"
<<<<<<< HEAD

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
=======
#include "GPS.h"

class Radio: public Module {
	GPS *gps;
	HardwareSerial *radio_comms;

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
>>>>>>> Steven
	int systems_check();
};
