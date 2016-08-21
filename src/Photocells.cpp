#include <Arduino.h>
#include "Module.h"
#include "Photocells.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports
//first sensor attached to A1, second attached to A2, etc.
Photocells::Photocells(uint8_t _firstPin, uint8_t _numCells)
	: FIRST_PIN(_firstPin)
	, NUM_CELLS(_numCells)
{
	photocellData = new int[NUM_CELLS];

	freq.interval = 1000/20;
	// 20hz * (4 char num + 3 commas + 1 nl)
	// = 160
	// = lets use 200
	allocatePersistBuffer(200);
}

void Photocells::begin(){
	// empty
}

void Photocells::tick(){
	analogReadResolution(12);

	char lineBuffer[16];
	int lineBufferIdx = 0;
	for(int i=0; i<NUM_CELLS; i++) {
		photocellData[i]=analogRead(FIRST_PIN+i);
		// if is last iteration, skip the comma, add new line
		lineBufferIdx += sprintf(lineBuffer + lineBufferIdx, i==NUM_CELLS-1?"%d\n":"%d,", photocellData[i]); //appends to output str
	}

	persistBufferIndex += snprintf(persistBuffer + persistBufferIndex
			, persistBufferSize - persistBufferIndex
			, "%s" , lineBuffer);

	analogReadResolution(10);
}

// TODO: use persistBufferIndex instead of strchr for speed
// outputs in format <value1>, ... ,<value4>,<value5>
const char* Photocells::flushPersistBuffer(){
	persistBufferIndex = 0;
	return persistBuffer;
}

const char* Photocells::getModuleName() {
	return "Photocells";
}
