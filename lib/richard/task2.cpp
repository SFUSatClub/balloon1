// #include "task2.h"
#include "tasks.h"
static int trigger = 0;

void task2(){
  // Serial.println(*getAccel(MPU_addr));

  if(trigger == 1){
    digitalWrite(8, 1);
    trigger = 0;
  }
  else{
    digitalWrite(8, 0);
    trigger = 1;
  }
}
