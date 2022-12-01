#ifndef WS28_H
#define WS28_H

#include <Arduino.h>

#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

class WS28 {
  protected:
    int _numleds;
    int _pattern;
    int _lastMillis;

    CRGB _leds[50];

    bool black();
    bool red();
    bool breathe();
    bool pride();
    bool wakeup();

  public:
    virtual void begin() = 0;

    // true when pattern complete
    virtual bool update() = 0;

    void setPattern(int pattern);
};

#endif
