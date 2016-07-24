#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "Module.h"
#include "Barometer.h"
#include "GPS.h"

class StateHandler: public Module {
	SystemState balloonState=SystemState::PRE_FLIGHT; 
	bool stateChanged;

	static const int SAVED_VALUES=3;    //number of values saved in array
	float altitude[SAVED_VALUES]={0};    //most recent stored in altitude[0]
	float speed[SAVED_VALUES]={0};
	float pressure[SAVED_VALUES]={0};     //for compiling
	static const int LANDED_SPEED_VARIES=0;	//note: data needed
	static const int LANDED_ALTIUDE_VARIES=0;   //note: data needed

	static const int VOLT_PIN=3; //something!!
	int voltLevel;
	static const int LOW_BATTERY_LEVEL=100; //something!

	Barometer *barometer;
	GPS *gps;

	bool deltaAltitude=false;
	bool deltaPressure=false;
	bool deltaSpeed=false;


public:
	StateHandler(Barometer *, GPS *);

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
