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

void setup(){
  initAccelerometer(MPU_addr);


  Serial.begin(115200);
  pinMode(13, OUTPUT);
  tickConfig();
  Task_ptr = task_getConfig();    // Get a pointer to the task configuration

  Task task1(500,100);
  task1.setFunc((task1));

}


void loop(){
  tick =  getSystemTick();

  for(TaskIndex = 0; TaskIndex < NumTasks; TaskIndex++)
  {
    if(Task_ptr[TaskIndex].Interval == 0)
    {
      // Run continuous tasks.
      (*Task_ptr[TaskIndex].Func)();
    }
    else if((tick - Task_ptr[TaskIndex].LastTick) >= Task_ptr[TaskIndex].Interval) // Richard: add AND LastTick != current tick to prevent double running
    {
      (*Task_ptr[TaskIndex].Func)();         // Execute Task

      Task_ptr[TaskIndex].LastTick = tick;  // Save last tick the task was ran.
    }
  }// end for
  // LEDControl(1);
  // int x = getVelocity(1);
  // x = x+1; // just so no warnings;

// Serial.println(getSystemTick());

}
