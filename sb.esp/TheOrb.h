#ifndef THEORB_H
#define THEORB_H

#include <Arduino.h>

class TheOrb {
  public:
    void begin();
    void update();
    void setSpeed(int speed);
};

#endif
