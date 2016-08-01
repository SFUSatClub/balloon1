#include "Radio.h"

Radio::Radio(HardwareSerial *ser, int restart_time)
	: radio_comms(ser) {
	restart_time_out  = restart_time;
}

void Radio::begin() {
	radio_comms->begin(19200);
}

void Radio::tick() {
}

int Radio::enable() {
	//PD1
	return restart_time_out;
}

void Radio::disable() {
	//PD0
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

int Radio::systems_check() {
	int check_result = 1337;
	return check_result;
}

const char* Radio::dataToPersist() {
	return "returning some radio data";
}

const char* Radio::getModuleName() {
	return "Radio";
}
