#ifndef THERMAL_H
#define THERMAL_H

#include <Wire.h>
#include <Module.h>

class Thermal: public Module {
  double currentR;
  char toWrite[150];
  const int THERMAL_PIN;
public:
  Thermal(int pin);
  void begin();
  void tick();
  int enable();

  const char* dataToPersist();
  const char* getModuleName();

};

#endif
