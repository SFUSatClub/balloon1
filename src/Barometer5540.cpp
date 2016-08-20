#include "Barometer5540.h"

/*
   MS5540C Miniature Barometer Module
   This program will read your MS5440C or compatible pressure sensor every 5 seconds and show you the calibration words, the calibration factors,
   the raw values and the compensated values of temperature and pressure.
   Once you read out the calibration factors you can define them in the header of any sketch you write for the sensor.

   Pins (for '328 based arduino):
   MS5540 sensor attached to pins 10 - 13:
   MOSI: pin 11
   MISO: pin 12
   SCK: pin 13
   MCLK: pin 9 (or use external clock generator on 32kHz)
   CS is not in use, but might be pin 10
   Richard Arthurs, July 31 2016 - based on work by MiGeRA
*/
void Barometer5540::begin(){
	Serial.begin(9600);
	SPI.begin(); //see SPI library details on arduino.cc for details
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV32); //divide 16 MHz to communicate on 500 kHz // Richard: for the due, we divide by 168...?? Is this possible?
	setupClock(); // sets up the clock signal we must provide the barometer's ADC
	getCoefficients();

	freq.interval = 500;

	allocatePersistBuffer(150);
}

void Barometer5540::barometerReset() //we often reset the barometer, so may as well have a function for it
{
	SPI.setDataMode(SPI_MODE0);
	SPI.transfer(0x15);
	SPI.transfer(0x55);
	SPI.transfer(0x40);
	//BEWARE: SPI is in mode 0 (master write) after reset
}

void Barometer5540::getCoefficients(){

	barometerReset();
	//Calibration word 1
	uint16_t result1 = 0;// 16 bit
	uint16_t inbyte1 = 0;
	SPI.transfer(0x1D); //send first byte of command to get calibration word 1
	SPI.transfer(0x50); //send second byte of command to get calibration word 1
	SPI.setDataMode(SPI_MODE1); //change mode in order to listen
	result1 = SPI.transfer(0x00); //send dummy byte to read first byte of word
	result1 = result1 << 8; //shift returned byte
	inbyte1 = SPI.transfer(0x00); //send dummy byte to read second byte of word
	result1 = result1 | inbyte1; //combine first and second byte of word
	Serial.print("Calibration word 1 = ");
	Serial.print(result1,HEX);
	Serial.print(" ");
	Serial.println(result1);

	barometerReset();

	//Calibration word 2; see comments on calibration word 1
	uint16_t result2 = 0;
	byte inbyte2 = 0;
	SPI.transfer(0x1D);
	SPI.transfer(0x60);
	SPI.setDataMode(SPI_MODE1);
	result2 = SPI.transfer(0x00);
	result2 = result2 <<8;
	inbyte2 = SPI.transfer(0x00);
	result2 = result2 | inbyte2;
	Serial.print("Calibration word 2 = ");
	Serial.print(result2,HEX);
	Serial.print(" ");
	Serial.println(result2);

	barometerReset();

	//Calibration word 3; see comments on calibration word 1
	uint16_t result3 = 0;
	byte inbyte3 = 0;
	SPI.transfer(0x1D);
	SPI.transfer(0x90);
	SPI.setDataMode(SPI_MODE1);
	result3 = SPI.transfer(0x00);
	result3 = result3 <<8;
	inbyte3 = SPI.transfer(0x00);
	result3 = result3 | inbyte3;
	Serial.print("Calibration word 3 = ");
	Serial.print(result3,HEX);
	Serial.print(" ");
	Serial.println(result3);

	barometerReset();

	//Calibration word 4; see comments on calibration word 1
	uint16_t result4 = 0;
	byte inbyte4 = 0;
	SPI.transfer(0x1D);
	SPI.transfer(0xA0);
	SPI.setDataMode(SPI_MODE1);
	result4 = SPI.transfer(0x00);
	result4 = result4 <<8;
	inbyte4 = SPI.transfer(0x00);
	result4 = result4 | inbyte4;
	Serial.print("Calibration word 4 = ");
	Serial.print(result4,HEX);
	Serial.print(" ");
	Serial.println(result4);

	//now we do some bitshifting to extract the calibration factors
	//out of the calibration words;
	coefficients[0] = (result1 >> 1) & 0x7FFF;
	coefficients[1] = ((result3 & 0x003F) << 6) | (result4 & 0x003F);
	coefficients[2] = (result4 >> 6) & 0x03FF;
	coefficients[3] = (result3 >> 6) & 0x03FF;
	coefficients[4] = ((result1 & 0x0001) << 10) | ((result2 >> 6) & 0x03FF);
	coefficients[5] = result2 & 0x003F;

	// print all of the things
	Serial.print("c1 = ");
	Serial.println(coefficients[0]);
	Serial.print("c2 = ");
	Serial.println(coefficients[1]);
	Serial.print("c3 = ");
	Serial.println(coefficients[2]);
	Serial.print("c4 = ");
	Serial.println(coefficients[3]);
	Serial.print("c5 = ");
	Serial.println(coefficients[4]);
	Serial.print("c6 = ");
	Serial.println(coefficients[5]);

	barometerReset();
}
void Barometer5540::tick(){
	// gets the temperature and pressure values

	//Pressure:
	uint16_t presMSB = 0; //first byte of value
	uint16_t presLSB = 0; //last byte of value
	uint16_t D1 = 0;
	SPI.transfer(0x0F); //send first byte of command to get pressure value
	SPI.transfer(0x40); //send second byte of command to get pressure value
	delay(35); //wait for conversion end // Richard: can we even use this??
	SPI.setDataMode(SPI_MODE1); //change mode in order to listen
	presMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
	presMSB = presMSB << 8; //shift first byte
	presLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
	D1 = presMSB | presLSB; //combine first and second byte of value
	Serial.print("D1 - Pressure raw = ");
	Serial.println(D1);

	barometerReset(); //resets the sensor

	//Temperature:
	uint16_t tempMSB = 0; //first byte of value
	uint16_t tempLSB = 0; //last byte of value
	uint16_t D2 = 0;
	SPI.transfer(0x0F); //send first byte of command to get temperature value
	SPI.transfer(0x20); //send second byte of command to get temperature value
	delay(35); //wait for conversion end
	SPI.setDataMode(SPI_MODE1); //change mode in order to listen
	tempMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
	tempMSB = tempMSB << 8; //shift first byte
	tempLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
	D2 = tempMSB | tempLSB; //combine first and second byte of value
	Serial.print("D2 - Temperature raw = ");
	Serial.println(D2); //voila!

	//calculation of the real values by means of the calibration factors and the maths
	//in the datasheet. const MUST be long
	const int32_t UT1 = (coefficients[4] << 3) + 20224;
	const int32_t dT = D2 - UT1;
	int32_t TEMP = 200 + ((dT * (coefficients[5] + 50)) >> 10);
	const int32_t OFF  = (coefficients[1] * 4) + (((coefficients[3] - 512) * dT) >> 12);
	const int32_t SENS = coefficients[0] + ((coefficients[2] * dT) >> 10) + 24576;
	const int32_t X = (SENS * (D1 - 7168) >> 14) - OFF;
	int32_t PCOMP = ((X * 10) >> 5) + 2500;
	float TEMPREAL = TEMP/10;

	Serial.print("Real Temperature in C = ");
	Serial.println(TEMPREAL);

	Serial.print("Compensated pressure in mbar = ");
	Serial.println(PCOMP);

	//If temp is greater than 45°C or less than 20°C, do fancy second-order compensation

	int32_t T2 = 0;
	float P2 = 0;

	if (TEMP < 200)
	{
		T2 = (11 * (coefficients[5] + 24) * (200 - TEMP) * (200 - TEMP) ) >> 20;
		P2 = (3 * T2 * (PCOMP - 3500) ) >> 14;
	}
	else if (TEMP > 450)
	{
		T2 = (3 * (coefficients[5] + 24) * (450 - TEMP) * (450 - TEMP) ) >> 20;
		P2 = (T2 * (PCOMP - 10000) ) >> 13;
	}

	if ((TEMP < 200) || (TEMP > 450))
	{
		TEMP = TEMP - T2;
		PCOMP = PCOMP - P2;

		TEMPREAL = TEMP/10;
	}

	barometerData [0] = TEMPREAL; // temperature      (degrees C)
	barometerData [1] = PCOMP; //   pressure (mbar)
}

