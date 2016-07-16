#include "schedule.h"
#include "tasks.h"

Task::Task(uint16_t Interval){
  // timeOut = timeout;
  lastRun = 0;
  // runStart = 0;
  interval = Interval;
  pointedFunc =  nullptr; // pointedFunc(nullptr)
}


void Task::setLastRun(uint32_t lastrun){
  lastRun = lastrun;
}

//  void Task::setRunStart(uint16_t runstart){
//   runStart = runstart;
// }


void Task::setFunc( funcPtr f ) {
     pointedFunc = f;
}

void Task::runTask(){
  // set the run start and the last run time
  // run the function
  // check that timeout has not occurred
(*pointedFunc)(); // run the function from the poitner
}
