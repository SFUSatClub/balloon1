// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain

// Richard Notes for SFUSat:
// Simple 6050 IMU read example. See archive.txt for a more complicated example that sort of works, but is meant for a different board.
// More info: https://github.com/jrowberg/i2cdevlib/blob/master/Arduino/MPU6050/MPU6050.cpp - from SparkFun product page
// Connections: SDA = analog 4
//              SCL  = analog 5
//              3.3V power!
// This example is from: http://playground.arduino.cc/Main/MPU-6050#short

#include "arduino.h" //Richard: required for platformIO
#include "I2Cdev.h" // Richard: PlatformIO lib #113 - install this instead of Wire library, works the same.
#include <richard.h>
// #include<Wire.h>

int16_t Tmp,GyX,GyY,GyZ;
// float AcX,AcY,AcZ;
// float accelScale = 16383.5;
// const int MPU_addr=0x68;  // I2C address of the MPU-6050
// static float VxO = 0;
// static float Vx = 0;
// static float dt = 0.001;

void setup(){
  // Wire.begin();
  // Wire.beginTransmission(MPU_addr);
  // Wire.write(0x6B);  // PWR_MGMT_1 register
  // Wire.write(0);     // set to zero (wakes up the MPU-6050)
  // Wire.endTransmission(true);
  initAccelerometer(MPU_addr);
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}
void loop(){
  LEDControl(1);
  int x = getVelocity(1);
  x = x+1; // just so no warnings;

  // Wire.beginTransmission(MPU_addr);
  // Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  // Wire.endTransmission(false);
  // Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  // AcX = (Wire.read()<<8|Wire.read())/accelScale;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  // AcY = (Wire.read()<<8|Wire.read())/accelScale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  // AcZ = (Wire.read()<<8|Wire.read())/accelScale;  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  // Tmp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  // GyX = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  // GyY = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  // GyZ = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)



// float *accel = getAccel(MPU_addr);
// printAccel(accel);
  // AcX = accel[0];
  // AcY = accel[1];
  // AcZ = accel[2];
  //
  //
  // Serial.print("AcX = "); Serial.print(AcX,6);  // let's see this to 6 decimal places :)
  // Serial.print(" | AcY = "); Serial.print(AcY,6);
  // Serial.print(" | AcZ = "); Serial.print(AcZ,6);
  // Serial.print("\n");
  // Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  // doStuff(0);
  // Serial.print(" | GyX = "); Serial.print(GyX);
  // Serial.print(" | GyY = "); Serial.print(GyY);
  // Serial.print(" | GyZ = "); Serial.println(GyZ);


}
