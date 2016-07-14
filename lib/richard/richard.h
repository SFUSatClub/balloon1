#include "I2Cdev.h" // Richard: PlatformIO lib #113 - install this instead of Wire library, works the same.
#include <arduino.h> // required by platformIO
// #include "satScheduler.h"
// #include <tsk_cfg.h>


// #include <tsk_cfg.h>
// Constants
const int MPU_addr=0x68;  // address of the accelerometer
static volatile uint32_t tick = 0;

// Functions
void LEDControl(int);
void initAccelerometer(int);  // sets up the accelerometer
float* getAccel(int);  // return a pointer to the data array so we can get at it later
void printAccel(float*);  // prints accelerometer data
float getVelocity(int); // returns velocity in specified direction
// void tickConfig(void);  // initializes timer 2 on the 328
void tickConfig(void);  // initializes timer 2 on the 328
// uint32_t getSystemTick(void);
