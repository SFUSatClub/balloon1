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
#include "schedule.h" // deals with all the scheduling stuff (except ISR)
#include "tasks.h"  // header for all of the task functions
#include "ISR.h"  // keeping interrupt service routines here since there may be quite a few of them

void setup(){
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  tickConfig(); // configures the master tick (1kHz)
}

// setup the tasks, figure out a nicer way to do this
const uint8_t numTasks = 2;
Task *allTasks[numTasks] = {
  new Task(500,500, &task1),
  new Task(500,100, &task2)
};

void loop(){
  uint32_t tickNow =  getSystemTick();

  for(uint8_t TaskIndex = 0; TaskIndex < numTasks; TaskIndex++){

    if(allTasks[TaskIndex]->interval == 0){  // run continuous tasks
        allTasks[TaskIndex]->runTask();
    }

    else if((tickNow - allTasks[TaskIndex]->lastRun) >= allTasks[TaskIndex]->interval && allTasks[TaskIndex]->lastRun < tickNow){
        allTasks[TaskIndex]->runTask();         // Execute Task
        allTasks[TaskIndex]->setLastRun(tickNow);  // Save last tick the task was ran.
    }
  }// end for
} // loop

// Richard todo:
// add the watchdog to the Time -> runTask function
