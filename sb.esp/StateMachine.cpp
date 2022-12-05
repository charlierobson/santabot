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

extern char fzm1[];
extern char cf20[];
extern char name[];

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


static const int orbBaseSpeed = 20;

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
          PosterBoy::send(fzm1, "play", "startup.wav");
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
//          PosterBoy::send(fzm1, "play", "ambience-quiet.wav");
          ringLed.setPattern(WS28::ptnOff);
          skirtLed.setPattern(WS28::ptnNameSelect);
          FastLED.setBrightness(128);
          }
        // exit state when tablet says so -> waittouch
      }
      break;

    case waittouch:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnOff);
		  // if julie..?
		  if (strcmp(name, "naughty") == 0) {
		  	PosterBoy::send(fzm1, "play", "arcing.wav");
            skirtLed.setPattern(WS28::ptnArcing);
		  }
		  else
          	skirtLed.setPattern(WS28::ptnWaitTouch);
        }
        // exit state when touched
		if (capTouch.update()) 
			nextState = evaluate;
      }
      break;

    case evaluate:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnEvaluate);
          skirtLed.setPattern(WS28::ptnEvaluate);
          PosterBoy::send(fzm1, "play", "evaluate.wav");
          PosterBoy::send(cf20, "santabot", "touched");
        }

        theOrb.setSpeed(millis() & 1024 ? orbBaseSpeed : -orbBaseSpeed);

		bool isNaughty = strcmp(name, "naughty") == 0;

        if (millis() - _lastMillis > (isNaughty ? 8000 : 5000)) {
          nextState = isNaughty ? naughty : nice;
        }
      }
      break;

    case nice:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnNice);
          skirtLed.setPattern(WS28::ptnNice);
          theOrb.setSpeed(0);
          PosterBoy::send(fzm1, "play", "evaluate-nice.wav");
          PosterBoy::send(fzm1, "print", name);
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
          PosterBoy::send(fzm1, "loop", "evaluate-naughty.wav");
          PosterBoy::send(fzm1, "print", "naughty");
        }
		int rate = (millis()-_lastMillis) / 30;
		if (rate > (255-orbBaseSpeed)) rate = (255-orbBaseSpeed);
        theOrb.setSpeed(orbBaseSpeed+rate);

        // exit state when e-stop hit
       if (fStop.engaged()) {
			nextState = hal;
          PosterBoy::send(cf20, "santabot", "hal");
          PosterBoy::send(fzm1, "stop", "");
	   }
      }
      break;

    case hal:
      {
        if (_frames == 0) {
          ringLed.setPattern(WS28::ptnHAL);
          skirtLed.setPattern(WS28::ptnHAL);
          PosterBoy::send(fzm1, "play", "fading-alt2.wav");
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

  if (nextState != _state) {
    setState(nextState);
    // post new state to tablet?
  }
}
