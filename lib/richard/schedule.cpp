#include "schedule.h"
#include <arduino.h>
// #include "tasks.h"
uint32_t Mytick = 0;
uint32_t getSystemTick(){
  return Mytick;
}

Task::Task(uint16_t timeout, uint32_t Interval, funcPtr setFn){
  timeOut = timeout;
  lastRun = 0;
  runStart = 0;
  interval = Interval;
  pointedFunc =  setFn; // pointedFunc(nullptr)
}


void Task::setLastRun(uint32_t lastrun){
  lastRun = lastrun;
}

 void Task::setRunStart(uint32_t runstart){
  runStart = runstart;
}


void Task::setFunc( funcPtr f ) {
     pointedFunc = f;
}

void Task::runTask(){
  if (getSystemTick() != this->lastRun){
    (*pointedFunc)(); // run the function from the poitner

  }
  // uint32_t thing = getSystemTick();
  // Serial.println(thing);



  // thing = getSystemTick();
  // set the run start and the last run time
  // run the function
  // check that timeout has not occurred
}
