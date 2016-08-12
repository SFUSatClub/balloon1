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

#define DEBUG

void task1(void){
	static int task1Trigger = 0;
	if(task1Trigger == 1){
		digitalWrite(12, 1);
		task1Trigger = 0;
	}
	else{
		digitalWrite(12, 0);
		task1Trigger = 1;
	}
}

// Module setup
#define gpsSerial Serial1
#define radioSerial Serial2
const int sdChipSelectPin = 4;
const int buzzerEnablePin = 13;

GPS gps(&gpsSerial);
Radio radio(&radioSerial, &gps);
SDCard sd(sdChipSelectPin);
Photocells photocells(0, 5);
// uses i2c init, internal addr
IMU imu;
// uses i2c init, internal addr
Barometer barometer;
Buzzer buzzer(buzzerEnablePin);
Thermal tempSensor(6);

// Steven: maybe should use container classes. array/vector?
const int numModules = 8;
Module* modules[numModules] = {
	  &gps
	, &radio
	, &sd
	, &photocells
	, &imu
	, &barometer
	, &buzzer
	, &tempSensor
};

Scheduler scheduler(1 + numModules);
StateHandler stateHandler(&barometer, &gps);

void setup() {
	Wire.begin();
	Wire.setClock(400000);
	Serial.begin(115200);
	pinMode(12, OUTPUT);
	Serial.println("SFUSat weather balloon1 says hi");

	for(Module *module : modules) {
		module->enable();
		module->begin();
	}
	stateHandler.enable();
	stateHandler.begin();

	sd.registerModules(modules, numModules);
	radio.registerModules(modules, numModules);

	scheduler.setupISR();
	scheduler.addTask(new Task(500,500, &task1));
	scheduler.registerModulesAsTasks(modules, numModules);
	scheduler.registerStateHandler(&stateHandler);
}

void loop() {
	scheduler.run();
#ifdef DEBUG
	if(Serial.available()) {
		char buffer[32];
		buffer[0] = '\0';
		Serial.readBytesUntil('\n', buffer, 32);
		int inputLen = strlen(buffer);
		cout << "inputLen: " << inputLen << endl;

		switch(buffer[0]) {
			default: // fall-through
			case 'h': { // help
				cout <<
					F("\n\nSFUSat Balloon1\n\n"
					">h\n"
					"\tDisplay this help screen\n\n"
					">l\n"
					"\tDisplay a list of activated module IDs and their names\n\n"
					">s[MODULE_ID|(a)ll][PROPERTY_TYPE][PROPERTY_VALUE] - set module attribute\n"
					"\tUse \"a\" instead of MODULE_ID to target all modules\n"
					"\tEx: s0p0 - set module with id 0 property p a value of 0 (tells it to stop printing)\n"
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
					cout << "Too few characters; try again" << endl;
					break;
				}

				break;
			}
		}
		cout << "said: " << buffer << endl;
	}
#endif  // DEBUG
}
