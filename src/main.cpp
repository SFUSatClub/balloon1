// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain

// Richard Notes for SFUSat:
// Simple 6050 IMU read example. See archive.txt for a more complicated example that sort of works, but is meant for a different board.
// More info: https://github.com/jrowberg/i2cdevlib/blob/master/Arduino/MPU6050/MPU6050.cpp - from SparkFun product page
// Connections: SDA = analog 4
//              SCL  = analog 5
//              3.3V power!
// This example is from: http://playground.arduino.cc/Main/MPU-6050#short

#include <arduino.h> //Richard: required for platformIO
#include "I2Cdev.h" // Richard: PlatformIO lib #113 - install this instead of Wire library, works the same.
#include <richard.h>
#include "taskConfig.h"
#include "schedule.h"

// Task variables
static TaskType *Task_ptr;                 		// Task pointer
static uint8_t TaskIndex = 0;					// Task index
const uint8_t NumTasks = task_getNumTasks();		// Number of tasks
 // Task *allTasks[1];
 // Task allTasks[2];


// static int toggle0 = 1;

ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
 tick ++; // increment our tick
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


uint32_t getSystemTick(){
  return tick;
}

void task3(){
  static int trigger = 0;

    if(trigger == 1){
      digitalWrite(13, 1);
      trigger = 0;
    }
    else{
      digitalWrite(13, 0);
      trigger = 1;
    }
}
void task4(){
  static int trigger = 0;

    if(trigger == 1){
      digitalWrite(8, 1);
      trigger = 0;
    }
    else{
      digitalWrite(8, 0);
      trigger = 1;
    }
}

void setup(){
  initAccelerometer(MPU_addr);


  Serial.begin(115200);
  pinMode(13, OUTPUT);
  tickConfig();
  Task_ptr = task_getConfig();    // Get a pointer to the task configuration




  // Task task3P(500,500, &task3);
  // task3P.setFunc(&task3);
  // Task task4P(500,500);
  // task4P.setFunc(&task4);

// allTasks[0] = new Task(500,500,&task3);
// Task allTasks[1] = {task3P, task4P};
}
Task allTasks[2] = {
    Task(500,500,&task3),
    Task(500,100,&task4)
  };

void loop(){

  tick =  getSystemTick();
uint8_t numTasks = 2;

  for(TaskIndex = 0; TaskIndex < numTasks; TaskIndex++)
  {
    if(allTasks[TaskIndex].interval == 0)
    {
      // Run continuous tasks.
      allTasks[TaskIndex].runTask();
    }
    else if((tick - allTasks[TaskIndex].lastRun) >= allTasks[TaskIndex].interval && allTasks[TaskIndex].lastRun < tick) // Richard: add AND LastTick != current tick to prevent double running
    {
      allTasks[TaskIndex].runTask();         // Execute Task

      allTasks[TaskIndex].setLastRun(tick);  // Save last tick the task was ran.
    }
  }// end for
  // LEDControl(1);
  // int x = getVelocity(1);
  // x = x+1; // just so no warnings;

// Serial.println(getSystemTick());

}

// Richard todo:
// clean everything up
// be able to add more objects to the array so multitasking works
// make sure tasks have access to the tick pointer - or implement another timer that resets on each task
// add stability checks to the task class
// set the last run variable within the .runTask function and things like that
// figure out why it stops after a while (overflow somewhere?)
