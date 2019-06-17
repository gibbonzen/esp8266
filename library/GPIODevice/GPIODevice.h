#ifndef GPIODevice_h
#define GPIODevice_h

#include "Arduino.h"

class GPIODevice {
    public: 
        GPIODevice(int pin);

        void enable();

        void disable();

        void impulse(int millis = 200);

    private:
        int _pin;
};

#endif