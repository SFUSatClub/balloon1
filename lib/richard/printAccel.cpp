#include "Arduino.h"

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
