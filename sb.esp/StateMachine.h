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
    bool _fStop;

  public:
    void begin(int initialState);
    void setState(int state);
    void update();
};

#endif
