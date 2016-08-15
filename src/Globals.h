#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>

void WATCHDOG_RESET();

extern ArduinoOutStream cout;
extern ArduinoInStream cin;

// https://github.com/greiman/SdFat-beta/blob/master/SdFat/src/FreeStack.h
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

/* int getFreeRam(); */

#endif // GLOBALS_H

