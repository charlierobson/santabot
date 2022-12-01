#ifndef THEORB_H
#define THEORB_H

#include <Arduino.h>

class TheOrb {
    int _speed;

  public:
    void begin();
    void update();
    void setSpeed(int speed);
};

#endif
