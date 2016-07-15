#include "Module.h"


// Steven: Bitwise operators to use State enum as flags
inline Module::State operator|(Module::State a, Module::State b) {
	return static_cast<Module::State>(static_cast<int>(a) | static_cast<int>(b));
}
inline Module::State operator &(Module::State a, Module::State b) {
    return static_cast<Module::State>(static_cast<int>(a) & static_cast<int>(b));
}
inline Module::State& operator |=(Module::State& a, Module::State b) {
    return a = a | b;
}

Module::Module() {
	// Steven: nothing for now
}

const char* Module::dataToPersist() {
	return NULL;
}
