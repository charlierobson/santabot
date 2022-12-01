#include "SkirtLed.h"

static const int BRIGHTNESS = 255;

static void red(CRGB* leds, int numleds)
{
  for ( uint16_t i = 0 ; i < numleds; i++) {
    nblend( leds[i], millis() & 512 ? CRGB::Red : CRGB::Black, 8);
  }
}

static float pulseSpeed = 0.4;  // Larger value gives faster pulse.
static float delta = (120 - 00) / 2.35040238;  // Do Not Edit

static void breathe(CRGB* leds, int numleds)
{
  for ( uint16_t i = 0 ; i < numleds; i++) {
    leds[i] = CRGB::Black;
  }

  float dV = ((exp(sin(pulseSpeed * millis() / 2000.0 * PI)) - 0.36787944) * delta);

  leds[19] = CHSV(0, 0, dV);
}

static void wakeup(CRGB* leds, int numleds, int startTime)
{
  const int rate = vars[0];

  int elapsed = millis() - startTime;
  if (elapsed < rate*3) {

    int phase = elapsed / rate;
    int pos = elapsed % rate;

    float nPos = (float)pos / float(rate) * 40.0;

    if (phase == 0) {
      leds[(int)nPos] = CRGB::White;
    }
    else if (phase == 1) {
      leds[40 - (int)nPos] = CRGB::White;
    }
    else if (pos < 100) {
      for (int i = 0; i < numleds; ++i)
        leds[i] = CRGB::White;
    }
  }

  fadeToBlackBy(leds, numleds, vars[1]);
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
static void pride(CRGB* leds, int numleds)
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

  for ( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}


void SkirtLed::begin() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(_leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip)
  .setDither(false);

  // set master brightness control
  FastLED.setBrightness(255);
}

void SkirtLed::setPattern(int pattern) {
  _lastMillis = millis();
  _pattern = pattern;
}

void SkirtLed::update() {
  switch (_pattern)
  {
    case 0:
      breathe(_leds, NUM_LEDS);
      break;
    case 1:
      wakeup(_leds, NUM_LEDS, _lastMillis);
      break;
    case 2:
      pride(_leds, NUM_LEDS);
      break;
    case 3:
      red(_leds, NUM_LEDS);
      break;
  }
  FastLED.show();
}
