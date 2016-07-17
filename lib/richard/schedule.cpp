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
  this->setRunStart(getSystemTick());
  // bool functionDone = false;
  if (getSystemTick() != this->lastRun){  // prevents double running of functions

// put watchdog things here
      (*pointedFunc)(); // run the function from the poitner

  }
}
