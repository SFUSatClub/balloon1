#include "schedule.h"
#include <Arduino.h>
// Sets up timer 0 for 1kHzx

#if defined(__AVR_ATmega328P__)

void tickConfig(void){
  cli();//disable interrupts

  //set timer0 interrupt at 1kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 251;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    sei();//enable interrupts

}
#endif

#if defined (__arm__)

void tickConfig(void){
  pmc_set_writeprotect(false);		 // disable write protection for pmc registers
pmc_enable_periph_clk(ID_TC7);	 // enable peripheral clock TC7

/* we want wavesel 01 with RC */
TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
TC_SetRC(TC2, 1, 656);  // 1 kHz
TC_Start(TC2, 1);

// enable timer interrupts on the timer
TC2->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;   // IER = interrupt enable register
TC2->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;  // IDR = interrupt disable register

/* Enable the interrupt in the nested vector interrupt controller */
/* TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1 */
NVIC_EnableIRQ(TC7_IRQn);
}
#endif
