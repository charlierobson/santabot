#ifndef CAPTOUCH_H
#define CAPTOUCH_H

#include <Arduino.h>

class CapTouch {
  public:
    void begin();
    bool update();
};

#endif
