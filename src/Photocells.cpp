#include <Arduino.h>    
#include "Module.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.


Photocells::Photocells(HardwareSerial* ser){
        serial=ser;
}

void Photocells::begin(){
        serial->begin(9600);
}

void Photocells::tick(){
        //needs: scheduling functionality needed, reads sensor data how frequently?

        for (int i=0;i<CELLS;i++){
                photocellData[i]=AnalogRead(FIRST_PIN+i);
        }
}

int Photocells::enable(){
        return 0;
}

void Photocells::disable(){

}

//outputs in format <timestamp>, <sensor type>, <sort of sensor data (eg. pressure, sensorno)>, <value>\n
const char* Photocells::dataToPersist(){
        char toWrite[200];      //max buffer size may have to change depending on data
        sprintf(toWrite,",photocell,,\n");
        for (int i=0;i<CELLS;i++){
                sprintf(strchr(toWrite,'\0'),",,%d,%d\n",i+1,photocellData[i]); //appends to output str
        }
        return toWrite;
}


