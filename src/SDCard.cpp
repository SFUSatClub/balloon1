#include "SDCard.h"

SDCard::SDCard(int cs)
	: dataFile()
	, chipSelectPin(cs)
{
	scheduler = NULL;
}

void SDCard::begin() {
	if(SD.begin(chipSelectPin)) {
		Serial.println("SD Success");
		state = State::BEGIN_SUCCESS;
	} else {
		Serial.println("SD Fail");
		state = State::BEGIN_FAILED;
	}
}

void SDCard::tick() {
	if(state == State::BEGIN_FAILED) {
		return;
	}

	int t1 = millis();
	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleData = modules[currModule]->dataToPersist();
		if(moduleData == NULL) {
			continue;
		}
		const char* moduleName = modules[currModule]->getModuleName();
		const int moduleDataLen = strlen(moduleData);

		switchToFile(moduleName, FILE_WRITE);
		// TODO: benchmark difference between multiple writes vs strcat and 1 write
		if(moduleDataLen + 10 > BUFFER_WRITE_SIZE) {
			dataFile.write(moduleData);
			dataFile.write("\n");
		} else {
			buffer[0] = '\0';
			// TODO: multiple strcats is n^2 operation, keep track of index for speed
			/* sprintf(buffer, "%s\n". moduleData); */
			strcat(buffer, moduleData);
			strcat(buffer, "\n");
			dataFile.write(buffer);
		}
	}
	if(scheduler != NULL) {
		switchToFile("Scheduler", FILE_WRITE);
		const char* schedulerData = scheduler->dataToPersist();
		dataFile.write(schedulerData);
		dataFile.write("\n");
	}
	cout << "SD: writing this loop took " << millis() - t1 << "ms" << endl;
}

void SDCard::disable() {
	dataFile.close();
}

const char* SDCard::getModuleName() {
	return "SDCard";
}
const char* SDCard::dataToPersist() {
	return "returning sd data";
}


void SDCard::registerSeed(long _seed) {
	seed = _seed;
}
void SDCard::registerModules(Module **_modules, int _numModules) {
	modules = _modules;
	numModules = _numModules;

	char seedMarker[32];
	snprintf(seedMarker, 32, "::BOOT::%ld\n", seed);
	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleName = modules[currModule]->getModuleName();
		switchToFile(moduleName, FILE_WRITE);
		dataFile.write(seedMarker);
	}
	return;
}
void SDCard::registerScheduler(Scheduler *_scheduler) {
	scheduler = _scheduler;
	return;
}

bool SDCard::switchToFile(const char* file, uint8_t flag) {
	dataFile.close();
	dataFile = SD.open(file, flag);
	// Steven: returns if the file is opened successfully or not
	return dataFile;
}

#ifdef SD_DEBUG
void SDCard::doSDTimingBenchmark() {
	switchToFile("temp.txt", FILE_WRITE);

	const int numBytes = 512;
	char *temp = new char[numBytes];
	for(int i = 0; i < numBytes - 1; i++) {
		*(temp + i) = 'x';
	}
	*(temp + numBytes - 1) = '\0';

	unsigned long t1 = micros();
	dataFile.write(temp);
	dataFile.flush();
	unsigned long t2 = micros();
	cout << "writing and flushing " << numBytes << " bytes to sd card took: " << t2-t1 << " microseconds" <<  endl;

	switchToFile("temp.txt", FILE_READ);

	t1 = micros();
	while(dataFile.available()) {
		dataFile.read();
	}
	t2 = micros();
	cout << "reading " << dataFile.size() << " bytes from the sd card took: " << t2-t1 << " microseconds" <<  endl;

	delete[] temp;
}

// Steven: diagnostics code from SdFat "QuickStart" example
void SDCard::runDiagnostics() {

	// Set DISABLE_CHIP_SELECT to disable a second SPI device.
	// For example, with the Ethernet shield, set DISABLE_CHIP_SELECT
	// to 10 to disable the Ethernet controller.
	const int8_t DISABLE_CHIP_SELECT = -1;
	//
	// Test with reduced SPI speed for breadboards.
	// Change spiSpeed to SPI_FULL_SPEED for better performance
	// Use SPI_QUARTER_SPEED for even slower SPI bus speed
	const uint8_t spiSpeed = SPI_HALF_SPEED;

	bool firstTry = true;
	// read any existing Serial data
	while (Serial.read() >= 0) {}

	if (!firstTry) {
		cout << F("\nRestarting\n");
	}
	firstTry = false;

	if (DISABLE_CHIP_SELECT < 0) {
		cout << F(
				"\nAssuming the SD is the only SPI device.\n"
				"Edit DISABLE_CHIP_SELECT to disable another device.\n");
	} else {
		cout << F("\nDisabling SPI device on pin ");
		cout << int(DISABLE_CHIP_SELECT) << endl;
		pinMode(DISABLE_CHIP_SELECT, OUTPUT);
		digitalWrite(DISABLE_CHIP_SELECT, HIGH);
	}
	if (!SD.begin(chipSelectPin, spiSpeed)) {
		if (SD.card()->errorCode()) {
			cout << F(
					"\nSD initialization failed.\n"
					"Do not reformat the card!\n"
					"Is the card correctly inserted?\n"
					"Is chipSelectPin set to the correct value?\n"
					"Does another SPI device need to be disabled?\n"
					"Is there a wiring/soldering problem?\n");
			cout << F("\nerrorCode: ") << hex << showbase;
			cout << int(SD.card()->errorCode());
			cout << F(", errorData: ") << int(SD.card()->errorData());
			cout << dec << noshowbase << endl;
			return;
		}
		cout << F("\nCard successfully initialized.\n");
		if (SD.vol()->fatType() == 0) {
			cout << F("Can't find a valid FAT16/FAT32 partition.\n");
			cout << F("Check original reformatMsg()\n");
			return;
		}
		if (!SD.vwd()->isOpen()) {
			cout << F("Can't open root directory.\n");
			cout << F("Check original reformatMsg()\n");
			return;
		}
		cout << F("Can't determine error type\n");
		return;
	}
	cout << F("\nCard successfully initialized.\n");
	cout << endl;

	uint32_t size = SD.card()->cardSize();
	if (size == 0) {
		cout << F("Can't determine the card size.\n");
		cout << F("Check original cardOrSpeed()\n");
		return;
	}
	uint32_t sizeMB = 0.000512 * size + 0.5;
	cout << F("Card size: ") << sizeMB;
	cout << F(" MB (MB = 1,000,000 bytes)\n");
	cout << endl;
	cout << F("Volume is FAT") << int(SD.vol()->fatType());
	cout << F(", Cluster size (bytes): ") << 512L * SD.vol()->blocksPerCluster();
	cout << endl << endl;

	cout << F("Files found (date time size name):\n");
	SD.ls(LS_R | LS_DATE | LS_SIZE);

	if ((sizeMB > 1100 && SD.vol()->blocksPerCluster() < 64)
			|| (sizeMB < 2200 && SD.vol()->fatType() == 32)) {
		cout << F("\nThis card should be reformatted for best performance.\n");
		cout << F("Use a cluster size of 32 KB for cards larger than 1 GB.\n");
		cout << F("Only cards larger than 2 GB should be formatted FAT32.\n");
		cout << F("Check original reformatMsg()\n");
		return;
	}
	// read any existing Serial data
	while (Serial.read() >= 0) {}
	cout << F("\nSuccess!  Type any character to restart.\n");
	while (Serial.read() < 0) {}
}
#endif // SD_DEBUG
