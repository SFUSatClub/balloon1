#include "Buzzer.h"

Buzzer::Buzzer(int _buzzerPin)
	: buzzerPin(_buzzerPin)
	, enabledTime(0)
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
	enabledTime = millis();
	digitalWrite(buzzerPin, HIGH);
	return 0;
}

void Buzzer::disable()
{
	enabledTime = -1;
	digitalWrite(buzzerPin, LOW);
}

const char* Buzzer::dataToPersist() {
	if(enabledTime == 0) {
		return NULL;
	}
	if(enabledTime > 0) {
		enabledTime = 0;
		toWrite[0] = '\0';
		ultoa(enabledTime, toWrite, 10);
		strcat(toWrite, " enabled");
		return toWrite;
	} else { // enabledTime = -1
		enabledTime = 0;
		toWrite[0] = '\0';
		ultoa(enabledTime, toWrite, 10);
		strcat(toWrite, " disabled");
		return toWrite;
	}
}

const char* Buzzer::getModuleName() {
	return "Buzzer";
}

