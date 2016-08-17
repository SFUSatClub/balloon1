#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>
#include "Config.h"

void WATCHDOG_RESET();

extern ArduinoOutStream cout;
extern ArduinoInStream cin;

// https://github.com/greiman/SdFat-beta/blob/master/SdFat/src/FreeStack.h
extern "C" char* sbrk(int incr);
int FreeStack();

/* int getFreeRam(); */

#endif // GLOBALS_H

