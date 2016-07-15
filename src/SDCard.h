#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Module.h"

class SDCard: public Module {
	File dataFile;
	int chipSelectPin;
	Module **modules;
	uint32_t timer;
public:
	SDCard(const int cs);
	void begin();
	void tick();
	int enable();
	void disable();

	void runDiagnostics();
	void registerModules(Module* modules[]);
};

#endif /* SDCARD_H */
