#include "Radio.h"

Radio::Radio(HardwareSerial *ser, GPS *_gps)
	: gps(_gps)
	, radio_comms(ser)
{
	
}

void Radio::begin() {
	radio_comms->begin(19200);
}

void Radio::tick() {
}

int Radio::enable() {
	//PD1
	return 0;
}

void Radio::disable() {
	//PD0
}

// <latitude>\t<longitude>\t<time>\t<altitude>\t<misc data>\n
bool Radio::forwardAPRSToUno(const char *data_msg) {
	char toUno[100];
	snprintf(toUno, 100, 
			"%f\t%f\t%s\t%f\t%s",
			gps->getLatitude(), gps->getLongitude(),
			gps->getTime(), gps->getAltitude(),
			data_msg);
	radio_comms->println(toUno);
	return true;
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
