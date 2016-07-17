#include "GPS.h"

GPS::GPS(HardwareSerial *ser) {
	gpsImpl = new Adafruit_GPS(ser); // Constructor when using HardwareSerial

	serial = ser;
}

void GPS::begin() {
	// 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
	serial->begin(9600);
	gpsImpl->begin(9600);
	// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
	gpsImpl->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// uncomment this line to turn on only the "minimum recommended" data
	//gpsImpl->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	// For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
	// the parser doesn't care about other sentences at this time

	// Set the update rate
	gpsImpl->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	// For the parsing code to work nicely and have time to sort thru the data, and
	// print it out we don't suggest using anything higher than 1 Hz

	// Request updates on antenna status, comment out to keep quiet
	gpsImpl->sendCommand(PGCMD_ANTENNA);

	// Ask for firmware version
	serial->println(PMTK_Q_RELEASE);
	timer = millis();
}

void GPS::tick() {
	// read data from the GPS in the 'main loop'
	char c = gpsImpl->read();
	// if you want to debug, this is a good time to do it!
	if (GPSECHO)
		if (c) Serial.print(c);

	// if a sentence is received, we can check the checksum, parse it...
	if (gpsImpl->newNMEAreceived()) {
		// a tricky thing here is if we print the NMEA sentence, or data
		// we end up not listening and catching other sentences! 
		// so be very wary if using OUTPUT_ALLDATA and trytng to print out data
		//Serial.println(gpsImpl->lastNMEA());   // this also sets the newNMEAreceived() flag to false

		if (!gpsImpl->parse(gpsImpl->lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return;  // we can fail to parse a sentence in which case we should just wait for another
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, print out the current stats
	if (millis() - timer > 2000) { 
		timer = millis(); // reset the timer

		Serial.print("\nTime: ");
		Serial.print(gpsImpl->hour, DEC); Serial.print(':');
		Serial.print(gpsImpl->minute, DEC); Serial.print(':');
		Serial.print(gpsImpl->seconds, DEC); Serial.print('.');
		Serial.println(gpsImpl->milliseconds);
		Serial.print("Date: ");
		Serial.print(gpsImpl->day, DEC); Serial.print('/');
		Serial.print(gpsImpl->month, DEC); Serial.print("/20");
		Serial.println(gpsImpl->year, DEC);
		Serial.print("Fix: "); Serial.print((int)gpsImpl->fix);
		Serial.print(" quality: "); Serial.println((int)gpsImpl->fixquality); 
		if (gpsImpl->fix) {
			Serial.print("Location: ");
			Serial.print(gpsImpl->latitude, 4); Serial.print(gpsImpl->lat);
			Serial.print(", "); 
			Serial.print(gpsImpl->longitude, 4); Serial.println(gpsImpl->lon);

			Serial.print("Speed (knots): "); Serial.println(gpsImpl->speed);
			Serial.print("Angle: "); Serial.println(gpsImpl->angle);
			Serial.print("Altitude: "); Serial.println(gpsImpl->altitude);
			Serial.print("Satellites: "); Serial.println((int)gpsImpl->satellites);
		}
	}
}

int GPS::enable() {
	return 0;
}

void GPS::disable() {
}

const char* GPS::dataToPersist() {
	return "returning some gps data";
}

const char* GPS::getModuleName() {
	return "GPS";
}


