#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>
#include "Globals.h"

enum class SystemState {
        PRE_FLIGHT = 1,
        DURING_FLIGHT,
        DURING_DESCENT,
        LOW_BATTERY,
        LANDED,
        INVALID = -1
};

typedef struct {
	bool valid;
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
	 * @return a scheduling_freq that the Scheduler uses to update how this module would run.
	 * Be sure to set its .valid to true
	 */
	virtual scheduling_freq onStateChanged(const SystemState &state);

	/**
	 * @brief
	 *
	 * @return
	 */
	virtual scheduling_freq getSchedulingFreq();

	/**
	 * @brief Should the module creates data and wants to send it over the uno->radio link, this
	 * function would be called by the Radio module to handle it
	 *
	 * @return the c-style string to be sent over the radio
	 */
	virtual const char* dataToSend();

	/**
	 * @brief Should the module creates data and wants to persist it, this function
	 * would be called by the SD module to handle it
	 *
	 * @return the c-style string to be written to the SD card
	 */
	virtual const char* dataToPersist();

	/**
	 * @brief Called by the SD module and used as a label for this module's CSV output
	 *
	 * @return the c-style string for the name of this module
	 */
	virtual const char* getModuleName();

protected:
        State state;
};

#endif /* MODULE_H */

