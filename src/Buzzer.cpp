#include "Buzzer.h"

Buzzer::Buzzer(int _buzzerPin)
	: buzzerPin(_buzzerPin) 
{
	charLength = 4;
}

void Buzzer::tick()
{
	// Leave empty
}

void Buzzer::begin()
{
	pinMode(buzzerPin, OUTPUT);
}

int Buzzer::enable()
{
	digitalWrite(buzzerPin, HIGH);
	return 0;
}

void Buzzer::disable()
{
	digitalWrite(buzzerPin, LOW);
}

const char* Buzzer::dataToPersist() {
	char system_time[charLength];
	return ultoa(millis(),system_time,10);
}

const char* Buzzer::getModuleName() {
	return "Buzzer";
}

