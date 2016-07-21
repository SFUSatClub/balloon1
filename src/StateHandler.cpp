#include "StateHandler.h"

StateHandler::StateHandler(IMU *_imu, GPS *_gps)
	: imu(_imu)
	, gps(_gps)
{

}

void StateHandler::begin(){

}

void StateHandler::tick(){

}

bool StateHandler::hasStateChanged() {
	return false;
}

SystemState StateHandler::getSystemState() {
	return SystemState::INVALID;
}

// SD formatter adds moduleName and \n
// outputs in format <value1>, ... ,<value4>,<value5>
const char* StateHandler::dataToPersist(){
	return NULL;
}

const char* StateHandler::getModuleName() {
	return "StateHandler";
}

