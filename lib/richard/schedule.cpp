#include "schedule.h"

Task::Task(uint16_t timeout, uint16_t Interval){
  timeOut = timeout;
  lastRun = 0;
  runStart = 0;
  interval = Interval;
  pointedFunc =  nullptr; // pointedFunc(nullptr)
}


void Task::setLastRun(uint16_t lastrun){
  lastRun = lastrun;
}

 void Task::setRunStart(uint16_t runstart){
  runStart = runstart;
}


void Task::setFunc( funcType f ) {
     pointedFunc = f;
}

void Task::runTask(){
(*pointedFunc)(); // run the function from the poitner
}
