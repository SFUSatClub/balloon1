#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"

<<<<<<< HEAD
//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.

class Photocells: public Module { 
        HardwareSerial *serial;
        static const int CELLS=5;      //number of photocells
        static const int FIRST_PIN=0;  //first photosensor is attached to pin x
        int photocellData[CELLS];      //data acquired from sensor (assuming int datatype?)
        char toWrite[200];             //max buffer size may have to change depending on data
        unsigned long timer;       
    
public:
        Photocells(HardwareSerial*);
        void begin();
        void tick();
        int enable();
        void disable();
        const char* dataToPersist();
=======
//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports
//first sensor attached to A1, second attached to A2, etc.

class Photocells: public Module {
	const int FIRST_PIN;  //first photosensor is attached to pin x
	const int NUM_CELLS;      //number of photocells
	int *photocellData;      //data acquired from sensor (assuming int datatype?)
	char toWrite[200];             //max buffer size may have to change depending on data
	unsigned long timer;
public:
	Photocells(uint8_t firstPin, uint8_t numCell);
	void begin();
	void tick();

	const char* dataToPersist();
	const char* getModuleName();
>>>>>>> Steven
};
