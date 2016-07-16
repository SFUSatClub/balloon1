#include <stdint.h>

class Task{
private:
  //  uint16_t timeOut;
  //  uint16_t runStart;
  typedef void (*funcPtr)(void); // create a type to contain the function pointer
  funcPtr pointedFunc; // pointedFunc is the variable with the function pointer

public:
  // we need to poke at these as we're going through checking whether to run tasks or not, hence public
   uint32_t lastRun;
   uint16_t interval;

  // Task(); // default constructor
  Task(uint16_t Interval); // constructor
  void setLastRun(uint32_t lastrun);
// void setRunStart(uint16_t runstart);
  void runTask();
  void setFunc(funcPtr f);
};



//
// CFoo() : pointedFunc( NULL ) {}
//   typedef int (*funcType)(int, int);
//   funcType pointedFunc;
