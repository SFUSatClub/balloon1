#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include "Task.h"

class Scheduler {
  private:
    uint32_t systemTick;

    uint8_t numCurrTasks;
    uint8_t numMaxTasks;
    Task *allTasks[];
  public:
    Scheduler(uint8_t numMaxTasks);
    bool addTask(Task *taskptr);
    void registerModulesAsTasks(Module **modules, int numModules);
    void setupISR();
    uint32_t getSystemTick();
    void run();
};


#endif // SCHEDULER_H

