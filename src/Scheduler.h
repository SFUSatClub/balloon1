#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include "Task.h"
#include "StateHandler.h"
#include "Globals.h"

class Scheduler {
private:
	uint32_t systemTick;

	int numCurrTasks;
	int numMaxTasks;
	Task **allTasks;

	StateHandler *stateHandler;
public:
	Scheduler(uint8_t numMaxTasks);
	bool addTask(Task *taskptr);
	void registerModulesAsTasks(Module **modules, int numModules);
	void registerStateHandler(StateHandler *stateHandler);
	void setupISR();
	uint32_t getSystemTick();
	void run();

	static const uint16_t TICKS_PER_CYCLE = 1000;
};

#endif // SCHEDULER_H

