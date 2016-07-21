#ifndef IMU_H
#define IMU_H

#include "Module.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.

class IMU: public Module { 
public:
	IMU();
	void begin();
	void tick();

	const char* dataToPersist();
	const char* getModuleName();
};

#endif // IMU_H
