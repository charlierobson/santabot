#include "StateMachine.h"

#include "RingLed.h"
#include "SkirtLed.h"
#include "CapTouch.h"
#include "TheOrb.h"

RingLed ringLed;
SkirtLed skirtLed;
CapTouch capTouch;
TheOrb theOrb;

void StateMachine::begin()
{
  ringLed.begin();
  skirtLed.begin();
  theOrb.begin();
  capTouch.begin();
}

void StateMachine::setState(int state)
{
  _state = state;
  _frames = 0;
  _startTime = millis();
}

void StateMachine::update()
{
  int nextState = _state;

  switch (_state){
    
  }

  ++_frames;

  if (nextState != _state) {
    setState(nextState);
    // post new state to tablet?
  }

  ringLed.update();
  skirtLed.update();
  theOrb.update();
  capTouch.update();
}
