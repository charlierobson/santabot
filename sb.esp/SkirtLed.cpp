#include "SkirtLed.h"

SkirtLed::SkirtLed() {
  FastLED
  .addLeds<WS2812, DATA_PIN, RGB>(_leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip)
  .setDither(false);

  FastLED.setBrightness(255);
  _numLeds = NUM_LEDS;
  _pattern = 0;
}
