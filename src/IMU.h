#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <LSM303.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Module.h"

#define OUTPUT_READABLE_EULER

class IMU: public Module {
private:
	LSM303  *imuImpl;
	MPU6050  *imuImpl2; // 0x68 default
	uint32_t timer;
	int extIntPin;
	int dataAccelerometer[3];
	int dataMagnetometer[3];
	char toWrite[150];

	uint8_t mpuIntStatus;
	uint16_t packetSize;
	uint16_t fifoCount;
	uint8_t fifoBuffer[64];

	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion dcontainer
	VectorInt16 aa;         // [x, y, z]            accel sensor measurements
	VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
	VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float euler[3];         // [psi, theta, phi]    Euler angle container
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

public:
	IMU(int IMUexternalInterruptPin);
	void begin();
	void tick();

	scheduling_freq getSchedulingFreq();
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
