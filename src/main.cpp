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

void task2(){
  static int task2Trigger = 0;
  Serial.println("Hello");

  if(task2Trigger == 1){
    digitalWrite(8, 1);
    task2Trigger = 0;
  }
  else{
    digitalWrite(8, 0);
    task2Trigger = 1;
  }
}

// Module setup
#define gpsSerial Serial1
#define radioSerial Serial2
const int sdChipSelectPin = 4;

GPS gps(&gpsSerial);
Radio radio(&radioSerial, 2);
SDCard sd(sdChipSelectPin);
Photocells photocells(0, 5);
IMU imu;
Barometer barometer;

// Steven: maybe should use container classes. array/vector?
const int numModules = 5;
Module* modules[numModules] = {
    &gps
  , &radio
  , &sd
  , &photocells
  , &barometer
};

Scheduler scheduler(2 + numModules);
StateHandler stateHandler(&barometer, &gps);

void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("SFUSat weather balloon1 says hi");

  for(Module *module : modules) {
    module->enable();
    module->begin();
  }
  stateHandler.enable();
  stateHandler.begin();

  sd.registerModules(modules, numModules);

  scheduler.setupISR();
  scheduler.addTask(new Task(500,500, &task1));
  scheduler.addTask(new Task(500,100, &task2));
  scheduler.registerModulesAsTasks(modules, numModules);
  scheduler.registerStateHandler(&stateHandler);
}

void loop() {
  scheduler.run();
}

// Richard todo:
// add the watchdog to the Time -> runTask function
