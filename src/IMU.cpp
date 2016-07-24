#include "IMU.h"

IMU::IMU() {
  imuImpl = new LSM303();
}

void IMU::begin() {
  Serial.print("Begin IMU\n");
  imuImpl->init();
  imuImpl->enableDefault();
}

void IMU::tick(){
  char response[80];
  Serial.print("Tick IMU\n");
  return; // imuImpl->read() locks up due, watchdog resets
  imuImpl->read();
  dataAccelerometer[0] = imuImpl->a.x;
  dataAccelerometer[1] = imuImpl->a.y;
  dataAccelerometer[2] = imuImpl->a.z;
  dataMagnetometer[0] = imuImpl->a.x;
  dataMagnetometer[1] = imuImpl->a.y;
  dataMagnetometer[2] = imuImpl->a.z;
  snprintf(response, sizeof(response), "A: %6d %6d %6d    M: %6d %6d %6d",
    imuImpl->a.x, imuImpl->a.y, imuImpl->a.z,
    imuImpl->m.x, imuImpl->m.y, imuImpl->m.z);
  Serial.println(response);
}

int IMU::enable() {
  return 0;
}

void IMU::disable() {

}

const char* IMU::getModuleName() {
  return "IMU";
}

// Data in format <accX>,<accY>,<accZ>,<magX>,<magY>,<magZ>
const char* IMU::dataToPersist() {
  toWrite[0] = '\0';
	for (int i=0;i<3;i++){
		sprintf(strchr(toWrite,'\0'), "%d,", dataAccelerometer[i]); //appends to output str
	}
  for (int i=0;i<3;i++){
    sprintf(strchr(toWrite,'\0'), "%d,", dataMagnetometer[i]); //appends to output str
  }
	return toWrite;
}

int IMU::getAccX() {
  return dataAccelerometer[0];
}
int IMU::getAccY() {
  return dataAccelerometer[1];
}
int IMU::getAccZ() {
  return dataAccelerometer[2];
}
int IMU::getMagX() {
  return dataMagnetometer[0];
}
int IMU::getMagY() {
  return dataMagnetometer[1];
}
int IMU::getMagZ() {
  return dataMagnetometer[2];
}
