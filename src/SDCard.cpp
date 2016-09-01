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

	startTime = millis();
	// stop writing when 4 hours is hit
	endTime = 1000*60*60*4;
	shouldWriteToSD = true;
}

void SDCard::tick() {
	int t1 = millis();
	if(t1 - startTime > 1000*60*60*4) {
		shouldWriteToSD = false;
	}

	for(int currModule = 0; currModule < numModules; currModule++) {
		const char* moduleData = modules[currModule]->flushPersistBuffer();
		if(moduleData == NULL) {
			continue;
		}
		const char* moduleName = modules[currModule]->getModuleName();
		const int moduleDataLen = strlen(moduleData);

		const int availCache = 512 - modulesPersistCacheIdx[currModule];
		//cout << moduleName << " - cache: " << availCache << " idx: " << modulesPersistCache[currModule] << endl;
		// if length of module data is greater than saving to the cache twice, just do one big write
		if(moduleDataLen >= 512) {
			//cout << ":::ONE:::" << endl;
			// prepend the contents of what was in the cache before
			snprintf(buffer, BUFFER_WRITE_SIZE, "%s%s", modulesPersistCache[currModule], moduleData);
			doWrite(moduleName, buffer);
			modulesPersistCacheIdx[currModule] = 0;
			// if we can, cache all of the module's data so we save on a write
		} else if(availCache >= moduleDataLen) {
			//cout << ":::TWO:::" << endl;
			strncpy(modulesPersistCache[currModule] + modulesPersistCacheIdx[currModule]
				, moduleData, moduleDataLen);
			modulesPersistCacheIdx[currModule] += moduleDataLen;
			/* modulesPersistCache[currModule][modulesPersistCacheIdx[currModule] - 3] = 'X'; */
			/* modulesPersistCache[currModule][modulesPersistCacheIdx[currModule] - 2] = 'A'; */
			// if our avail cache cannot store all of the module's data...
		} else {
			//cout << ":::THREE:::" << endl;
			int offsetIfOverwritten = 0;
			int moduleDataOffset = 0;
			// ... but if cache is big enough to store at least 1 char from moduleData and null char
			if(availCache >= 2) {
				// leave 1 char free for null char
				strncpy(modulesPersistCache[currModule] + modulesPersistCacheIdx[currModule]
						, moduleData, availCache - 1);
				// 511 = modulesPersistCacheIdx[currModule] + availCache - 1
				modulesPersistCache[currModule][511] = '\0';
				/* modulesPersistCache[currModule][509] = 'X'; */
				/* modulesPersistCache[currModule][510] = 'I'; */
				offsetIfOverwritten = 1;
				moduleDataOffset = availCache - 1;
				// not needed since it will be 0'd later
				// modulesPersistCacheIdx[currModule] += availCache - 1;
			}
			//cout << ":::THREE:::1" << endl;
			// write and flush the contents of the cache
			doWrite(moduleName, modulesPersistCache[currModule]);
			modulesPersistCacheIdx[currModule] = 0;

			const int newAvailCache = 512;
			//cout << ":::THREE:::2" << endl;
			strncpy(modulesPersistCache[currModule], moduleData + moduleDataOffset, newAvailCache);
			/* modulesPersistCache[currModule][0] = 'X'; */
			/* modulesPersistCache[currModule][1] = 'C'; */
			modulesPersistCacheIdx[currModule] += moduleDataLen - moduleDataOffset;
			/* modulesPersistCacheIdx[currModule] += moduleDataLen - availCache + offsetIfOverwritten; */
			//cout << ":::THREE:::3" << endl;
		}
	}
	if(scheduler != NULL) {
		const char* schedulerData = scheduler->flushPersistBuffer();
		if(schedulerData != NULL) {
			doWrite("Scheduler", schedulerData);
		}
	}
#ifdef DEBUG
	PP(cout << "SD: writing this loop took " << millis() - t1 << "ms" << endl;)
#endif
}

void SDCard::disable() {
	dataFile.close();
}

const char* SDCard::getModuleName() {
	return "SDCard";
}
const char* SDCard::flushPersistBuffer() {
	return NULL;
}


void SDCard::registerSeed(long _seed) {
	seed = _seed;
}
void SDCard::registerModules(Module **_modules, int _numModules) {
	modules = _modules;
	numModules = _numModules;

	modulesPersistCache = new char*[_numModules];
	modulesPersistCacheIdx = new int[_numModules];

	char seedMarker[32];
	snprintf(seedMarker, 32, "\n::BOOT::%ld\n", seed);
#ifdef DEBUG
	cout << seedMarker;
#endif
	for(int currModule = 0; currModule < numModules; currModule++) {
		modulesPersistCache[currModule] = new char[512];
		modulesPersistCache[currModule][0] = '\0';
		modulesPersistCacheIdx[currModule] = 0;
		const char* moduleName = modules[currModule]->getModuleName();
		doWrite(moduleName, seedMarker);
	}
	return;
}
void SDCard::registerScheduler(Scheduler *_scheduler) {
	scheduler = _scheduler;

	char seedMarker[32];
	snprintf(seedMarker, 32, "::BOOT::%ld\n", seed);
	doWrite("Scheduler", seedMarker);

	return;
}

bool SDCard::doWrite(const char* file, const char* data) {
	if(state == State::BEGIN_SUCCESS && shouldWriteToSD) {
		if(switchToFile(file, FILE_WRITE)) {
			dataFile.write(data);
			return true;
		}
	}
	return false;
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