float Barometer5540::getTemperature(){
	return barometerData[0]; // degrees celsius
}

float Barometer5540::getPressure(){
	return barometerData[1];  // mbar
}

// <temp deg c>,<pressure mbar>
const char* Barometer5540::flushPersistBuffer(){
	persistBuffer[0] = '\0';
	snprintf(persistBuffer, persistBufferSize, "%f,%f\n", getTemperature(), getPressure());
	return persistBuffer;
}

const char* Barometer5540::getModuleName() {
	return "Barometer5540";
}

// the barometer requires a 32.768kHz external clock for its ADC, which we must provide.
// Since we're using timers, it's hardware dependent
#if defined(__AVR_ATmega328P__)
void Barometer5540::setupClock() {
	cli();//disable interrupts
	TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
	pinMode(clockPin, OUTPUT);
	analogWrite (clockPin, 128); // frequency is a bit off, but it seemed to work fine (actual = 31.7 kHz)
	// if this doesn't work or is unstable, legit define a timer using the Scheduler.cpp method, instead of messing with
	// default analogWrite stuff.
	sei();//enable interrupts
}
#endif

#if defined (__arm__)

void Barometer5540::setupClock() {
	pmc_set_writeprotect(false);		 // disable write protection for pmc registers
	pmc_enable_periph_clk(ID_TC0);	 // enable peripheral clock TC7

	/* we want wavesel 01 with RA */
	// TC0, channel 0, clock5 = SLCK, 32kHz - output = TIOA0 = PB25 = digital pin 2
	// TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
	// page 865 of datasheet, plus the due timer TIOA0 = digital pin 2, PB25
	// RA means register A, count up to 1 since we're clocking at the desired frequency. Do this because RA controls TIOA frequency (datasheet), and it's mapped to a Due pin we have access to
	// https://github.com/ivanseidel/DueTimer/blob/master/TimerCounter.md



	// Richard: probably need to set to RA instead of RC - but it's not a macro we have? hmm..
	TC_Configure(/* clock */TC0,/* channel */0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK5);
	TC_SetRC(TC0, 0, 1);  // 32.768 kHz
	TC_Start(TC0, 0);

	// enable timer interrupts on the timer
	TC0->TC_CHANNEL[0].TC_IER=TC_IER_CPCS;   // IER = interrupt enable register
	TC0->TC_CHANNEL[0].TC_IDR=~TC_IER_CPCS;  // IDR = interrupt disable register

	/* Enable the interrupt in the nested vector interrupt controller */
	/* TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1 */
	NVIC_EnableIRQ(TC0_IRQn);

	// Richard: worst case here we toggle the pin in the ISR, but this should work ... may need to set PB25 as an output??
}

void TC0_Handler(){
	// PORT_PB < 26; // figure out how to toggle PB26 in the ISR without variable weirdness, or enable the automatic output from the timer
	TC_GetStatus(TC0, 0);   // reading this register flips something to ensure the counter can be incremented again
}
#endif
