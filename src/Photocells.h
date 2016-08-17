#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports
//first sensor attached to A1, second attached to A2, etc.

class Photocells: public Module {
	const int FIRST_PIN;  //first photosensor is attached to pin x
	const int NUM_CELLS;      //number of photocells
	int *photocellData;      //data acquired from sensor (assuming int datatype?)
	unsigned long timer;
public:
	Photocells(uint8_t firstPin, uint8_t numCell);
	void begin();
	void tick();

	const char* flushPersistBuffer();
	const char* getModuleName();
};
