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
      startup,        // 1 quick startup animation -> nameselect
      nameselect,     // 2 ambient animation
      waittouch,      // 3 name is selected, sb does waiting for touch anim touch -> touched
      touched,        // 4 confirmation anim -> evaluate
      evaluate,       // 5 main animation
      nice,           // 6 colourful happy anim -> nameselect
      naughty,        // 7 naughty anim -> go wrong
      gowrong,        // 8 glitching out, press of f-stop -> hal
      hal
    } state;

    void begin(int initialState);
    int getState();
    void setState(int state);
    void update();
};

#endif
