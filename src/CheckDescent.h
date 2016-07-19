#include <Arduino.h>    //assuming inclusion of string.h
#include "Module.h"
#include "GPS.h"
#include "IMU.h"

//Checks whether balloon is descending. Aim to call every 3-5 cycles

class CheckDescent: public Module { 
    HardwareSerial *serial;
    IMU *theIMU;
    GPS *theGPS;
    static const int SAVED_VALUES=3;    //number of values saved in array
    int altitude[SAVED_VALUES]={0};    //most recent stored in altitude[0]
    int pressure[SAVED_VALUES]={0};     //for compiling
    bool deltaAltitude=false;
    bool deltaPressure=false;
    unsigned long timer;
    static const int CYCLES=3;      //run every x CYCLES       
    bool descending=false;   //RETURN value for function

public:
    CheckDescent(HardwareSerial*,IMU*,GPS*);
    void begin();
    void tick();
    int enable();
    void disable();
    const char* dataToPersist();
    bool isDescending();
};
