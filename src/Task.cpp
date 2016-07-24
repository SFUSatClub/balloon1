#include "Task.h"

Task::Task(uint16_t timeout, uint32_t Interval, funcPtr setFn){
  timeOut = timeout;
  lastRun = 0;
  runStart = 0;
  interval = Interval;
  pointedFunc =  setFn; // pointedFunc(nullptr)
  module = NULL;
  printTimer = 0;
}

Task::Task(uint16_t timeout, uint32_t Interval, Module *_module) {
  timeOut = timeout;
  lastRun = 0;
  runStart = 0;
  interval = Interval;
  pointedFunc = NULL;
  module = _module;
  printTimer = 0;
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

void Task::runTask(uint32_t systemTick){
  setRunStart(systemTick);
  if (systemTick != lastRun){  // prevents double running of functions
    uint32_t ms = millis();
    uint32_t mins = (ms / 1000.0 ) / 60.0;
    uint32_t secs = (ms / 1000 ) % 60;
    // put watchdog things here
    char timeBuffer[12];
    snprintf(timeBuffer, 10, "%02d:%02d:%03d", mins, secs, ms % 1000);
    if(pointedFunc == NULL) {
      module->tick();
      bool okayToPrint = systemTick - printTimer > 1000;
      if(okayToPrint) {
        printTimer = systemTick;
        /* cout << mins << ":" << secs << ":" << ms % 1000 << "\t"; */
        cout << timeBuffer << "\t";
        cout << module->getModuleName() << "\t\t"
          << module->dataToPersist() << endl;
      }
    } else {
      (*pointedFunc)(); // run the function from the poitner
      bool okayToPrint = systemTick - printTimer > 1000;
      if(okayToPrint) {
        printTimer = systemTick;
        cout << timeBuffer << "\t";
        cout << "pointedFunc" << endl;
      }
    }
  }
  lastRun = systemTick;
}

scheduling_freq Task::onStateChanged(const SystemState &state) {
  if(module == NULL) {
    scheduling_freq ret;
    ret.valid = false;
    return ret;
  } else {
    return module->onStateChanged(state);
  }
}
