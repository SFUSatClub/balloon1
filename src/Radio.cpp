#include "Radio.h"

Radio::Radio(int restart_time, int tx_pin, int rx_pin) {
	restart_time_out  = time;
	radio_comms = SoftwareSerial(rx_pin, tx_pin);
	radio_comms.begin(19200);
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
	return check_result
}
