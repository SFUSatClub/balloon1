#include "Module.h"


// Steven: Bitwise operators to use State enum as flags
inline Module::State operator|(Module::State a, Module::State b) {
	return static_cast<Module::State>(static_cast<int>(a) | static_cast<int>(b));
}
inline Module::State operator &(Module::State a, Module::State b) {
	return static_cast<Module::State>(static_cast<int>(a) & static_cast<int>(b));
}
inline Module::State& operator |=(Module::State& a, Module::State b) {
	return a = a | b;
}

Module::Module() {
	// default module timeout, interval
	freq.timeout = 1000;
	freq.interval = 1000;
}

int Module::enable() {
	return 0;
}

void Module::disable() {
	return;
}

void Module::onStateChanged(const SystemState &state) {
	return;
}

scheduling_freq Module::getSchedulingFreq() {
	return freq;
}

const char* Module::dataToPersist() {
	return NULL;
}

const char* Module::dataToSend() {
	return NULL;
}

const char* Module::getModuleName() {
	return "Module";
}

bool Module::shouldTick(uint32_t currSystemTick) {
	// continuous modules should always be ticked
	if(freq.interval == 0) {
		return true;
	}

	bool shouldModuleRun = (currSystemTick - lastSystemTick) >= freq.interval;
	// if modules in this current system tick all finish early (within the
	// current tick), Scheduler::run() will execute many times. Without this check,
	// this module will also be run more than once
	bool moduleDidNotRunYet = lastSystemTick < currSystemTick;

#ifdef DEBUG
	// if the print property is set, module should run,
	// and 1 second has passed since last print
	if(propertyShouldPrint && shouldModuleRun && moduleDidNotRunYet
			&& currSystemTick - lastPrintTick > 1000) {
		lastPrintTick = currSystemTick;
		const uint32_t ms = millis();
		const int mins = (ms / 1000.0 ) / 60.0;
		const int secs = (ms / 1000 ) % 60;
		char buffer[32];
		snprintf(buffer, 32, "%02d:%02d:%03d\t%-12s\t", mins, secs, ms % 1000, getModuleName());
		cout << buffer;
		// -12 = left align to fit 12 chars in field
		// right pad with spaces and tabulate for aligned columns
		snprintf(buffer, 16, "%-12s\t", getModuleName());
		cout << buffer << dataToPersist() << endl;
		return true;
	}
#endif

	return shouldModuleRun && moduleDidNotRunYet;
}

void Module::setTicked(uint32_t currSystemTick) {
	lastSystemTick = currSystemTick;
}

