#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include "tasks.h"

extern uint32_t Mytick;
void tickConfig(void);  // initializes timer 2 on the 328
// staticuint32_t tickPtr = &tick;

extern uint32_t getSystemTick();

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
  void runTask();
  void setFunc(funcPtr f);
};



//
// CFoo() : pointedFunc( NULL ) {}
//   typedef int (*funcType)(int, int);
//   funcType pointedFunc;
#endif // SCHEDULE_H
