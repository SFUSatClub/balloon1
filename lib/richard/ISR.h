#include "schedule.h"

#if defined (__SAM3X8E__) // ISR for Due board
  void TC7_Handler(){
    Mytick++;
    TC_GetStatus(TC2, 1);   // reading this register flips something to ensure the counter can be incremented again
}
#endif

#if defined(__AVR_ATmega328P__) // ISR when we're on the 328
  ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
    Mytick ++; // increment our tick
//generates pulse wave of frequency 2kHz/2 = 500Hz (takes two cycles for full wave- toggle high then toggle low)
  // if (toggle0){
  //   // digitalWrite(8,HIGH);
  //   bitWrite(PORTB, 0, 1);
  //   toggle0 = 0;
  // }
  // else{
  //   // digitalWrite(8,LOW);
  //   bitWrite(PORTB, 0, 0);
  //
  //   toggle0 = 1;
  // }
  }
#endif
