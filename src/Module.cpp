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

int Module::enable() {
    return 0;
}

void Module::disable() {
    return;
}

scheduling_freq Module::onStateChanged(const SystemState &state) {
    scheduling_freq ret;
    ret.valid = false;
    return ret;
}

scheduling_freq Module::getSchedulingFreq() {
    scheduling_freq ret;
    ret.valid = true;
    ret.timeout = 1000;
    ret.interval = 1000;
    return ret;
}

const char* Module::dataToPersist() {
    return NULL;
}

const char* Module::getModuleName() {
    return "Module";
}

