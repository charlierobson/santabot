#include "SkirtLed.h"

SkirtLed::SkirtLed() {
  _numleds = NUM_LEDS;
}


void SkirtLed::begin() {
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(_leds, _numleds)
  .setCorrection(TypicalLEDStrip)
  .setDither(false);

  FastLED.setBrightness(255);
  _pattern = 0;
}


bool SkirtLed::update() {
  bool completed = false;
  switch (_pattern)
  {
    case 0:
      black();
      break;
    case 1:
      breathe();
      break;
    case 2:
      completed = wakeup();
      break;
    case 3:
      pride();
      break;
    case 4:
      red();
      break;
  }
  FastLED.show();
  return completed;
}
