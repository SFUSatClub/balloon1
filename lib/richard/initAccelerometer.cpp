#include "I2Cdev.h" // Richard: PlatformIO lib #113 - install this instead of Wire library, works the same.

// Richard: This function sets up the MPU 6050 accelerometer. Run once or after accelerometer power cycle. 

void initAccelerometer(int MPU_addr){

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
