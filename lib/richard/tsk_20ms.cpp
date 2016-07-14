
#include "tsk_20ms.h"			// For this module's definitions
#include <arduino.h>

uint8_t trigger = 0;

void tsk_20ms(void)
{

  if(trigger == 1){
    digitalWrite(8, 1);
    trigger = 0;
  }
  else{
    digitalWrite(8, 0);
    trigger = 1;
  }
}
