#include "IMU.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.
IMU::IMU() {
}

void IMU::begin(){
	// empty
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

