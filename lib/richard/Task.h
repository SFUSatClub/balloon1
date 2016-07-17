#ifndef TASK_H
#define TASK_H

#include <stdint.h>

class Task{
  private:
    //  uint32_t Time1 = *tickPtr;
    uint16_t timeOut;
    uint32_t runStart;
    typedef void (*funcPtr)(void); // create a type to contain the function pointer
    funcPtr pointedFunc; // pointedFunc is the variable with the function pointer
    void setRunStart(uint32_t runstart);

  public:
    // we need to poke at these as we're going through checking whether to run tasks or not, hence public
    uint32_t lastRun;
    uint32_t interval;

    Task(uint16_t timeout, uint32_t Interval, funcPtr setFn); // constructor
    void setLastRun(uint32_t lastrun);
    void runTask(uint32_t systemTick);
    void setFunc(funcPtr f);
};

#endif // TASK_H
