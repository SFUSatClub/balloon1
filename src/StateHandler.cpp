#include "StateHandler.h"

/* To Do :  needs checkBattery code --Matilda
   needs at-rest variance values for pressure/altitude
   is state logic represented correctly?
   correct getters for gps and IMU
   */


StateHandler::StateHandler(Barometer *_barometer, GPS *_gps)
	: barometer(_barometer)
	, gps(_gps)
{
	balloonState=SystemState::PRE_FLIGHT;   //start in preflight
	stateChanged=false;
}

void StateHandler::begin(){
	altitude[0]=gps->getAltitude();
	speed[0]=gps->getSpeed();
	pressure[0]=barometer->getPressure();
}

void StateHandler::tick(){
	stateChanged=false;
	//stateChanged should be modified in checker functions

	if (balloonState==SystemState::PRE_FLIGHT){
		balloonState=SystemState::DURING_FLIGHT; //only execute once
		stateChanged=true;
	}

	else if (balloonState==SystemState::DURING_FLIGHT){
		if (checkBattery())      balloonState=SystemState::LOW_BATTERY;
		else if (checkDescent()) balloonState=SystemState::DURING_DESCENT;
	}

	else if (balloonState==SystemState::DURING_DESCENT){
		if (checkBattery())     balloonState=SystemState::LOW_BATTERY;
		else if (checkLanded()) balloonState=SystemState::LANDED;
	}

	else if (balloonState==SystemState::LOW_BATTERY){
		if (checkLanded())      balloonState=SystemState::LANDED;
	}

	else if (balloonState==SystemState::LANDED){
		stateChanged=true; //stop calling statehandler after this
	}
}

bool StateHandler::hasStateChanged() {
	return stateChanged;
}

SystemState StateHandler::getSystemState() {
	return balloonState;
}

// returns current system state
const char* StateHandler::flushPersistBuffer(){
	switch(balloonState) {
		case SystemState::PRE_FLIGHT:
			return "PRE_FLIGHT";
		case SystemState::DURING_FLIGHT:
			return "DURING_FLIGHT";
		case SystemState::DURING_DESCENT:
			return "DURING_DESCENT";
		case SystemState::LANDED:
			return "LANDED";
		case SystemState::LOW_BATTERY:
			return "LOW_BATTERY";
		default: // fall-through
		case SystemState::INVALID:
			return "INVALID";
	}
}

const char* StateHandler::getModuleName() {
	return "StateHandler";
}

bool StateHandler::checkDescent(){
	//update value arrays
	for (int i=SAVED_VALUES-1;i>=1;i--){
		altitude[i]=altitude[i-1];
		pressure[i]=pressure[i-1];
	}
	altitude[0]=gps->getAltitude();
	pressure[0]=barometer->getPressure();

	//calculate deltas (assume true, check for contradiction)
	deltaPressure=true;
	deltaAltitude=true;
	for (int i=SAVED_VALUES-1;i>=1;i--){
		if (pressure[i]>pressure[i-1])  deltaPressure=false;
		if (altitude[i]<altitude[i-1])  deltaAltitude=false;
	}

	stateChanged= (deltaAltitude && deltaPressure);

	return stateChanged;
}

bool StateHandler::checkLanded(){
	//update value arrays
	for (int i=SAVED_VALUES-1;i>=1;i--){
		altitude[i]=altitude[i-1];
		speed[i]=speed[i-1];
	}
	altitude[0]=gps->getAltitude();
	speed[0]=gps->getSpeed();

	//calculate deltas (assume true, check for contradiction)
	deltaSpeed=true;
	deltaAltitude=true;
	for (int i=SAVED_VALUES-1;i>=1;i--){
		if ( abs(speed[i]-speed[i-1])<LANDED_SPEED_VARIES)              deltaSpeed=false;
		if ( abs(altitude[i]-altitude[i-1])<LANDED_ALTIUDE_VARIES)      deltaAltitude=false;
	} //needs to be drift-resistant

	stateChanged= (deltaAltitude && deltaSpeed);

	return stateChanged;
}

bool StateHandler::checkBattery(){
	voltLevel=analogRead(VOLT_PIN);
	if (voltLevel<LOW_BATTERY_LEVEL )  stateChanged=true; //3.5V *4
	return stateChanged;
}
