#include "schedule.h"

schedule::schedule{ // set up the timer in the constructor ... kind of a bad idea :/
  cli();//disable interrupts
  //set timer0 interrupt at 1kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 251;// = (16*10^6) / (2000*64) - 1 (must be <256)
    TCCR0A |= (1 << WGM01);     // turn on CTC mode
    TCCR0B |= (1 << CS01) | (1 << CS00);     // Set CS01 and CS00 bits for 64 prescaler
    TIMSK0 |= (1 << OCIE0A);     // enable timer compare interrupt

    sei();//enable interrupts
}

// TaskType schedule::*task_getConfig(void){
//   return Tasks;
// }
//
// uint8_t schedule::task_getNumTasks(void){
//   return sizeof(Tasks) / sizeof(*Tasks);
// }
