#ifndef RINGLED_H
#define RINGLED_H

#include "WS28.h"

class RingLed : public WS28 {
  private:
    static const int DATA_PIN = 5;
    static const int NUM_LEDS = 10;

  public:
    RingLed();
};

#endif
