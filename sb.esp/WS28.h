#ifndef WS28_H
#define WS28_H

#include <Arduino.h>

#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

class WS28 {
  protected:
    int _numLeds;
    int _pattern;
    int _lastMillis;
    int _hue;
  
    CRGB _leds[50];

    bool black();
    bool red();
    bool breathe();
    bool wakeup();
    bool nameSelect();
    bool arcing();
    bool waitTouch();
    bool touched();
    bool evaluate();
    bool nice();
    bool naughty();
    bool hal();
    bool pride();

  public:
    enum {
      ptnOff = 0,
      ptnBreathe,
      ptnStartup,
      ptnNameSelect,
      ptnWaitTouch,
      ptnArcing,
      ptnTouched,
      ptnEvaluate,
      ptnNice,
	  ptnNaughty,
	  ptnHAL
    } pattern;

    // true when pattern complete
    bool update(int hue);

    void setPattern(int pattern);

    void show();
};

#endif
