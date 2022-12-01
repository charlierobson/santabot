#include "RingLed.h"


RingLed::RingLed() {
  _numleds = NUM_LEDS;
}


void RingLed::begin() {
  FastLED
  .addLeds<WS2812, DATA_PIN, GRB>(_leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip)
  .setDither(false);

  FastLED.setBrightness(255);
  _pattern = 0;
}


bool RingLed::update() {
  switch (_pattern) {
    case 0:
      black();
      break;
    case 1:
      pride();
      break;
    case 2:
      red();
      break;
  }
  FastLED.show();
  return false;
}
