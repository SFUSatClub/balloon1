#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Module.h"

//#define SD_DEBUG

class SDCard: public Module {
	File dataFile;
	int chipSelectPin;
	int numModules;
	Module **modules;
	SdFat SD;

	static const uint32_t BUFFER_WRITE_SIZE = 2048;
	char buffer[BUFFER_WRITE_SIZE];

	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

	void registerModules(Module *modules[], int numModules);
#ifdef SD_DEBUG
	void doSDTimingBenchmark();
	void runDiagnostics();
#endif // SD_DEBUG
};


#endif /* SDCARD_H */
