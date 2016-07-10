#include "GPS.h"

MyGPS::MyGPS(Stream * ser) {
	usingInterrupt = false;
#ifdef __AVR__
#if ARDUINO >= 100 
	gpsImpl = new Adafruit_GPS((SoftwareSerial *)ser); // Constructor when using SoftwareSerial
#else
	gpsImpl = new Adafruit_GPS((NewSoftSerial *)ser); // Constructor when using NewSoftSerial
#endif
#endif
	gpsImpl = new Adafruit_GPS((HardwareSerial *)ser); // Constructor when using HardwareSerial

	serial = ser;

}

void MyGPS::begin(uint16_t baud) {
	gpsImpl->begin(baud);
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

	// the nice thing about this code is you can have a timer0 interrupt go off
	// every 1 millisecond, and read data from the GPS for you. that makes the
	// loop code a heck of a lot easier!

#ifdef __arm__
	usingInterrupt = false;  //NOTE - we don't want to use interrupts on the Due
#else
	useInterrupt(true);
#endif

	delay(1000);
	// Ask for firmware version
	serial->println(PMTK_Q_RELEASE);
	timer = millis();
}

void MyGPS::tick() {
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = gpsImpl->read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
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

#ifdef __AVR__
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
#endif //#ifdef__AVR__
