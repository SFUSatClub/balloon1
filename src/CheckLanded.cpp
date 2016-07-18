#include <Arduino.h>    
#include "Module.h"
#include "CheckLanded.h"


CheckLanded::CheckLanded(HardwareSerial* ser,IMU* imuunit,GPS* gpsunit){
        serial=ser;
        theIMU=imuunit;
        theGPS=gpsunit;
}

void CheckLanded::begin(){
        timer=millis();
        altitude[0]=theGPS->getAltitude();
        pressure[0]=theIMU->getPressure();
        //shouldn't need anything else, not hardware connected
}


void CheckLanded::tick(){
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
                altitude[0]=theGPS->getAltitude();
                pressure[0]=theIMU->getPressure();


                //calculate deltas
                deltaPressure=true; //assume true, check for a false case
                deltaAltitude=true; //assume true, check for a false case
                for (int i=SAVED_VALUES-1;i>=1;i--){
                         
                        if ( abs(pressure[i]-pressure[i-1])<PRESSURE_VARIES)   deltaPressure=false;
                        if ( abs(altitude[i]-altitude[i-1])<ALTITUDE_VARIES )  deltaAltitude=false;
                } //needs to be drift-resistant 
               
                landed=(deltaAltitude && deltaPressure); 
        }

}



bool CheckLanded::isLanded(){
        return landed;
}


int CheckLanded::enable(){
        return 0;
}

void CheckLanded::disable(){

}

