#include <Arduino.h>
#include "Telerupteur.h"

Telerupteur::Telerupteur(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void enable() {
    digitalWrite(_pin, HIGH);
}

void disable() {
    digitalWrite(_pin, LOW);
}

void Telerupteur::impulse(int millis = 200) {
    enable();
    delay(millis);
    disable();
    delay(millis);
}