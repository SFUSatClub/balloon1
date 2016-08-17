#ifndef THERMAL_H
#define THERMAL_H

#include <Arduino.h>
#include "Module.h"

class Thermal: public Module {
  double currentR;
  double currentTempC;
  const int THERMAL_PIN;
  // R2 in voltage divider used to caculate R1 (Thermistor R), in kOhms
  // Resistor needs to be changed if more precision is required in different temperature ranges
  // Ex: get a resistor with higher resistance to obtain bigger precision for negative temperatures
  const double R2;

  static const int NAME_SIZE = 16;
  char name[NAME_SIZE];
public:
  Thermal(int pin, double R, char const * id);
  void begin();
  void tick();

  const char* flushPersistBuffer();
  const char* getModuleName();
};

#endif
