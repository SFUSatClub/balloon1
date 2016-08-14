#include "Task.h"

Task::Task(uint16_t timeout, uint32_t Interval, funcPtr setFn){
	timeOut = timeout;
	interval = Interval;
	lastRun = 0;
	runStart = 0;
	pointedFunc =  setFn; // pointedFunc(nullptr)
	module = NULL;
	printTimer = 0;

	propertyShouldPrint = true;
}

Task::Task(Module *_module) {
	scheduling_freq f = _module->getSchedulingFreq();
	timeOut = f.timeout;
	interval = f.interval;
	lastRun = 0;
	runStart = 0;
	pointedFunc = NULL;
	module = _module;
	printTimer = 0;

	propertyShouldPrint = true;
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
		if(pointedFunc == NULL) {
			module->tick();
			bool okayToPrint = systemTick - printTimer > 1000;
			if(okayToPrint && propertyShouldPrint) {
				printTimer = systemTick;
				uint32_t ms = millis();
				int mins = (ms / 1000.0 ) / 60.0;
				int secs = (ms / 1000 ) % 60;
				char buffer[16];
				snprintf(buffer, 16, "%02d:%02d:%03d\t", mins, secs, (int)ms % 1000);
				cout << buffer;
				// -12 = left align to fit 12 chars in field
				// right pad with spaces and tabulate for aligned columns
				snprintf(buffer, 16, "%-12s\t", module->getModuleName());
				cout << buffer << module->dataToPersist() << endl;
			}
		} else {
			(*pointedFunc)(); // run the function from the poitner
			bool okayToPrint = systemTick - printTimer > 1000;
			if(okayToPrint && propertyShouldPrint) {
				printTimer = systemTick;
				uint32_t ms = millis();
				int mins = (ms / 1000.0 ) / 60.0;
				int secs = (ms / 1000 ) % 60;
				char buffer[16];
				snprintf(buffer, 16, "%02d:%02d:%03d\t", mins, secs, (int)ms % 1000);
				cout << buffer;
				snprintf(buffer, 16, "%-12s\n", "pointedFunc");
				cout << buffer;
			}
		}
	}
	lastRun = systemTick;
}

void Task::setPropertyPrint(bool _propertyShouldPrint) {
	propertyShouldPrint = _propertyShouldPrint;
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

