#include <SoftwareSerial.h>

//soft UART pins
const byte rxPin = 2;
const byte txPin = 3;

//speed for serial communication in bits/sec
const int BaudRate = 9600;

//set up a gps connected to pins
SoftwareSerial gps(rxPin, txPin);

void setup()
{
	//set the data rate for the gps
	gps.begin(BaudRate);
	//enables data to be recieved from gps
	gps.listen();
}

void loop()
{
	//Read a single character recieved from gps
	//returns -1 if no data available
	char gpsData = gps.read();	
}
