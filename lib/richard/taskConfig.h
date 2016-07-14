// #include "newScheduler.h"  // contains some definitions
#include <stdint.h>
#include<arduino.h>


typedef struct{
  uint16_t Interval;
  uint32_t LastTick;
  void (*Func)(void);
}TaskType;

TaskType *task_getConfig(void);
uint8_t task_getNumTasks(void);

void task1(void);
void task2();
// void task3(void);
