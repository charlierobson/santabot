#include <Arduino.h>

#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

extern int vars[5];

class SkirtLed {
private:
  static const int DATA_PIN = 27;
  static const int NUM_LEDS = 50;

  int _pattern;
  int _lastMillis;
  CRGB _leds[NUM_LEDS];

public:
  void begin();
  void setPattern(int pattern);
  void update();
};
