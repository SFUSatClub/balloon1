#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>

/* Base sensor class */
class Module {
public: 
	Module();
	
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
};

#endif /* MODULE_H */
