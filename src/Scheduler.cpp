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
static uint32_t schedulerTick = 0;

Scheduler::Scheduler(uint8_t _numMaxTasks) {
	numMaxTasks = _numMaxTasks;
	numCurrTasks = 0;
	allTasks = new Task*[_numMaxTasks];
	for(int i = 0; i < numMaxTasks; i++) {
		allTasks[i] = NULL;
	}
	stateHandler = NULL;
	toWrite[0] = '\0';
}

void Scheduler::run() {
	uint32_t systemTick = schedulerTick;

	const uint32_t sysTickMod = systemTick % TICKS_PER_CYCLE;
	const bool isFirstTickOfCycle = sysTickMod == (TICKS_PER_CYCLE);
	const bool isNearEndOfCycle = sysTickMod >= (int)(TICKS_PER_CYCLE * 0.80);
	const bool isLastTickOfCycle = sysTickMod == (TICKS_PER_CYCLE - 1);

	bool hasStateChanged = false;
	bool alreadyRanFirstTickOfCycle = false;
	if(isFirstTickOfCycle && !alreadyRanFirstTickOfCycle) {
		alreadyRanFirstTickOfCycle = true;
		if(stateHandler != NULL) {
			stateHandler->tick();
			hasStateChanged = stateHandler->hasStateChanged();
		}
		toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - 1 - toWriteIndex, "t,%d\n", millis());
	}

	bool alreadyRanLastTickOfCycle = false;
	if(isLastTickOfCycle && !alreadyRanLastTickOfCycle) {
		alreadyRanLastTickOfCycle = true;
		/* cout << "RAM: " << getFreeRam() << endl; */
		/* cout << systemTick << ": is last tick of a cycle" << endl; */
	}

	for(int i = 0; i < numCurrTasks; i++) {
		Task *currTask = allTasks[i];
		if(currTask->interval == 0) {  // run continuous tasks
			currTask->runTask(schedulerTick);
		} else {
			bool shouldTaskRun = (schedulerTick - currTask->lastRun) >= currTask->interval;
			// if tasks in this current system tick all finish early (within the
			// current tick), Scheduler::run() will execute many times. Without this check,
			// these tasks will also be run more than once
			bool taskDidNotRunYet = currTask->lastRun < schedulerTick;

			if(shouldTaskRun && taskDidNotRunYet){
				if(isLastTickOfCycle && hasStateChanged && stateHandler != NULL) {
					scheduling_freq res = currTask->onStateChanged(stateHandler->getSystemState());
					res.valid = false; // to something with res to stop warning
				}
				uint32_t t1Sched = schedulerTick % TICKS_PER_CYCLE;
				currTask->runTask(schedulerTick); // Execute Task
				toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - 1 - toWriteIndex, "%d,%d,%d\n", i, t1Sched, (schedulerTick % TICKS_PER_CYCLE) - t1Sched);
			}
		}
	}
}

bool Scheduler::addTask(Task *taskptr) {
	if(numCurrTasks >= numMaxTasks) {
		delete taskptr;
		while(true) {
			Serial.println(F("SCHEDULER ERROR: CHECK NUMBER OF MAX TASKS"));
		}
		return false;
	}
	allTasks[numCurrTasks++] = taskptr;
	return true;
}


void Scheduler::registerModules(Module **modules, int numModules) {
	if(numCurrTasks + numModules > numMaxTasks) {
		while(true) {
			Serial.println(F("SCHEDULER ERROR: CHECK NUMBER OF MAX TASKS"));
		}
	}
	for(int currModule = 0; currModule < numModules; currModule++) {
		Module *currModulePtr = modules[currModule];
		Task *taskptr = new Task(currModulePtr);
		allTasks[numCurrTasks++] = taskptr;
		toWriteIndex += snprintf(toWrite + toWriteIndex, BUFFER_SIZE - 1 - toWriteIndex, "%d=%s\n", currModule, currModulePtr->getModuleName());
	}
	return;
}

void Scheduler::registerStateHandler(StateHandler *_stateHandler) {
	stateHandler = _stateHandler;
}

const char* Scheduler::dataToPersist() {
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

