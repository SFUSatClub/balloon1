#include "IMU.h"


IMU::IMU() {
	imuImpl2 = new MPU6050();
	toWrite[0] = 0;
}

void IMU::begin() {
	imuImpl2->initialize();
	if(imuImpl2->testConnection()) {
		cout << "IMU2 success" << endl;
		state = State::BEGIN_SUCCESS;
	} else {
		cout << "IMU2 failed" << endl;
		state = State::BEGIN_FAILED;
	}
	if(state == State::BEGIN_SUCCESS) {
		cout << "IMU2 Initializing DMP" << endl;
		int retStat = imuImpl2->dmpInitialize();
		// offsets to be adjusted
		imuImpl2->setXGyroOffset(220);
		imuImpl2->setYGyroOffset(76);
		imuImpl2->setZGyroOffset(-85);
		imuImpl2->setZAccelOffset(1788);
		if(retStat == 0) {
			cout << "IMU2 Enabling DMP" << endl;
			imuImpl2->setDMPEnabled(true);

			state = State::BEGIN_SUCCESS;

			packetSize = imuImpl2->dmpGetFIFOPacketSize();
			cout << "IMU2 Ready for first interrupt" << endl;
		} else {
			// ERROR!
			// 1 = initial memory load failed
			// 2 = DMP configuration updates failed
			// (if it's going to break, usually the code will be 1)
			cout << "IMU2 Initialization failed: " << retStat << endl;
			state = State::BEGIN_FAILED;
		}
	}
}

void IMU::tick(){
	if(state == State::BEGIN_FAILED) {
		return;
	}

	// reset interrupt flag and get INT_STATUS byte
	mpuIntStatus = imuImpl2->getIntStatus();

	// get current FIFO count
	fifoCount = imuImpl2->getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		// reset so we can continue cleanly
		imuImpl2->resetFIFO();
		//Serial.println(F("FIFO overflow!"));

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
	} else if (mpuIntStatus & 0x02) {
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = imuImpl2->getFIFOCount();

		// read a packet from FIFO
		imuImpl2->getFIFOBytes(fifoBuffer, packetSize);

		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		imuImpl2->dmpGetAccel(&aa, fifoBuffer);
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		imuImpl2->dmpGetEuler(euler, &q);
		imuImpl2->dmpGetGravity(&gravity, &q);
		imuImpl2->dmpGetYawPitchRoll(ypr, &q, &gravity);
		imuImpl2->dmpGetLinearAccel(&aaReal, &aa, &gravity);
		imuImpl2->dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

		if(currSample <= SAMPLE_RATE_HZ && toWriteIndex < BUFFER_SIZE) {
			for(size_t i = 0; i < 3; i++) {
				euler[i] *= 180/M_PI;
				ypr[i] *= 180/M_PI;
			}
			if(currSample == 0) {
				toWrite[0] = '\0';
			}

			toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - toWriteIndex
					, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d\n"
					, euler[0], euler[1], euler[2]
					, ypr[0], ypr[1], ypr[2]
					, aaReal.x, aaReal.y, aaReal.z);

			currSample++;
		} else {
			cout << F("IMU BUFFER OVERFLOW!!!") << endl;
		}

#ifdef OUTPUT_READABLE_QUATERNION
		// display quaternion values in easy matrix form: w x y z
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		Serial.print("quat\t");
		Serial.print(q.w);
		Serial.print("\t");
		Serial.print(q.x);
		Serial.print("\t");
		Serial.print(q.y);
		Serial.print("\t");
		Serial.println(q.z);
#endif

#ifdef OUTPUT_READABLE_EULER
		// display Euler angles in degrees
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		imuImpl2->dmpGetEuler(euler, &q);
		Serial.print("euler\t");
		Serial.print(euler[0] * 180/M_PI);
		Serial.print("\t");
		Serial.print(euler[1] * 180/M_PI);
		Serial.print("\t");
		Serial.println(euler[2] * 180/M_PI);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
		// display Euler angles in degrees
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		imuImpl2->dmpGetGravity(&gravity, &q);
		imuImpl2->dmpGetYawPitchRoll(ypr, &q, &gravity);
		Serial.print("ypr\t");
		Serial.print(ypr[0] * 180/M_PI);
		Serial.print("\t");
		Serial.print(ypr[1] * 180/M_PI);
		Serial.print("\t");
		Serial.println(ypr[2] * 180/M_PI);
#endif

#ifdef OUTPUT_READABLE_REALACCEL
		// display real acceleration, adjusted to remove gravity
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		imuImpl2->dmpGetAccel(&aa, fifoBuffer);
		imuImpl2->dmpGetGravity(&gravity, &q);
		imuImpl2->dmpGetLinearAccel(&aaReal, &aa, &gravity);
		Serial.print("areal\t");
		Serial.print(aaReal.x);
		Serial.print("\t");
		Serial.print(aaReal.y);
		Serial.print("\t");
		Serial.println(aaReal.z);
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
		// display initial world-frame acceleration, adjusted to remove gravity
		// and rotated based on known orientation from quaternion
		imuImpl2->dmpGetQuaternion(&q, fifoBuffer);
		imuImpl2->dmpGetAccel(&aa, fifoBuffer);
		imuImpl2->dmpGetGravity(&gravity, &q);
		imuImpl2->dmpGetLinearAccel(&aaReal, &aa, &gravity);
		imuImpl2->dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
		Serial.print("aworld\t");
		Serial.print(aaWorld.x);
		Serial.print("\t");
		Serial.print(aaWorld.y);
		Serial.print("\t");
		Serial.println(aaWorld.z);
#endif

#ifdef OUTPUT_TEAPOT
		// display quaternion values in InvenSense Teapot demo format:
		teapotPacket[2] = fifoBuffer[0];
		teapotPacket[3] = fifoBuffer[1];
		teapotPacket[4] = fifoBuffer[4];
		teapotPacket[5] = fifoBuffer[5];
		teapotPacket[6] = fifoBuffer[8];
		teapotPacket[7] = fifoBuffer[9];
		teapotPacket[8] = fifoBuffer[12];
		teapotPacket[9] = fifoBuffer[13];
		Serial.write(teapotPacket, 14);
		teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
#endif
	}
}

scheduling_freq IMU::getSchedulingFreq() {
	scheduling_freq ret;
	ret.valid = true;
	ret.timeout = 1000;
	// ret.interval = 20hz = 1000/20 = 50ms
	ret.interval = 1000/SAMPLE_RATE_HZ;
	return ret;
}

const char* IMU::getModuleName() {
	return "IMU";
}

// Data in format <eulerX>,<eulerY>,<eulerZ>,<yaw>,<pitch>,<roll>,<accX>,<accY>,<accZ>
const char* IMU::dataToPersist() {
	currSample = 0;
	toWriteIndex = 0;
	return toWrite;
}

