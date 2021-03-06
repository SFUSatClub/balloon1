#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Module.h"
#include "Scheduler.h"

//#define SD_DEBUG

class SDCard: public Module {
	File dataFile;
	int chipSelectPin;
	int numModules;
	Scheduler *scheduler;
	Module **modules;
	SdFat SD;
	long seed;

	static const int BUFFER_WRITE_SIZE = 2048;
	char buffer[BUFFER_WRITE_SIZE];

	/* static const int TO_WRITE_BUFFER_SIZE = 16; */
	/* char toWrite[TO_WRITE_BUFFER_SIZE]; */

	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

	void registerSeed(long seed);
	void registerModules(Module *modules[], int numModules);
	void registerScheduler(Scheduler *scheduler);
#ifdef SD_DEBUG
	void doSDTimingBenchmark();
	void runDiagnostics();
#endif // SD_DEBUG
};


#endif /* SDCARD_H */
