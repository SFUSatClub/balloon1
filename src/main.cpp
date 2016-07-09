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
// #include <satScheduler.h>


ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
 tick ++; // increment our tick
// //generates pulse wave of frequency 2kHz/2 = 500Hz (takes two cycles for full wave- toggle high then toggle low)
//   if (toggle0){
//     digitalWrite(8,HIGH);
//     toggle0 = 0;
//   }
//   else{
//     digitalWrite(8,LOW);
//     toggle0 = 1;
//   }
}


uint32_t getSystemTick(){
  return tick;
}

void setup(){
  initAccelerometer(MPU_addr);
  tickConfig();

  Serial.begin(9600);

  pinMode(13, OUTPUT);
}
void loop(){
  // LEDControl(1);
  // int x = getVelocity(1);
  // x = x+1; // just so no warnings;

Serial.println(getSystemTick());

}
