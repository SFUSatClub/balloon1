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
}

void Photocells::begin(){
	// empty
}

void Photocells::tick(){
	for (int i=0;i<NUM_CELLS;i++){
		photocellData[i]=analogRead(FIRST_PIN+i);
	}
}

int Photocells::enable(){
	return 0;
}

void Photocells::disable(){

}

// SD formatter adds moduleName and \n
// outputs in format <value1>, ... ,<value4>,<value5>
const char* Photocells::dataToPersist(){
	toWrite[0] = '\0';
	for (int i=0;i<NUM_CELLS;i++){
		sprintf(strchr(toWrite,'\0'), "%d,", photocellData[i]); //appends to output str
	}
	return toWrite;
}

const char* Photocells::getModuleName() {
	return "Photocells";
}

