#ifndef THERMAL_H
#define THERMAL_H

#include <Wire.h>
#include <Module.h>

class Thermal: public Module {
  double currentR;
  double currentTempC;
  char toWrite[10];
  const int THERMAL_PIN;
  // R2 in voltage divider used to caculate R1 (Thermistor R), in kOhms
  // Resistor needs to be changed if more precision is required in different temperature ranges
  // Ex: get a resistor with higher resistance to obtain bigger precision for negative temperatures
  const double R2;
  const char* name;
public:
  Thermal(int pin, double R, char const * id);
  void begin();
  void tick();

  const char* dataToPersist();
  const char* getModuleName();
};

#endif
