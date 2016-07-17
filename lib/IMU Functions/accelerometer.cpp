// Richard: This function sets up the MPU 6050 accelerometer. Run once or after accelerometer power cycle.
#include "IMU.h"

void initAccelerometer(int MPU_addr){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

// GET ACCELEROMETER DATA
float Data [3] = {};
float accelScale = 0.00059816; // 9.8/16387.5 (get into g and then m/s/s)

float* getAccel(int MPU_addr){

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  Data[0] = (Wire.read()<<8|Wire.read())*accelScale;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  Data[1] = (Wire.read()<<8|Wire.read())*accelScale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  Data[2] = (Wire.read()<<8|Wire.read())*accelScale;  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  return Data;  // comes back as a pointer
  // Tmp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  // GyX = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  // GyY = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  // GyZ = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}


// PRINT ACCELEROMETER DATA
float AcX, AcY, AcZ;

void printAccel(float accel [3]){
  AcX = accel[0];
  AcY = accel[1];
  AcZ = accel[2];

  Serial.print("AcX = "); Serial.print(AcX,6);  // let's see this to 6 decimal places :)
  Serial.print(" | AcY = "); Serial.print(AcY,6);
  Serial.print(" | AcZ = "); Serial.print(AcZ,6);
  Serial.print("\n");
}

// GET VELOCITY DATA
static float VxO = 0;
static float Vx = 0;
static float dt = 0.001;

// Richard: gets the velocity in a direction (1=x, 2=y, 3=z)

float getVelocity(int Direction){

for (int i = 0; i < 10; i++){
  float *accel = getAccel(MPU_addr);
  float Ax = abs(accel[Direction - 1]); // current x acceleration m/s/s

  Vx = VxO + dt*Ax;
  VxO = Vx;
  delay(1);
}
VxO = 0;

// print it out
Serial.print(Vx, 6);  // metres per second
Serial.print('\n');
return Vx;
}
