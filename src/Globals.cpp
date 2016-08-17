#include "Globals.h"

void WATCHDOG_RESET() {
	WDT_Restart(WDT);
}

// Serial streams
ArduinoOutStream cout(Serial);

// input buffer for line
char cinBuf[40];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

int FreeStack() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
/* // http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c */
/* void genRandStr(char *s, const int len) { */
/*     static const char alphanum[] = */
/*         "0123456789" */
/*         "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
/*         "abcdefghijklmnopqrstuvwxyz"; */
/*     for (int i = 0; i < len; ++i) { */
/*         s[i] = alphanum[rand() % (sizeof(alphanum) - 1)]; */
/*     } */
/*     s[len] = 0; */
/* } */

/* int getFreeRam() { */
/*   extern int __heap_start, *__brkval; */
/*   int v; */
/*   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); */
/* } */
