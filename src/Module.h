#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>
#include "Globals.h"
#include "SystemState.h"

// Macro to help condition-away logging to serial
// Use with #ifdef DEBUG too
#define PP(x) if(propertyShouldPrint) {x}

typedef struct {
	uint16_t timeout;
	uint32_t interval;
} scheduling_freq;


/**
 * @brief Base sensor class
 *
 * Steven: Pure virtual functions: begin() + tick(), must be
 * implemented in derived classes otherwise will get linker error
 * 'undefined reference to vtable'. Forces all sensor 'modules' to have a
 * common interface
 *
 */
class Module {
public:
	Module();

	/**
	 * @brief Bit shifted to use as flags
	 */
	enum class State {
		BEGIN_FAILED = 1 << 0,
		BEGIN_SUCCESS = 1 << 1,
		GOOD = 1 << 2
	};


	/**
	 * @brief Called once in setup()
	 */
	virtual void begin() = 0;

	/**
	 * @brief Called periodically by the Scheduler
	 */
	virtual void tick() = 0;

	/**
	 * @brief Enable the module and exit powersaving mode
	 *
	 * @return something for success, something for failure
	 */
	virtual int enable();

	/**
	 * @brief Disable the module and enter powersaving mode
	 */
	virtual void disable();

	/**
	 * @brief Called by the Scheduler whenever an actionable state-change is detected
	 *
	 * @param state what state the system just changed to
	 *
	 * @return
	 */
	virtual void onStateChanged(const SystemState &state);

	/**
	 * @brief
	 *
	 * @return
	 */
	scheduling_freq getSchedulingFreq();

	/**
	 * @brief Should the module creates data and wants to send it over the uno->radio link, this
	 * function would be called by the Radio module to handle it
	 *
	 * @return the c-style string to be sent over the radio
	 */
	virtual const char* flushSendBuffer();

	/**
	 * @brief Should the module creates data and wants to persist it, this function
	 * would be called by the SD module to handle it
	 *
	 * @return the c-style string to be written to the SD card
	 */
	virtual const char* flushPersistBuffer();

	/**
	 * @brief Called by the SD module and used as a label for this module's CSV output
	 *
	 * @return the c-style string for the name of this module
	 */
	virtual const char* getModuleName();

	void allocatePersistBuffer(int bufferSize);
	void allocateSendBuffer(int bufferSize);

	const char* getPersistBuffer();
	const char* getSendBuffer();

	bool shouldTick(uint32_t currSystemTick);
	void setTicked(uint32_t currSystemTick);
	bool propertyShouldPrint;

protected:
	State state;

	int persistBufferIndex;
	int persistBufferSize;
	char *persistBuffer;

	int sendBufferIndex;
	int sendBufferSize;
	char *sendBuffer;

	scheduling_freq freq;
private:
	uint32_t lastSystemTick;
	uint32_t lastPrintTick;
};

#endif /* MODULE_H */
