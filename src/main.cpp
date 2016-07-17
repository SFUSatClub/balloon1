// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain

// Richard Notes for SFUSat:
// Simple 6050 IMU read example. See archive.txt for a more complicated example that sort of works, but is meant for a different board.
// More info: github.com/jrowberg/i2cdevlib/blob/master/Arduino/MPU6050/MPU6050.cpp - from SparkFun product page
// Connections: SDA = analog 4
//              SCL  = analog 5
//              3.3V power!
// This example is from: playground.arduino.cc/Main/MPU-6050#short

#include <arduino.h> //Richard: required for platformIO
#include <IMU.h>
#include "schedule.h"
#include "tasks.h"

ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
 Mytick ++; // increment our tick
//generates pulse wave of frequency 2kHz/2 = 500Hz (takes two cycles for full wave- toggle high then toggle low)
  // if (toggle0){
  //   // digitalWrite(8,HIGH);
  //   bitWrite(PORTB, 0, 1);
  //   toggle0 = 0;
  // }
  // else{
  //   // digitalWrite(8,LOW);
  //   bitWrite(PORTB, 0, 0);
  //
  //   toggle0 = 1;
  // }
}
//
void setup(){
  // initAccelerometer(0x86);
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  tickConfig();
}

 
const uint8_t numTasks = 2;
Task *allTasks[numTasks] = {
    new Task(500,500, &task1)
  , new Task(500,100, &task2)
};

void loop(){
    uint32_t tickNow =  getSystemTick();

    for(uint8_t TaskIndex = 0; TaskIndex < numTasks; TaskIndex++)
    {
      if(allTasks[TaskIndex]->interval == 0)
      {
        // Run continuous tasks.
        allTasks[TaskIndex]->runTask();
      }
      else if((tickNow - allTasks[TaskIndex]->lastRun) >= allTasks[TaskIndex]->interval && allTasks[TaskIndex]->lastRun < tickNow) // Richard: add AND LastTick != current tick to prevent double running
      {
        allTasks[TaskIndex]->runTask();         // Execute Task

        allTasks[TaskIndex]->setLastRun(tickNow);  // Save last tick the task was ran.
      }
    }// end for

}

// Richard todo:
// make sure tasks have access to the tick pointer - or implement another timer that resets on each task
// add stability checks to the task class
// set the last run variable within the .runTask function and things like that
// figure out why it stops after a while (overflow somewhere?)
