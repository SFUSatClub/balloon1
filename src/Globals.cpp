#include "Globals.h"

void WATCHDOG_RESET() {
  WDT_Restart(WDT);
}

// Serial streams
ArduinoOutStream cout(Serial);

// input buffer for line
char cinBuf[40];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));
