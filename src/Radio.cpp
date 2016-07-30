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
	bool alreadyForwarded = false;
	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleRadioData = modules[currModule]->dataToPersist();
		if(moduleRadioData == NULL) {
			continue;
		}
		// Steven: add 2 for comma and new line char
		if(strlen(buffer) + strlen(moduleRadioData) + 2 < BUFFER_WRITE_SIZE) {
			strcat(buffer, moduleRadioData);
			strcat(buffer, "\n");
			/* cout << "Radio: appending, current buffer: " << buffer << endl; */
		} else {
			/* cout << "Radio: hit buffer size, sending to uno: " << buffer << endl; */
			// Steven: c-style strings, clear the buffer with null char
			// WARNING: will send multiple aprs packets if we are sending a lot of data
			// maybe should just throw away all data if full again in current tick
			forwardAPRSToUno(buffer);
			alreadyForwarded = true;
			buffer[0] = 0;
			strcat(buffer, moduleRadioData);
			strcat(buffer, "\n");
			/* cout << "Radio: appending, current buffer: " << buffer << endl; */
		}
	}
	// Make sure we still send telemetry data even if we don't have any msgs to send
	if(!alreadyForwarded) {
		forwardAPRSToUno(NULL);
	}
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
	char toUno[BUFFER_UNO_SIZE];
	int all = snprintf(toUno, BUFFER_UNO_SIZE,
			"%f\t%f\t%lu\t%f\t%s",
			gps->getLatitude(), gps->getLongitude(),
			(long)gps->getGPSEpoch(), gps->getAltitude(),
			data_msg);
	/* snprintf(toUno, BUFFER_UNO_SIZE, */
	/* 		"%f\t%f\t%d\t%f\t%s", */
	/* 		49.2142, 122.2342, */
	/* 		2019013901, 452.2, */
	/* 		data_msg); */
	radio_comms->println(toUno);
	// return if the full string length of what we want to send (all) would
	// have fit in the buffer size of BUFFER_UNO_SIZE chars
	return all < BUFFER_UNO_SIZE;
}

scheduling_freq Radio::getSchedulingFreq() {
	scheduling_freq ret;
	ret.valid = true;
	ret.timeout = 1000;
	ret.interval = 1000*10;
	return ret;
}

int Radio::systems_check() {
	int check_result = 1337;
	return check_result;
}

const char* Radio::dataToPersist() {
	return gps->getTime();
}

const char* Radio::getModuleName() {
	return "Radio";
}

void Radio::registerModules(Module **_modules, int _numModules) {
	modules = _modules;
	numModules = _numModules;
	return;
}
