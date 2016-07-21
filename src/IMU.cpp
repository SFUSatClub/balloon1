#include "IMU.h"

IMU::IMU() {

}

void IMU::begin(){

}

void IMU::tick(){

}

// SD formatter adds moduleName and \n
// outputs in format <value1>, ... ,<value4>,<value5>
const char* IMU::dataToPersist(){
	return NULL;
}

const char* IMU::getModuleName() {
	return "IMU";
}

