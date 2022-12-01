#include "StateMachine.h"

#include "RingLed.h"
#include "SkirtLed.h"
#include "CapTouch.h"
#include "TheOrb.h"

RingLed ringLed;
SkirtLed skirtLed;
CapTouch capTouch;
TheOrb theOrb;

void StateMachine::begin(int initialState)
{
  ringLed.begin();
  skirtLed.begin();
  theOrb.begin();
  capTouch.begin();

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

void StateMachine::update()
{
  int nextState = _state;

  switch (_state) {

    case 0: // sleeping
      {
        if (_frames == 0) {
          Serial.println("State 0.0");
          ringLed.setPattern(0); // off
          skirtLed.setPattern(1); // breathe
          theOrb.setSpeed(0);
        }
        // exit state when tablet says so
      }
      break;

    case 1: // wakeup
      {
        if (_frames == 0) {
          Serial.println("State 1.0");
          ringLed.setPattern(0); // off
          skirtLed.setPattern(2); // wakeup
          theOrb.setSpeed(0);
        }
        if (_skirtComplete) {
          nextState = 2;
        }
      }
      break;
    case 2: // nameselect
      {
        // dummy
        if (_frames == 0) {
          Serial.println("State 2.0");
          ringLed.setPattern(1);
          skirtLed.setPattern(3);
          theOrb.setSpeed(75);
        }
        if (millis() - _lastMillis > 2000)
          nextState = 0;
      }
      break;
  }

  ++_frames;

  ringLed.update();
  _skirtComplete = skirtLed.update();

  _touched = capTouch.update();

  theOrb.update();

  if (nextState != _state) {
    setState(nextState);
    // post new state to tablet?
  }
}
