#include "Buzzer.h"

int Buzzer::flipFlop;

Buzzer::Buzzer(int _buzzerPin)
	: buzzerPin(_buzzerPin)
	, enabledTime(0)
{
	charLength = 4;
	flipFlop = 0;
}

void Buzzer::tick()
{
	enabledTime = millis();

if(flipFlop == 0){
	digitalWrite(buzzerPin, HIGH);
	flipFlop = 1;
}
else{
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

const char* Buzzer::dataToPersist() {
	if(enabledTime == 0) {
		return NULL;
	}
	if(flipFlop == 0) {
		// enabledTime = 0;
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
