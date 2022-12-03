#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>

class StateMachine {
  private:
    int _state;
    int _frames;
    int _lastMillis;

    bool _skirtComplete;
    bool _touched;

  public:
    enum {
      sleep = 0,      // sb is idle, single led 'breathing'
      startup,        // quick startup animation -> nameselect
      nameselect,     // ambient animation
      waittouch,      // name is selected, sb does waiting for touch anim touch -> touched
      touched,        // confirmation anim -> evaluate
      evaluate,       // main animation
      nice,           // colourful happy anim -> nameselect
      naughty,        // naughty anim -> go wrong
      gowrong,        // glitching out, press of f-stop -> hal
      hal
    } state;

    void begin(int initialState);
    int getState();
    void setState(int state);
    void update();
};

#endif
