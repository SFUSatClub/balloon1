#include "IMU.h"

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
