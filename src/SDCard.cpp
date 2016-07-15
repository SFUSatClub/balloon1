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

// Steven: diagnostics code from SdFat "QuickStart" example
void SDCard::runDiagnostics() {
  // Serial streams
  ArduinoOutStream cout(Serial);

  // input buffer for line
  char cinBuf[40];
  ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

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

  cout << F("\nEnter the chip select pin number: ");
  while (!Serial.available()) {}
  delay(400);  // catch Due restart problem

  cin.readline();
  if (cin >> chipSelectPin) {
    cout << chipSelectPin << endl;
  } else {
    cout << F("\nInvalid pin number\n");
    return;
  }
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

