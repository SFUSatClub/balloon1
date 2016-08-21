#include "Radio.h"

Radio::Radio(HardwareSerial *ser, GPS *_gps)
	: gps(_gps)
	, radio_comms(ser)
{
	currForwardCount = 0;

	freq.interval = 1000*10;
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
		cout << "Radio failed" << endl;
	}
}

void Radio::tick() {
	bool alreadyForwarded = false;
	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleRadioData = modules[currModule]->flushSendBuffer();
		if(moduleRadioData == NULL) {
			continue;
		}
		// Steven: add 2 for comma and new line char
		// TODO: fix when flushSendBuffer size is bigger than BUFFER_WRITE_SIZE, due will crash
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
	currForwardCount++;
	/* int all = snprintf(toUno, BUFFER_UNO_SIZE, */
	/* 		"a%d\t%d\t%lu\t%.4f\t%lu,%d,%d\r", */
	/* 		(int)(gps->getLatitude()*100), (int)(gps->getLongitude()*100), */
	/* 		gps->getGPSEpoch(), gps->getAltitude(), */
	/* 		currForwardCount, gps->getFixQuality(), gps->getSats()); */
	int all = snprintf(toUno, BUFFER_UNO_SIZE,
			"a%.2f\t%.2f\t%lu\t%.4f\t%lu,%d,%d\r",
			gps->getLatitude(), gps->getLongitude(),
			gps->getGPSEpoch(), gps->getAltitude(),
			currForwardCount, gps->getFixQuality(), gps->getSats());
	/* snprintf(toUno, BUFFER_UNO_SIZE, */
	/* 		"%f\t%f\t%d\t%f\t%s", */
	/* 		49.2142, 122.2342, */
	/* 		2019013901, 452.2, */
	/* 		data_msg); */
#ifdef DEBUG
	PP(cout << "Sending to radio: " << endl << toUno << endl;)
#endif
	radio_comms->print(toUno);
	// return if the full string length of what we want to send (all) would
	// have fit in the buffer size of BUFFER_UNO_SIZE chars
	return all < BUFFER_UNO_SIZE;
}

int Radio::systems_check() {
	int check_result = 1337;
	return check_result;
}

const char* Radio::flushPersistBuffer() {
	return NULL;
}

const char* Radio::getModuleName() {
	return "Radio";
}

void Radio::registerModules(Module **_modules, int _numModules) {
	modules = _modules;
	numModules = _numModules;
	return;
}
