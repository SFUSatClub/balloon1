#include "SDCard.h"

// TODO: sd cards only deal in 512 byte blocks, so buffer 512 bytes before
// performing any writes to sd

SDCard::SDCard(int cs)
	: dataFile()
	, chipSelectPin(cs) 
{
}

void printDirectory(File dir, int numTabs);

void SDCard::begin() {
	if(SD.begin(chipSelectPin)) {
		Serial.println("SD Success");
		state = State::BEGIN_SUCCESS;
		
		File root = SD.open("/");
		printDirectory(root, 0);
		root.close();

		dataFile = SD.open("data.txt", FILE_WRITE);
	} else {
		Serial.println("SD Fail");
		state = State::BEGIN_FAILED;
	}
	timer = millis();
}

// Steven: TODO: iterate through each module and store that data
static int test = 0;
void SDCard::tick() {
	if(state == State::BEGIN_FAILED) {
		return;
	}
	test++;

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, do something with the sd card
	if (millis() - timer > 2000) { 
		timer = millis();
		// Steven: TODO: see how long each file open() and close() takes for science,
		// and if need be, only close/reopen when necessary
		Serial.print("opening data.txt in write mode...");
		dataFile = SD.open("data.txt", FILE_WRITE);
		if(dataFile) {
			Serial.println(" OK");
			String toWrite = "test_data: ";
			toWrite.concat(test);
			Serial.println("writing to sd card: " + toWrite);
			dataFile.println(toWrite);
			dataFile.close();
			Serial.println("closing file...");
		} else {
			Serial.println(" FAILED :(");
		}
		Serial.print("opening data.txt in read mode...");
		dataFile = SD.open("data.txt", FILE_READ);
		if(dataFile) {
			Serial.println(" OK");
			Serial.println("reading everything from the sd card...");
			while(dataFile.available()) {
				Serial.write(dataFile.read());
			}
			dataFile.close();
			Serial.println("closing file...");
		} else {
			Serial.println(" FAILED :(");
		}
	}
}

int SDCard::enable() {
	if(!dataFile) {
		dataFile = SD.open("data.txt", FILE_READ);
	}
	return 0;
}

void SDCard::disable() {
	dataFile.close();
}

void SDCard::registerModules(Module* _modules[]) {
	modules = _modules;
	return;
}

// Steven: Example code from Sparkfun to help troubleshoot SD card issues
void SDCard::runDiagnostics() {
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, 4)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }
}

// Steven: from arduino.cc
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
