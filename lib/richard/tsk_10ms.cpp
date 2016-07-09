
#include "tsk_10ms.h"			// For this module's definitions
#include <arduino.h>

static int trigger = 0;

void tsk_10ms(void)
{

  if(trigger == 1){
    digitalWrite(13, 1);
    trigger = 0;
  }
  else{
    digitalWrite(13, 0);
    trigger = 1;
  }
}
