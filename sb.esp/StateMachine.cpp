#include "StateMachine.h"

#include "RingLed.h"
#include "SkirtLed.h"
#include "CapTouch.h"
#include "FStop.h"
#include "TheOrb.h"
#include "RandoLED.h"
#include "PosterBoy.h"

RingLed ringLed;
SkirtLed skirtLed;
CapTouch capTouch;
FStop fStop;
TheOrb theOrb;
RandoLED randoLED;

extern int vars[5];

void StateMachine::begin(int initialState)
{
  theOrb.begin();
  capTouch.begin();
  fStop.begin();
  randoLED.begin();

  setState(initialState);
}


void StateMachine::setState(int state)
{
  Serial.print("StateMachine::setState ");
  Serial.println(state);

  _state = state;

  _frames = 0;
  _lastMillis = millis();
}


int StateMachine::getState()
{
	return _state;
}


void StateMachine::update()
{
  static int hue;

  int nextState = _state;

  switch (_state) {

    case sleep:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnOff);
          skirtLed.setPattern(WS28::ptnBreathe);
          theOrb.setSpeed(0);
        }
        // exit state when tablet says so
      }
      break;

    case startup:
      {
        if (_frames == 0) {
          PosterBoy::send("play", "startup.wav");
          ringLed.setPattern(WS28::ptnOff);
          skirtLed.setPattern(WS28::ptnStartup);
        }
        if (_skirtComplete) {
          nextState = nameselect;
        }
      }
      break;

    case nameselect:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnOff);
          skirtLed.setPattern(WS28::ptnNameSelect);
          FastLED.setBrightness(128);
          }
        // exit state when tablet says so -> waittouch
        if (millis() - _lastMillis > 7500)
          nextState = waittouch;
      }
      break;

    case waittouch:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnOff);
          skirtLed.setPattern(WS28::ptnWaitTouch);
		  // if julie..?
		  if (vars[0])
		  	PosterBoy::send("play", "arcing.wav");
        }
        // exit state when touched
		if (capTouch.update()) 
			nextState = touched;
      }
      break;

    case touched:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnTouched);
          skirtLed.setPattern(WS28::ptnTouched);
        }
        if (millis() - _lastMillis > 1000) {
          nextState = evaluate;
        }
      }
      break;

    case evaluate:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnEvaluate);
          skirtLed.setPattern(WS28::ptnEvaluate);
          PosterBoy::send("play", "evaluate.wav");
        }

        theOrb.setSpeed(millis() & 1024 ? 90 : -90);

        // exit state when tablet says so?
        if (millis() - _lastMillis > 10000)
			nextState = vars[0] == 0 ? nice : naughty;
      }
      break;

    case nice:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnNice);
          skirtLed.setPattern(WS28::ptnNice);
          theOrb.setSpeed(0);
          PosterBoy::send("play", "evaluate-nice.wav");
          PosterBoy::send("print", "nice Your Name Here");
        }
        // exit state when print complete
        if (millis() - _lastMillis > 6000) {
          nextState = nameselect;
        }
      }
      break;

    case naughty:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnNaughty);
          skirtLed.setPattern(WS28::ptnNaughty);
          PosterBoy::send("loop", "evaluate-naughty.wav");
          PosterBoy::send("print", "naughty");
        }
		int rate = (millis()-_lastMillis) / 30;
		if (rate > (255-90)) rate = (255-90);
        theOrb.setSpeed(90+rate);

        // exit state when e-stop hit
       if (fStop.engaged()) {
			nextState = hal;
          PosterBoy::send("stop", "");
	   }
      }
      break;

    case hal:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnHAL);
          skirtLed.setPattern(WS28::ptnHAL);
          PosterBoy::send("play", "fading-alt2.wav");
        }

        int rate = 255 - millis()-_lastMillis;
        if (rate < 0) rate = 0;
        theOrb.setSpeed(rate);
      }
      break;
  }

  ++_frames;

  EVERY_N_MILLISECONDS( 20 ) { ++hue; }

  ringLed.update(hue);
  _skirtComplete = skirtLed.update(hue);
  skirtLed.show();

  theOrb.update();
  if (_state > sleep && _state < hal)
  	randoLED.update();
  else
  	randoLED.begin();

  vars[2] = capTouch.update();
  vars[3] = fStop.engaged();

  if (nextState != _state) {
    setState(nextState);
    // post new state to tablet?
  }
}
