#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.

class Photocells: public Module { 
        HardwareSerial *serial;
        static const int CELLS=5;      //number of photocells
        static const int FIRST_PIN=0;  //first photosensor is attached to pin x
        int photocellData[CELLS];      //data acquired from sensor (assuming int datatype?)
        
    
public:
        Photocells(HardwareSerial*);
        void begin();
        void tick();
        int enable();
        void disable();
        const char* dataToPersist();
};