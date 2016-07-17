// Richard Notes for SFUSat:
// Make sure to add the 328 or the Due to your platformio environment (PlatformIO > Initialize new platformio target or update existing, choose Due programming port)
// if you have multiple targets, you might need to select the one to build or upload to, type:
// platformio run --target upload --environment due
// or select PlatformIO > Run Other target
// the programming port on the Due is the one closest to the DC power jack
// Info about Due timer counter: http://2manyprojects.net/timer-interrupts
//                              http://forum.arduino.cc/index.php?topic=130423.0
// The functions used to configure the Due timers are from the Atmal ASF API: http://asf.atmel.com/docs/latest/api.html

#include <Arduino.h> //Richard: required for platformIO
#include "Task.h"
#include "Scheduler.h" // deals with all the scheduling stuff

Scheduler scheduler(2);

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

void setup(){
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  scheduler.addTask(new Task(500,500, &task1));
  scheduler.addTask(new Task(500,100, &task2));
  scheduler.setupISR();
}

void loop(){
  scheduler.run();
}

// Richard todo:
// add the watchdog to the Time -> runTask function
