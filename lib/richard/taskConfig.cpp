// include the header files for each task you want to run
// #include "task1.h"
// #include "task2.h"
#include "tasks.h"
#include "taskConfig.h"
// #include "newScheduler.h"

// config table for the scheduler. Task interval, last time executed, function
static TaskType Tasks[] = {
{1000,0,task1},
{100,0,task2}

};

uint8_t taskNumber = sizeof(Tasks) / sizeof(*Tasks);

TaskType *task_getConfig(void){
  return Tasks;
}

uint8_t task_getNumTasks(void){
  return sizeof(Tasks) / sizeof(*Tasks);
}
