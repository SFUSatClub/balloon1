#include <stdint.h>
#include "tasks.h"

static volatile uint32_t tick = 0;
void tickConfig(void);  // initializes timer 2 on the 328

class Task{
private:
   uint16_t timeOut;
   uint32_t runStart;
  typedef void (*funcPtr)(void); // create a type to contain the function pointer
  funcPtr pointedFunc; // pointedFunc is the variable with the function pointer

public:
  // we need to poke at these as we're going through checking whether to run tasks or not, hence public
    uint32_t lastRun;
   uint32_t interval;

  Task(uint16_t timeout, uint32_t Interval, funcPtr setFn); // constructor
 void setLastRun(uint32_t lastrun);
void setRunStart(uint32_t runstart);
  void runTask();
  void setFunc(funcPtr f);
};



//
// CFoo() : pointedFunc( NULL ) {}
//   typedef int (*funcType)(int, int);
//   funcType pointedFunc;
