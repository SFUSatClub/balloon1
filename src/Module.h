#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <FatLib/ArduinoStream.h>

/* Base sensor class */
class Module {
public: 
	Module();

	// Steven: bit shifted to use as flags
	enum class State {
		BEGIN_FAILED = 1 << 0,
		BEGIN_SUCCESS = 1 << 1,
		GOOD = 1 << 2
	};
	
	// Steven: Pure virtual functions: begin() + tick(), must be
	// implemented in derived classes otherwise will get linker error
	// 'undefined reference to vtable'. Forces all sensor 'modules' to have a
	// common interface.

	// Called once in setup()
	virtual void begin() = 0;
	// Called periodically in loop()
	virtual void tick() = 0;

	// Enable the module, renable power
	virtual int enable();
	// Disable the module, powersaving mode
	virtual void disable();
	// Should the module creates data and wants to persist it, this function
	// will be called by the SD module to take care of it
	virtual const char* dataToPersist();
	// The name of the module
	virtual const char* getModuleName();
protected:
	State state;
};

extern ArduinoOutStream cout;

extern ArduinoInStream cin;

#endif /* MODULE_H */
