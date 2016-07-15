#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>

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
	
	// Steven: Pure virtual functions, they must be implemented in derived classes
	// otherwise will get linker error 'undefined reference to vtable'. Forces
	// all sensor 'modules' to have a common interface.
	
	// Called once in setup()
	virtual void begin() = 0;
	// Called periodically in loop()
	virtual void tick() = 0;
	// Enable the module, renable power
	virtual int enable() = 0;
	// Disable the module, powersaving mode
	virtual void disable() = 0;
	// Should the module creates data and wants to persist it, this function
	// will be called elsewhere to take care of it
	virtual const char* dataToPersist();
protected:
	State state;
};

#endif /* MODULE_H */
