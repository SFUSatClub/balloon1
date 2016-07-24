#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "Module.h"

class Task{
  private:
    uint16_t timeOut;
    uint32_t runStart;
    uint32_t printTimer;
    typedef void (*funcPtr)(void); // create a type to contain the function pointer
    funcPtr pointedFunc; // pointedFunc is the variable with the function pointer
    Module *module;
    void setRunStart(uint32_t runstart);
  public:
    // we need to poke at these as we're going through checking whether to run tasks or not, hence public
    uint32_t lastRun;
    uint32_t interval;

    //
    Task(uint16_t timeout, uint32_t Interval, funcPtr setFn); // constructor
    Task(uint16_t timeout, uint32_t Interval, Module *module); // constructor
    void setLastRun(uint32_t lastrun);
    void runTask(uint32_t systemTick);
    void setFunc(funcPtr f);

    // propogates state changes to modules/tasks
    scheduling_freq onStateChanged(const SystemState &state);

    static const uint16_t DEFAULT_TIMEOUT = 100;
    static const uint16_t DEFAULT_INTERVAL = 1000;
};

#endif // TASK_H
