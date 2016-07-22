#include "IMU.h"

IMU::IMU(int _i2cAddr)
	: i2cAddr(_i2cAddr)
{

}

void IMU::begin(){

}

void IMU::tick(){

}

const char* IMU::dataToPersist(){
	return NULL;
}

const char* IMU::getModuleName() {
	return "IMU";
}

