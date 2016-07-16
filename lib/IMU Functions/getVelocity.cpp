 #include "IMU.h"

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
