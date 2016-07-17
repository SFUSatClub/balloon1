// #include "task1.h"
#include "tasks.h"
static int trigger = 0;

void task1(void){
// cli();  // interrupts off
  if(trigger == 1){
    digitalWrite(12, 1);
    trigger = 0;
    // Serial.println("On");
  }
  else{
    digitalWrite(12, 0);
    trigger = 1;
    // Serial.println("Off");

  }
  // sei();
}
