#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Module.h"

<<<<<<< HEAD
=======
//#define SD_DEBUG

>>>>>>> Steven
class SDCard: public Module {
	File dataFile;
	int chipSelectPin;
	int numModules;
	Module **modules;
<<<<<<< HEAD
	uint32_t timer;
	SdFat SD;

	const int BUFFER_WRITE_SIZE;
	char buffer[512];
=======
	SdFat SD;

	static const uint32_t BUFFER_WRITE_SIZE = 2048;
	char buffer[BUFFER_WRITE_SIZE];
>>>>>>> Steven

	bool switchToFile(const char* file, uint8_t flag);
public:
	SDCard(const int cs);
	void begin();
	void tick();
<<<<<<< HEAD
	int enable();
=======
>>>>>>> Steven
	void disable();

	const char* getModuleName();
	const char* dataToPersist();

<<<<<<< HEAD
	void doSDTimingBenchmark();
	void runDiagnostics();
	void registerModules(Module *modules[], int numModules);
=======
	void registerModules(Module *modules[], int numModules);
#ifdef SD_DEBUG
	void doSDTimingBenchmark();
	void runDiagnostics();
#endif // SD_DEBUG
>>>>>>> Steven
};


#endif /* SDCARD_H */
<<<<<<< HEAD

=======
>>>>>>> Steven
