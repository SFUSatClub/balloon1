#include "Barometer.h"

Barometer::Barometer() {
	baromImpl = new SFE_BMP180();
} // on Due, SDA to 20, SCL to 21

void Barometer::begin() {
	if ( baromImpl->begin() ) {
		cout << "Barometer initialized properly." << endl;
		state =  State::BEGIN_SUCCESS;
	} else { 
		cout << "Barometer failed to initialize" << endl;
		state =  State::BEGIN_FAILED;
	}
}

void Barometer::tick() {
	if(state == State::BEGIN_FAILED) {
		return;
	}
	char status;

	// Start a temperature measurement:
	// If request is successful, the number of ms to wait is returned.
	// If request is unsuccessful, 0 is returned.

	status = baromImpl->startTemperature();
	// Wait for the measurement to complete:
	delay(status);

	// Retrieve the completed temperature measurement:
	// Note that the measurement is stored in the variable T.
	// Function returns 1 if successful, 0 if failure.

	status = baromImpl->getTemperature(temperature);
	// Print out the measurement:
	/* cout << "temperature: " << temperature << "deg C" <<endl; */

	// Start a pressure measurement:
	// The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
	// If request is successful, the number of ms to wait is returned.
	// If request is unsuccessful, 0 is returned.

	status = baromImpl->startPressure(3);
	// Wait for the measurement to complete:
	delay(status);

	// Retrieve the completed pressure measurement:
	// Note that the measurement is stored in the variable P.
	// Note also that the function requires the previous temperature measurement (T).
	// (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
	// Function returns 1 if successful, 0 if failure.

	status = baromImpl->getPressure(pressure,temperature);

	// Print out the measurement:
	/* cout << "absolute pressure:" << pressure << "in mb:"; */
	/* cout << pressure*0.0295333727 << "inHg"<< endl; */
}

/*
// On the other hand, if you want to determine your altitude from the pressure reading,
// To remove the effects of altitude, use the sealevel function and your current altitude.
// This number is commonly used in weather reports.
// Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
// Result: p0 = sea-level compensated pressure in mb

p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
Serial.print("relative (sea-level) pressure: ");
Serial.print(p0,2);
Serial.print(" mb, ");
Serial.print(p0*0.0295333727,2);
Serial.println(" inHg");

// On the other hand, if you want to determine your altitude from the pressure reading,
// use the altitude function along with a baseline pressure (sea-level or other).
// Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
// Result: a = altitude in m.

a = pressure.altitude(P,p0);
Serial.print("computed altitude: ");
Serial.print(a,0);
Serial.print(" meters, ");
Serial.print(a*3.28084,0);
Serial.println(" feet");

*/


int Barometer::enable() {
	return 0;
}

void Barometer::disable() {
}

// Data format: <temperature in deg c>,<abs pressure in Hg>
const char* Barometer::dataToPersist() {
	toWrite[0] = '\0';
	snprintf(toWrite, 100, "%0.6f,%0.6f",temperature,pressure);
	return toWrite;
}

const char* Barometer::getModuleName() {
	return "Barometer";
}

float Barometer::getPressure(){
	return pressure;
}
