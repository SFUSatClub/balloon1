#include <Arduino.h>    
#include "CheckDescent.h"

//implementation that treats all photocells as a single unit, provided that they are all connected to consecutive ports 
//first sensor attached to A1, second attached to A2, etc.

CheckDescent::CheckDescent(HardwareSerial* ser,IMU* imuunit,GPS* gpsunit){
    serial=ser;
    theIMU=imuunit;
    theGPS=gpsunit;
}

void CheckDescent::begin(){
    timer=millis();
    //altitude[0]=theGPS->getAltitude();
    //pressure[0]=theIMU->getPressure();
    //shouldn't need anything else, not hardware connected
}


void CheckDescent::tick(){
    // if millis() or timer wraps around, we'll just reset it
    if (timer > millis())  timer = millis();

    // approximately every x seconds or so, print out the current stats
    if (millis() - timer > CYCLES*1000) { 
        timer = millis(); // reset the timer


        //update value arrays
        for (int i=SAVED_VALUES-1;i>=1;i--){
            altitude[i]=altitude[i-1];
            pressure[i]=pressure[i-1];
        }
        //altitude[0]=theGPS->getAltitude();
        //pressure[0]=theIMU->getPressure();


        //calculate deltas
        deltaPressure=true; //assume true, check for a false case
        deltaAltitude=true; //assume true, check for a false case
        for (int i=SAVED_VALUES-1;i>=1;i--){
            if (pressure[i]>pressure[i-1])  deltaPressure=false;
            if (altitude[i]<altitude[i-1])  deltaAltitude=false;
        } 

        descending=(deltaAltitude && deltaPressure); 
    }

}



bool CheckDescent::isDescending(){
    return descending;
}


int CheckDescent::enable(){
    return 0;
}

void CheckDescent::disable(){

}

