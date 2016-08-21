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
	uint32_t startTime;
	uint32_t endTime;
	bool shouldWriteToSD;

	char **modulesPersistCache;
	int *modulesPersistCacheIdx;

	static const int BUFFER_WRITE_SIZE = 2048;
	char buffer[BUFFER_WRITE_SIZE];

	bool doWrite(const char* file, const char* data);
	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
	void disable();

	const char* getModuleName();
	const char* flushPersistBuffer();

	void registerSeed(long seed);
	void registerModules(Module *modules[], int numModules);
	void registerScheduler(Scheduler *scheduler);
#ifdef SD_DEBUG
	void doSDTimingBenchmark();
	void runDiagnostics();
#endif // SD_DEBUG
};


#endif /* SDCARD_H */
