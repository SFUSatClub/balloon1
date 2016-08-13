#ifndef THERMAL_H
#define THERMAL_H

#include <Arduino.h>
#include "Module.h"

class Thermal: public Module {
  double currentR;
  double currentTempC;
  static const int BUFFER_SIZE = 16;
  char toWrite[BUFFER_SIZE];
  const int THERMAL_PIN;
  // R2 in voltage divider used to caculate R1 (Thermistor R), in kOhms
  // Resistor needs to be changed if more precision is required in different temperature ranges
  // Ex: get a resistor with higher resistance to obtain bigger precision for negative temperatures
  const double R2;
  char name[20];
public:
  Thermal(int pin, double R, char const * id);
  void begin();
  void tick();

  const char* dataToPersist();
  const char* getModuleName();
};

#endif
