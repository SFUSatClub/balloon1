#include <Arduino.h>

class Radio {
	int restart_time_out;
	HardwareSerial *radio_comms;
	public:
		Radio(HardwareSerial *seral, int restart_time);
		bool transmit(String *packet);
		String to_AX25(String *data);
		void disable();
		int enable();
		int systems_check();
};
