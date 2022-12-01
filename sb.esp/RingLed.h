#include <Arduino.h>

#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

class RingLed {
private:
  static const int DATA_PIN = 5;
  static const int NUM_LEDS = 10;

  int _pattern;
  int _lastMillis;
  CRGB _leds[NUM_LEDS];

public:
  void begin();
  void setPattern(int);
  void update();
};
