#include "Task.h"

Task::Task(uint16_t timeout, uint32_t Interval, funcPtr setFn){
	timeOut = timeout;
	lastRun = 0;
	runStart = 0;
	interval = Interval;
	pointedFunc =  setFn; // pointedFunc(nullptr)
	module = NULL;
	printTimer = 0;
}

Task::Task(uint16_t timeout, uint32_t Interval, Module *_module) {
	timeOut = timeout;
	lastRun = 0;
	runStart = 0;
	interval = Interval;
	pointedFunc = NULL;
	module = _module;
	printTimer = 0;
}

void Task::setLastRun(uint32_t lastrun){
	lastRun = lastrun;
}

void Task::setRunStart(uint32_t runstart){
	runStart = runstart;
}


void Task::setFunc( funcPtr f ) {
	pointedFunc = f;
}

void Task::runTask(uint32_t systemTick){
	setRunStart(systemTick);
	if (systemTick != lastRun){  // prevents double running of functions
		uint32_t ms = millis();
		uint32_t mins = (ms / 1000.0 ) / 60.0;
		uint32_t secs = (ms / 1000 ) % 60;
		char buffer[16];
		snprintf(buffer, 16, "%02d:%02d:%03d\t", mins, secs, ms % 1000);
		if(pointedFunc == NULL) {
			module->tick();
			bool okayToPrint = systemTick - printTimer > 1000;
			if(okayToPrint) {
				printTimer = systemTick;
				// -12 = left align to fit 12 chars in field
				// right pad with spaces and tabulate for aligned columns
				snprintf(buffer, 16, "%-12s\t", module->getModuleName());
				cout << buffer << module->dataToPersist() << endl;
			}
		} else {
			(*pointedFunc)(); // run the function from the poitner
			bool okayToPrint = systemTick - printTimer > 1000;
			if(okayToPrint) {
				printTimer = systemTick;
				snprintf(buffer, 16, "%-12s\n", "pointedFunc");
				cout << buffer;
			}
		}
	}
	lastRun = systemTick;
}

scheduling_freq Task::onStateChanged(const SystemState &state) {
	if(module == NULL) {
		scheduling_freq ret;
		ret.valid = false;
		return ret;
	} else {
		return module->onStateChanged(state);
	}
}
