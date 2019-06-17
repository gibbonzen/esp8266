#include "Arduino.h"
#include "GPIODevice.h"

GPIODevice::GPIODevice(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void GPIODevice::enable() {
    digitalWrite(_pin, HIGH);
}

void GPIODevice::disable() {
    digitalWrite(_pin, LOW);
}

void GPIODevice::impulse(int millis = 200) {
    enable();
    delay(millis);
    disable();
    delay(millis);
}