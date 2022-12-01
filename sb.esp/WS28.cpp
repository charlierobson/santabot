#include "WS28.h"

extern int vars[5];


void WS28::setPattern(int pattern) {
  _lastMillis = millis();
  _pattern = pattern;
}


bool WS28::red()
{
  for ( uint16_t i = 0 ; i < _numleds; i++) {
    nblend(_leds[i], millis() & 512 ? CRGB::Red : CRGB::Black, 8);
  }
  return false;
}


bool WS28::black()
{
  for ( uint16_t i = 0 ; i < _numleds; i++) {
    nblend(_leds[i], CRGB::Black, 8);
  }
  return false;
}


bool WS28::breathe()
{
  for ( uint16_t i = 0 ; i < _numleds; i++) {
    _leds[i] = CRGB::Black;
  }

  int pos = millis() & 8191;
  if (pos < 4096)
    _leds[19] = CHSV(0, 0, pos / 16);
  else
    pos -= 4096;
    _leds[19] = CHSV(0, 0, 256 - (pos / 16));
  return false;
}

bool WS28::wakeup()
{
  const int rate = vars[0];

  int elapsed = millis() - _lastMillis;
  if (elapsed < rate * 3) {

    int phase = elapsed / rate;
    int pos = elapsed % rate;

    float nPos = (float)pos / float(rate) * 40.0;

    if (phase == 0) {
      _leds[(int)nPos] = CRGB::White;
    }
    else if (phase == 1) {
      _leds[40 - (int)nPos] = CRGB::White;
    }
    else if (pos < 100) {
      for (int i = 0; i < _numleds; ++i)
        _leds[i] = CRGB::White;
    }
  }

  fadeToBlackBy(_leds, _numleds, vars[1]);
  return elapsed > rate * 3;
}


// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
bool WS28::pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < _numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (_numleds - 1) - pixelnumber;

    nblend(_leds[pixelnumber], newcolor, 64);
  }
  return false;
}
