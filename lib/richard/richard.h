#include "I2Cdev.h" // Richard: PlatformIO lib #113 - install this instead of Wire library, works the same.
#include <arduino.h> // required by platformIO

// Constants
const int MPU_addr=0x68;  // address of the accelerometer

// Functions
void LEDControl(int);
void initAccelerometer(int);  // sets up the accelerometer
float* getAccel(int);  // return a pointer to the data array so we can get at it later
void printAccel(float*);  // prints accelerometer data
float getVelocity(int); // returns velocity in specified direction
