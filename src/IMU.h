#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <LSM303.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Module.h"

class IMU: public Module {
	LSM303  *imuImpl;
	MPU6050  *imuImpl2; // 0x68 default
	uint32_t timer;
	int dataAccelerometer[3];
	int dataMagnetometer[3];
	char toWrite[150];
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
