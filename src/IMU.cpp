#include "IMU.h"

IMU::IMU() {
	imuImpl = new LSM303();
	imuImpl2 = new MPU6050();
}

void IMU::begin() {
	imuImpl->init();
	imuImpl->enableDefault();
	/* last_status = Wire.endTransmission() */
	/* 0:success */
	/* 1:data too long to fit in transmit buffer */
	/* 2:received NACK on transmit of address
	 * 	<- if not connected, last_status will be 2 and imuImpl->read() in IMU::tick() will BLOCK */
	/* 3:received NACK on transmit of data */
	/* 4:other error */
	if(imuImpl->last_status == 0) {
		cout << "IMU success" << endl;
		state = State::BEGIN_SUCCESS;
	} else {
		cout << "IMU failed" << endl;
		state = State::BEGIN_FAILED;
	}

	imuImpl2->initialize();
	if(imuImpl2->testConnection()) {
		cout << "IMU2 success" << endl;
		state = State::BEGIN_SUCCESS;
	} else {
		cout << "IMU2 failed" << endl;
		state = State::BEGIN_FAILED;
	}
}

void IMU::tick(){
	if(state == State::BEGIN_FAILED) {
		return;
	}
	imuImpl->read();
	char response[80];
	dataAccelerometer[0] = imuImpl->a.x;
	dataAccelerometer[1] = imuImpl->a.y;
	dataAccelerometer[2] = imuImpl->a.z;
	dataMagnetometer[0] = imuImpl->m.x;
	dataMagnetometer[1] = imuImpl->m.y;
	dataMagnetometer[2] = imuImpl->m.z;
	snprintf(response, 80, "A: %6d %6d %6d    M: %6d %6d %6d",
			imuImpl->a.x, imuImpl->a.y, imuImpl->a.z,
			imuImpl->m.x, imuImpl->m.y, imuImpl->m.z);
	cout << "Resp: " << response << endl;
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
	int written = 0;
	for (int i=0;i<3;i++){
		written += snprintf(toWrite + written, 100 - written, "%d,", dataAccelerometer[i]); //appends to output str
	}
	for (int i=0;i<3;i++){
		written += snprintf(toWrite + written, 100 - written, "%d,", dataMagnetometer[i]); //appends to output str
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
