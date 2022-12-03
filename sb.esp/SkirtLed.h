#ifndef SKIRTLED_H
#define SKIRTLED_H

#include "WS28.h"

class SkirtLed : public WS28 {
  private:
    static const int DATA_PIN = 27;
    static const int NUM_LEDS = 50;

  public:
    SkirtLed();
};

#endif
