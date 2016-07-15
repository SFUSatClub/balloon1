#include <stdint.h>


class Task{
private:
  static uint16_t timeOut;
 static  uint16_t lastRun;
  static uint16_t runStart;
  static uint16_t interval;
  // void (*Func)(void)
  typedef void (*funcType)(void); // create a type to contain the function pointer
  funcType pointedFunc; // pointedFunc is the variable with the function pointer

public:
  Task(uint16_t timeout, uint16_t Interval); // constructor
 void setLastRun(uint16_t lastrun);
void setRunStart(uint16_t runstart);
  void runTask();
  void setFunc(funcType);
};

//
// CFoo() : pointedFunc( NULL ) {}
//   typedef int (*funcType)(int, int);
//   funcType pointedFunc;
