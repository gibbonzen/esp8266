#ifndef TimerUtils_h
#define TimerUtils_h

#include "Arduino.h"

class TimerUtils {
  public:
    unsigned long long superMillis();

    long convert(int time, char from, char to);
};

#endif