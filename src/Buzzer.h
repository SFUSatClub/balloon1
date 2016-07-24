#include <Arduino.h>
#include "Module.h"

class Buzzer: public Module {
int charLength;
const int buzzerPin=13;
Buzzer(void);
public:
  //set up pin as an OUTPUT, return nothing
void begin();

void tick();
int enable();
void disable(); //?

const char* dataToPersist();
const char* getModuleName();

};
