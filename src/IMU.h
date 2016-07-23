#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <LSM303.h>
#include "Module.h"

class IMU: public Module {
  LSM303 *imuImpl;
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
