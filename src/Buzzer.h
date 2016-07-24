#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Module.h"

class Buzzer: public Module {
  int charLength;
  int buzzerPin;
public:
  Buzzer(int buzzerPin);
  //set up pin as an OUTPUT, return nothing
  void begin();

  void tick();
  int enable();
  void disable(); //?

  const char* dataToPersist();
  const char* getModuleName();

};

#endif // BUZZER_H
