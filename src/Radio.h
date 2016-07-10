#include <Arduino.h>
#include <SoftwareSerial.h>

class Radio {
	int restart_time_out;
	SoftwareSerial radio_comms;
	public:
		Radio(int);
		bool transmit(String *packet);
		String to_AX25(String *data);
		void disable();
		int enable();
		int systems_check();
};
