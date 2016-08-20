#include <Arduino.h>
#include "Scheduler.h"

/*
 * The watchdog on the Arduino Due (SAM3X8E) can only be enabled/disabled
 * once, and the Arduino framework disables it by default, so first comment out
 * line 51 in
 *
 * balloon1/.pioenvs/due/FrameworkArduino/watchdog.cpp
 */

// doing it this way, there can only be one scheduler...
volatile static uint32_t schedulerTick = 0;

Scheduler::Scheduler() {
	stateHandler = NULL;
	toWrite[0] = '\0';
	alreadyRanFirstTickOfCycle = false;
	alreadyRanLastTickOfCycle = false;
}

void Scheduler::run() {
	uint32_t systemTick = schedulerTick;

	const uint32_t sysTickMod = systemTick % TICKS_PER_CYCLE;
	const bool isFirstTickOfCycle = sysTickMod == 0;
	const bool isNearEndOfCycle = sysTickMod >= (int)(TICKS_PER_CYCLE * 0.80);
	const bool isLastTickOfCycle = sysTickMod == (TICKS_PER_CYCLE - 1);

	bool hasStateChanged = false;
	if(isFirstTickOfCycle) {
		if(!alreadyRanFirstTickOfCycle) {
			alreadyRanFirstTickOfCycle = true;
			if(stateHandler != NULL) {
				stateHandler->tick();
				hasStateChanged = stateHandler->hasStateChanged();
			}
			toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - toWriteIndex, "t,%lu\n", millis());
			for(int i = 0; i < numModules; i++) {
				toWriteIndex += snprintf(toWrite + toWriteIndex
						, BUFFER_SIZE - toWriteIndex, "%d,%lu\n"
						, i, modulesTime[i]);
				modulesTime[i] = 0;
			}
#ifdef DEBUG
			cout << "New Cycle\n";
#endif // DEBUG
		}
	} else {
		alreadyRanFirstTickOfCycle = false;
	}

	if(isLastTickOfCycle) {
		if(!alreadyRanLastTickOfCycle) {
			alreadyRanLastTickOfCycle = true;
		}
	} else {
		alreadyRanLastTickOfCycle = false;
	}

	for(int i = 0; i < numModules; i++) {
		Module *currModule = modules[i];
		if(currModule->shouldTick(schedulerTick)) {
			const uint32_t schedulerTickBefore = schedulerTick;
			currModule->tick();
			const uint32_t schedulerTicksTaken = schedulerTick - schedulerTickBefore;
			modulesTime[i] += schedulerTicksTaken;
			currModule->setTicked(schedulerTickBefore);
			if(hasStateChanged && stateHandler != NULL) {
				currModule->onStateChanged(stateHandler->getSystemState());
			}
		}
	}
}

void Scheduler::registerModules(Module **_modules, int _numModules) {
	modules = _modules;
	numModules = _numModules;

	modulesTime = new uint32_t[_numModules];
	for(int i = 0; i < numModules; i++) {
		Module *currModule = modules[i];
		toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - toWriteIndex
				, "%d=%s\n", i, currModule->getModuleName());
		modulesTime[i] = 0;
	}

	return;
}

void Scheduler::registerStateHandler(StateHandler *_stateHandler) {
	stateHandler = _stateHandler;
}

const char* Scheduler::flushPersistBuffer() {
	toWriteIndex = 0;
	return toWrite;
}

#if defined(__AVR_ATmega328P__)
void Scheduler::setupISR() {
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

ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
	schedulerTick++; // increment our tick
}
#endif

#if defined (__arm__)
#define __WDP_MS 4095

void Scheduler::setupISR() {
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

	// enable the watchdog
	WDT_Enable(WDT, 0x2000 | __WDP_MS | ( __WDP_MS << 16));
}

void TC7_Handler(){
	schedulerTick++;
	TC_GetStatus(TC2, 1);   // reading this register flips something to ensure the counter can be incremented again
}
#endif

