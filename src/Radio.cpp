#include "Radio.h"

Radio::Radio(HardwareSerial *ser, int restart_time) {
	restart_time_out  = restart_time;
	radio_comms->begin(19200);
}

bool Radio::transmit(String * packet) {
	//PTT to low
	//tx data over uart
	return true;
}

String Radio::to_AX25(String * data) {
	//encode data into packet
	return " ";
}

void Radio::disable() {
	//PD0
}

int Radio::enable() {
	//PD1
	return restart_time_out;
}

int Radio::systems_check() {
	int check_result = 1337;
	return check_result;
}
