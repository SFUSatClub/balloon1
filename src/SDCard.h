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
	SdFat SD;

	static const uint32_t BUFFER_WRITE_SIZE = 512;
	char buffer[BUFFER_WRITE_SIZE];

	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

	void doSDTimingBenchmark();
	void runDiagnostics();
	void registerModules(Module *modules[], int numModules);
};


#endif /* SDCARD_H */
