// #include "task2.h"
#include "tasks.h"
static int trigger = 0;

void task2(uint8_t thing){
  Serial.println(*getAccel(MPU_addr));
Serial.println(thing)
  if(trigger == 1){
    digitalWrite(8, 1);
    trigger = 0;
  }
  else{
    digitalWrite(8, 0);
    trigger = 1;
  }
}
