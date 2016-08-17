#ifndef BAROMETER5540_H
#define BAROMETER5540_H

#include <Arduino.h>
#include "Module.h"
#include <SPI.h>


class Barometer5540: public Module {
  const uint8_t clockPin = 9; // Richard: this will depend on where we set up the timer that will toggle this pin. 32.768 khz (for atmega 328)
  static int32_t coefficients[6]; // the coefficients we'll be getting
  static float barometerData[2];  // temperature_degc, pressure_mbar

  void barometerReset();
  void getCoefficients();
  void setupClock(); // the barometer requires us to provide a 32.768kHz clock for its ADC

public:
	void begin();
	void tick();

	float getTemperature();
	float getPressure();

	const char* flushPersistBuffer();
	const char* getModuleName();
};

#endif // BAROMETER5540_H
