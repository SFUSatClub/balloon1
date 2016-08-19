#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "Module.h"
#include "Barometer.h"
#include "GPS.h"
#include "Battery.h"
#include "IMU.h"
#include "SystemState.h"

class StateHandler: public Module {
	SystemState balloonState=SystemState::PRE_FLIGHT;
	bool stateChanged;

	static const int SAVED_VALUES=5;    //number of values saved in array
	float altitude[SAVED_VALUES]={0};    //most recent stored in altitude[0]
	float speed[SAVED_VALUES]={0};
	float pressure[SAVED_VALUES]={0};     //for compiling
	static const int LANDED_SPEED_VARIES=0;	//note: data needed
	static const int LANDED_ALTIUDE_VARIES=0;   //note: data needed

	// Values from IMU to determine if IMU has stabilized, which indicates readyness
	float IMUeulerX[SAVED_VALUES] = {0};

	static const int VOLT_PIN=3; //something!!
	int voltLevel;
	static const int LOW_BATTERY_LEVEL=100; //something!

	Barometer *barometer;
	GPS *gps;
	Battery *battery;
	IMU *imu;

	char *currentState;

	bool isImuStable = false;
	bool deltaAltitude=false;
	bool deltaPressure=false;
	bool deltaSpeed=false;


public:
	StateHandler(Barometer *, GPS *, Battery *_battery, IMU *_imu);

	void begin();
	void tick();

	const char* flushPersistBuffer();
	const char* getModuleName();

	bool hasStateChanged();
	SystemState getSystemState();

	bool checkReady();
	bool checkLanded();
	bool checkDescent();
	bool checkBattery();

};

#endif /* STATEHANDLER_H */
