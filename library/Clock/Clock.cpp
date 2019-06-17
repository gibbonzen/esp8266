#include "Clock.h"

#include "TimerUtils.h"

Clock::Clock(int* time) {
    this.setTime(time);
}

Clock::setTime(int* time) {
    _hCount = time[0];
    _mCount = time[1];
    _sCount = time[2];
}

long Clock::getTime() {

}

void Clock::process() {
    unsigned long timer = TimerUtils.superMillis();

    if(timer % 1000 == 0) { // Each seconds
        _sCount++;
    }

    if(_sCount >= 60) { // one minute
        _mCount++;
        _sCount = 0; // reinitiate seconds counter
    }

    if(_mCount >= 60) { // one hour
        _hCount++;
        _mCount = 0;
    }

    if(_hCount >= 24) { // one day
        _hCount = 0;
    }
}