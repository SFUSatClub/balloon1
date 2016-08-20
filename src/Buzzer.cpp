#include "Buzzer.h"

int Buzzer::flipFlop;

Buzzer::Buzzer(int _buzzerPin)
	: buzzerPin(_buzzerPin)
	, enabledTime(0)
{
	charLength = 4;
	flipFlop = 0;

	allocatePersistBuffer(16);
}

void Buzzer::tick()
{
	enabledTime = millis();

	if(flipFlop == 0){
		digitalWrite(buzzerPin, HIGH);
		flipFlop = 1;
	} else {
		digitalWrite(buzzerPin, LOW);
		flipFlop = 0;
	}
}

void Buzzer::begin()
{
	flipFlop = 0;
	pinMode(buzzerPin, OUTPUT);
}

// int Buzzer::enable()
// {
// 	// enabledTime = millis();
// 	// flipFlop = 0;
// 	// // digitalWrite(buzzerPin, HIGH);
// 	// return 0;
// }
//
// void Buzzer::disable()
// {
// 	// enabledTime = -1;
// 	// digitalWrite(buzzerPin, LOW);
// }

const char* Buzzer::flushPersistBuffer() {
	if(enabledTime == 0) {
		return NULL;
	}
	if(flipFlop == 0) {
		// enabledTime = 0;
		persistBuffer[0] = '\0';
		ultoa(enabledTime, persistBuffer, 10);
		strcat(persistBuffer, " enabled\n");
		return persistBuffer;
	} else { // enabledTime = -1
		enabledTime = 0;
		persistBuffer[0] = '\0';
		ultoa(enabledTime, persistBuffer, 10);
		strcat(persistBuffer, " disabled\n");
		return persistBuffer;
	}
}

const char* Buzzer::getModuleName() {
	return "Buzzer";
}
