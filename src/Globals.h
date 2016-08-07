#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>

void WATCHDOG_RESET();

extern ArduinoOutStream cout;
extern ArduinoInStream cin;

/* int getFreeRam(); */

#endif // GLOBALS_H

