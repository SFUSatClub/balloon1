#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "Module.h"
#include "IMU.h"
#include "GPS.h"

class StateHandler: public Module {
	IMU *imu;
	GPS *gps;
public:
	StateHandler(IMU *imu, GPS *gps);

	void begin();
	void tick();

	bool hasStateChanged();
	SystemState getSystemState();

	const char* dataToPersist();
	const char* getModuleName();
};

#endif /* STATEHANDLER_H */
