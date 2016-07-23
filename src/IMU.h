#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <Adafruit_LSM303_U.h>
#include "Module.h"

class IMU: public Module {
  Adafruit_LSM303_Mag_Unified  *imuImpl;
	uint32_t timer;
  int dataAccelerometer[3];
  int dataMagnetometer[3];
  char toWrite[200];
public:
	IMU();
	void begin();
	void tick();
	int enable();
	void disable();

	const char* dataToPersist();
	const char* getModuleName();

  int getAccX();
  int getAccY();
  int getAccZ();
  int getMagX();
  int getMagY();
  int getMagZ();
};

#endif
