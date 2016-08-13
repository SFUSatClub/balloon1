#include "Globals.h"

void WATCHDOG_RESET() {
	WDT_Restart(WDT);
}

// Serial streams
ArduinoOutStream cout(Serial);

// input buffer for line
char cinBuf[40];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

/* int getFreeRam() { */
/*   extern int __heap_start, *__brkval; */
/*   int v; */
/*   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); */
/* } */
