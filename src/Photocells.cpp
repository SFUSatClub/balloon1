#include <Arduino.h>    
#include "Module.h"
#include "Photocells.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.


Photocells::Photocells(HardwareSerial* ser){
        serial=ser;
}

void Photocells::begin(){
        serial->begin(9600);
        delay(1000);
        timer=millis();
}

void Photocells::tick(){
        timer= millis();
        for (int i=0;i<CELLS;i++){
                photocellData[i]=analogRead(FIRST_PIN+i);
        }
        
}

int Photocells::enable(){
        return 0;
}

void Photocells::disable(){

}

//outputs in format <timestamp>, <sensor type>, <sort of sensor data (eg. pressure, sensorno)>, <value>\n
const char* Photocells::dataToPersist(){
        sprintf(toWrite,",photocell,,\n");
        for (int i=0;i<CELLS;i++){
                sprintf(strchr(toWrite,'\0'),",,%d,%d\n",i+1,photocellData[i]); //appends to output str
        }
        return toWrite;
}


