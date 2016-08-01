#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Module.h"

class SDCard: public Module {
	File dataFile;
	int chipSelectPin;
	int numModules;
	Module **modules;
	uint32_t timer;
	SdFat SD;

	const int BUFFER_WRITE_SIZE;
	char buffer[512];

	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
	int enable();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

	void doSDTimingBenchmark();
	void runDiagnostics();
	void registerModules(Module *modules[], int numModules);
};


#endif /* SDCARD_H */

