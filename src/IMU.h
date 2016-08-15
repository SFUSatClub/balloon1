#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Module.h"

// NOTE: if changing DMP sample rate, change last hex value in line ~305 of
// MPU6050_6Axis_MotionApps20.h
// DMP output frequency calculated with: (200Hz / (1 + value))
// We want sample rate of 20Hz (lets make it 25 to be safe), so we'll use 0x07

class IMU: public Module {
private:
	MPU6050  *imuImpl; // 0x68 default
	uint32_t timer;

	// assuming worst case widths (eg, 180 for each float, 65536 for each int)
	// 36 chars = 6 floats * 6 chars (3 whole numbers, 1 decimal point, 2 precision) = xxx.xx
	// 15 chars = 3 ints * 5 chars (16 bit ints) = 65536 = xxxxx
	// 20Hz * (36 chars + 15 chars + 1 new line) = 1040 chars
	static const int SAMPLE_RATE_HZ = 20;
	static const int BUFFER_SIZE = (SAMPLE_RATE_HZ + 4) * (36 + 15 + 1); // extra buffer room (sample rate + 4) just in case
	uint8_t currSample;
	int toWriteIndex;
	char toWrite[BUFFER_SIZE];

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
	IMU();
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
