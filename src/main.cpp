<<<<<<< HEAD

// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code shows how to listen to the GPS module in an interrupt
// which allows the program to have more 'freedom' - just parse
// when a new NMEA sentence is available! Then access data when
// desired.
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada

#include <Arduino.h>
#include "Radio.h"
#include "GPS.h"
#include "SDCard.h"

using namespace std;

// This sketch is ONLY for the Arduino Due!
// You should make the following connections with the Due and GPS module:
// GPS power pin to Arduino Due 3.3V output.
// GPS ground pin to Arduino Due ground.
// For hardware serial 1 (recommended):
//   GPS TX to Arduino Due Serial1 RX pin 19
//   GPS RX to Arduino Due Serial1 TX pin 18
#define gpsSerial Serial1
#define radioSerial Serial2
const int sdChipSelectPin = 4;

GPS gps(&gpsSerial);
Radio radio(&radioSerial, 2);
SDCard sd(sdChipSelectPin);

// Steven: maybe should use container classes. array/vector?
const int numModules = 3;
Module* modules[numModules] = {&gps, &radio, &sd};

void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  // Steven: example base class usage
  for(Module *module : modules) {
    module->enable();
  }

  gps.begin();
  radio.begin();
  sd.begin();

  sd.registerModules(modules, numModules);
  // sd.doSDTimingBenchmark();
}

void loop() {
  gps.tick();
  radio.tick();
  sd.tick();
=======
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
Thermal tempSensor(6, 10.0, "inner"); 

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
	while(Serial.available()) {
		/* char buffer[10]; */
		/* Serial.readBytesUntil('\n', buffer, 10); */
		cin.readline();
		char command;
		cin >> command;
		cout << "said: " << command << endl;
	}
#endif  // DEBUG
>>>>>>> Steven
}
