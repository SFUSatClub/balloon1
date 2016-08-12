#include "Radio.h"

Radio::Radio(HardwareSerial *ser, GPS *_gps)
	: gps(_gps)
	, radio_comms(ser)
{

}

void Radio::begin() {
	radio_comms->begin(19200);
	// on startup, uno expects "s\n" to be sent
	cout << "Waiting for radio..." << endl;
	long startTime = millis();
	bool success = false;
	while(millis() - startTime < 2000 && !success) {
		radio_comms->print("s\r");
		if(radio_comms->available()) {
			/* char buffer[8]; */
			/* int b = radio_comms->readBytes(buffer, 3); */
			/* cout << "radio got: " << buffer << endl; */
			/* buffer[b] = '\0'; */
			/* if(strcmp(buffer, "ack") == 0) { */
			/* 	success = true; */
			/* } */
			char buffer[8];
			int b = radio_comms->readBytesUntil('\r', buffer, 8);
			buffer[b] = '\0';
			if(strcmp(buffer, "ack") == 0) {
				success = true;
			}
		}
	}
	if(success) {
		cout << "Radio connected" << endl;
	} else {
		cout << "Radio failed to connect" << endl;
	}
}

void Radio::tick() {
	bool alreadyForwarded = false;
	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleRadioData = modules[currModule]->dataToSend();
		if(moduleRadioData == NULL) {
			continue;
		}
		// Steven: add 2 for comma and new line char
		// TODO: fix when dataToSend size is bigger than BUFFER_WRITE_SIZE, due will crash
		if(strlen(buffer) + strlen(moduleRadioData) + 2 < BUFFER_WRITE_SIZE) {
			strcat(buffer, moduleRadioData);
			strcat(buffer, "\n");
			/* cout << "Radio: appending, current buffer: " << buffer << endl; */
		} else {
			/* cout << "Radio: hit buffer size, sending to uno: " << buffer << endl; */
			// Steven: c-style strings, clear the buffer with null char
			// WARNING: will send multiple aprs packets if we are sending a lot of data
			// maybe should just throw away all data if full again in current tick
			if(!alreadyForwarded) {
				forwardAPRSToUno(buffer);
				alreadyForwarded = true;
			}
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

// a<latitude>\t<longitude>\t<time>\t<altitude>\t<misc data>\r
bool Radio::forwardAPRSToUno(const char *data_msg) {
	char toUno[BUFFER_UNO_SIZE];
	char millisStr[10];
	snprintf(millisStr, 10, "%d", millis());
	int all = snprintf(toUno, BUFFER_UNO_SIZE,
			"a%f\t%f\t%d\t%f\t%s\r",
			gps->getLatitude(), gps->getLongitude(),
			gps->getGPSEpoch(), gps->getAltitude(),
			millisStr);
	/* snprintf(toUno, BUFFER_UNO_SIZE, */
	/* 		"%f\t%f\t%d\t%f\t%s", */
	/* 		49.2142, 122.2342, */
	/* 		2019013901, 452.2, */
	/* 		data_msg); */
	cout << "Sending to radio...." << endl << toUno << endl;
	radio_comms->print(toUno);
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
