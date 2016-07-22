#ifndef IMU_H
#define IMU_H

#include "Module.h"

class IMU: public Module { 
	const int i2cAddr;
public:
	IMU(int i2cAddr);
	void begin();
	void tick();

	const char* dataToPersist();
	const char* getModuleName();
};

#endif // IMU_H
