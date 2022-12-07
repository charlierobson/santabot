#include "WS28.h"

extern int vars[5];

bool WS28::update(int hue) {
  _hue = hue;

  bool completed = false;
  switch (_pattern)
  {
    case WS28::ptnOff:
      black();
      break;
    case ptnBreathe:
      breathe();
      break;
    case ptnStartup:
      completed = wakeup();
      break;
    case ptnWaitTouch:
      waitTouch();
      break;
    case ptnArcing:
      arcing();
      break;
    case ptnTouched:
      touched();
      break;
    case ptnNameSelect:
      nameSelect();
      break;
    case ptnEvaluate:
      evaluate();
      break;
    case ptnNice:
      nice();
      break;
    case ptnNaughty:
      naughty();
      break;
    case ptnHAL:
      hal();
      break;
  }
  return completed;
}


void WS28::show() {
  FastLED.show();
}


void WS28::setPattern(int pattern) {
  _lastMillis = millis();
  _pattern = pattern;
  FastLED.setBrightness(255);
}


bool WS28::red()
{
  for ( uint16_t i = 0 ; i < _numLeds; i++) {
    nblend(_leds[i], millis() & 512 ? CRGB::Red : CRGB::Black, 8);
  }
  return false;
}


bool WS28::black()
{
  for ( uint16_t i = 0 ; i < _numLeds; i++) {
    nblend(_leds[i], CRGB::Black, 8);
  }
  return false;
}


bool WS28::touched()
{
  int elapsed = millis() - _lastMillis;
  if (elapsed < 100) {
    for ( uint16_t i = 0 ; i < _numLeds; i++)
      _leds[i] = CRGB::Green;
  }
  FastLED.setBrightness(255-((elapsed-100)/4));
  return false;
}


bool WS28::hal()
{
  for ( uint16_t i = 0 ; i < _numLeds; i++)
    _leds[i] = CRGB::Red;

  int phase = (millis() - _lastMillis) / 1000;
  if (phase >= 6 && phase < 15){
	float b = ((float)(phase - 6) / 8.0) * 255;
  	FastLED.setBrightness(255-(int)b);
  }
  if (phase >= 15) {
  	FastLED.setBrightness(255);
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(_leds, _numLeds, 20);
	int pos = beatsin16( 13, 0, _numLeds-1 );
	_leds[pos] += CHSV( _hue, 255, 192);
  }
  return false;
}


bool WS28::nameSelect()
{
  FastLED.setBrightness(16);
  if (millis() & 1024) {
	for (int i = 0; i < 40; ++i) {
		_leds[i] = ((i/4)&1) ? CRGB::Green : CRGB::Red;
	}
  } else {
	for (int i = 0; i < 40; ++i) {
		_leds[i] = ((i/4)&1) ? CRGB::Red : CRGB::Green;
	}
  }

  return false;
}


bool WS28::waitTouch()
{
  int offset = ((millis() - _lastMillis) / 100) % 40;
  for (int i = 0; i < 40; ++i) {
     _leds[(i+offset)%40] = ((i/4)&1) ? CRGB::Green : CRGB::Red;
  }

  return false;
}


bool WS28::arcing()
{
  FastLED.setBrightness(random(255));
  if (millis() - _lastMillis > 3000) {
	_pattern = ptnWaitTouch;
    FastLED.setBrightness(255);
  }
  return false;
}


bool WS28::evaluate()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, _numLeds, 10);
  int pos = random16(_numLeds);
  _leds[pos] += CHSV( (millis()&1024 ? 96: 0) + random8(64), 200, 255);
  return false;
}


bool WS28::nice()
{
  // random colored speckles that blink in and fade smoothly
  fill_rainbow( _leds, _numLeds, _hue, 7);
  if( random8() < 80) {
    _leds[ random16(_numLeds) ] += CRGB::White;
  }
  return false;
}


bool WS28::naughty()
{
  int divisor = 1 << ((millis() - _lastMillis) / 1000);
  if (divisor > 256) divisor = 256;

  for ( uint16_t i = 0 ; i < _numLeds; i++) {
    nblend(_leds[i], millis() & (512/divisor) ? CRGB::Red : CRGB::Black, 8);
  }
  return false;
}


bool WS28::breathe()
{
  int pos = millis() & 8191;
  if (pos < 4096)
    _leds[14] = CHSV(0, 0, pos / 16);
  else
    pos -= 4096;
    _leds[14] = CHSV(0, 0, 256 - (pos / 16));

  fadeToBlackBy(_leds, _numLeds, 20);
  return false;
}

static CRGB dark(32,32,32);

bool WS28::wakeup()
{
  const int rate = 1000;

  int elapsed = millis() - _lastMillis;
  if (elapsed < rate * 4) {

    int phase = elapsed / rate;
    int pos = elapsed % rate;

    float nPos = (float)pos / float(rate) * 40.0;

    if (phase == 0) {
		for (int i = 0; i < _numLeds; ++i)
			_leds[i] = pos < 500 ? dark : CRGB::White;
    }
    else if (phase == 1) {
      _leds[(int)nPos] = CRGB::HotPink;
    }
    else {
		int o = elapsed % 2000;
		int start = (o / 500) * 10;
		for (int i = 0; i < _numLeds; ++i)
			_leds[i] = (i >= start && i < start + 10) ? CRGB::White : CRGB::Blue;
    }
  }

  fadeToBlackBy(_leds, _numLeds, 20);
  return elapsed > rate * 4;
}


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

  for ( uint16_t i = 0 ; i < _numLeds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (_numLeds - 1) - pixelnumber;

    nblend(_leds[pixelnumber], newcolor, 64);
  }
  return false;
}
