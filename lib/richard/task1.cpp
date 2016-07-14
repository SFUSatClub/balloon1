#include "task1.h"

static int trigger = 0;

void task1(void){
  if(trigger == 1){ 
    digitalWrite(13, 1);
    trigger = 0;
  }
  else{
    digitalWrite(13, 0);
    trigger = 1;
  }
}
