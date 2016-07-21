#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "Module.h"
#include "IMU.h"
#include "GPS.h"
#include "Battery.h"

class StateHandler: public Module {
	SystemState balloonState=PRE_FLIGHT;
	bool stateChanged;

    static const int SAVED_VALUES=3;    //number of values saved in array
    int altitude[SAVED_VALUES]={0};    //most recent stored in altitude[0]
    int pressure[SAVED_VALUES]={0};     //for compiling
    static const int LANDED_PRESSURE_VARIES=0;	//note: data needed
    static const int LANDED_ALTIUDE_VARIES=0;   //note: data needed

    IMU *imu;
    GPS *gps;
    Battery *battery;

    bool deltaAltitude=false;
    bool deltaPressure=false;


public:
	StateHandler(IMU *, GPS *, Battery*);

	void begin();
	void tick();

	const char* dataToPersist();
	const char* getModuleName();

	bool hasStateChanged();
	SystemState getSystemState();

	bool checkLanded();
    bool checkDescent();
    bool checkBattery();

};

#endif /* STATEHANDLER_H */
