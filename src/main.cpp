// Richard Notes for SFUSat:
// Make sure to add the 328 or the Due to your platformio environment (PlatformIO > Initialize new platformio target or update existing, choose Due programming port)
// if you have multiple targets, you might need to select the one to build or upload to, type:
// platformio run --target upload --environment due
// or select PlatformIO > Run Other target
// the programming port on the Due is the one closest to the DC power jack
// Info about Due timer counter: http://2manyprojects.net/timer-interrupts
//                              http://forum.arduino.cc/index.php?topic=130423.0
// The functions used to configure the Due timers are from the Atmal ASF API: http://asf.atmel.com/docs/latest/api.html

#include <Arduino.h>

#include "Task.h"
#include "Scheduler.h" // deals with all the scheduling stuff
#include "StateHandler.h"

#include "Radio.h"
#include "GPS.h"
#include "SDCard.h"
#include "Photocells.h"
#include "IMU.h"
#include "Barometer.h"
#include "Buzzer.h"
#include "Barometer5540.h"	// Richard: the nice barometer
#include "Thermal.h"
#include "Battery.h"

#define DEBUG

// Module setup
#define gpsSerial Serial1
#define radioSerial Serial2
const int sdChipSelectPin = 4;
const int buzzerEnablePin = 13;
// do not plug anything into A0; its floating value is used as seed for rng
const int randomSeedPin = A0;
const int tempInsidePin = A6;
const int tempOutsidePin = A7;
const int batteryPin = A8;

GPS gps(&gpsSerial);
Radio radio(&radioSerial, &gps);
SDCard sd(sdChipSelectPin);
Photocells photocells(0, 5);
// uses i2c init, internal addr
IMU imu;
// uses i2c init, internal addr
Barometer barometer;
Buzzer buzzer(buzzerEnablePin);
Thermal tempInside(tempInsidePin, 10.0, "in");
Thermal tempOutside(tempOutsidePin, 10.0, "out");
Battery battery(batteryPin);

const int numModules = 10;
Module* modules[numModules] = {
	// long running modules first
	  &gps
	, &radio
	, &imu
	// quick running modules
	, &buzzer
	, &photocells
	, &barometer
	, &tempInside
	, &tempOutside
	, &battery
	// if SD module is last, it will run last (write after all modules are ticked)
	, &sd
};

Scheduler scheduler(numModules);
StateHandler stateHandler(&barometer, &gps);

long seed = 0;

void setup() {
	// get a random seed to establish common startup id on logs
	randomSeed(analogRead(randomSeedPin));
	seed = random(10000, 99999);
	// SAM3X8E hardware rng, to be tested.
	/* pmc_enable_periph_clk(ID_TRNG); */
	/* trng_enable(TRNG); */
	/* uint32_t trand = trng_read_output_data(TRNG); */

	Wire.begin();
	Wire.setClock(400000);
	Serial.begin(115200);
	Serial.println("SFUSat weather balloon1 says hi");

	for(Module *module : modules) {
		module->enable();
		module->begin();
	}
	stateHandler.enable();
	stateHandler.begin();

	// random seed from system to identify this particular startup
	sd.registerSeed(seed);
	sd.registerModules(modules, numModules);
	sd.registerScheduler(&scheduler);
	radio.registerModules(modules, numModules);

	scheduler.setupISR();
	scheduler.registerModules(modules, numModules);
	scheduler.registerStateHandler(&stateHandler);
}

void loop() {
	WATCHDOG_RESET();
	scheduler.run();
#ifdef DEBUG
	if(Serial.available()) {
		char buffer[32];
		buffer[0] = '\0';
		Serial.readBytesUntil('\n', buffer, 32);
		int inputLen = strlen(buffer);
		cout << "inputLen: " << inputLen << endl;

		const char command = buffer[0];
		switch(command) {
			default: // fall-through
			case 'h': { // help
				cout <<
					F("\n\nSFUSat Balloon1 Command Set\n\n"
					"h\n"
					"\tDisplay this help screen\n\n"
					"l\n"
					"\tDisplay a list of activated module IDs and their names\n\n"
					"s[MODULE_ID|(a)ll][PROPERTY_TYPE][PROPERTY_VALUE]\n"
					"\tSet module attribute\n"
					"\tUse \"a\" instead of MODULE_ID to target all modules\n"
					"\tEx: s0p0 - set property p of module with id 0 to a value of 0 (tells it to stop printing)\n"
					"\tEx: sap1 - set property p of all values to a value of 1 (print everything from all modules)\n"
					"\n\n"
					)
				<< endl;
				break;
			} case 'l': { // list module ids and their names
				for(size_t i = 0; i < numModules; i++) {
					Module* m = modules[i];
					cout << i << " - " << m->getModuleName() << endl;
				}
				break;
			} case 's': { // set module attributes
				// min length is 5 characters s0p0\0
				if(inputLen < 5) {
					cout << "Too few characters" << endl;
					break;
				}
				const char moduleID = buffer[1];
				const int moduleIDint = moduleID - '0';
				// if moduleID is out of range, break
				if(moduleID != 'a' && (moduleIDint < 0 || moduleIDint >= numModules)) {
					cout << "Wrong moduleID" << endl;
					break;
				}
				const char property = buffer[2];
				const char val = buffer[3];
				switch(property) {
					case 'p': { // print
						// selecting all modules
						if(moduleID == 'a') {
							for(size_t i = 0; i < numModules; i++) {
								Module* m = modules[i];
							}
							break;
						}
						Module* m = modules[moduleIDint];

					}
				}
				break;
			}
		}
		cout << "said: " << buffer << endl;
	}
#endif  // DEBUG
}
