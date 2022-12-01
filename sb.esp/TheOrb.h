#include <Arduino.h>

class TheOrb {
  int _speed;

public:
  void begin();
  void update();
  void setSpeed(int speed);
};
