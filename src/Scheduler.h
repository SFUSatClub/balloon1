#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include "Task.h"
#include "StateHandler.h"
#include "Globals.h"

class Scheduler {
private:
	int numCurrTasks;
	int numMaxTasks;
	Task **allTasks;

	int toWriteIndex;
	static const int BUFFER_SIZE = 256;
	char toWrite[BUFFER_SIZE];

	StateHandler *stateHandler;
public:
	Scheduler(uint8_t numMaxTasks);
	bool addTask(Task *taskptr);
	void registerModules(Module **modules, int numModules);
	void registerStateHandler(StateHandler *stateHandler);
	void setupISR();
	uint32_t getSystemTick();
	void run();
	const char* dataToPersist();

	static const uint16_t TICKS_PER_CYCLE = 1000;
};

#endif // SCHEDULER_H

