#include <Arduino.h>

class StateMachine {
private:
  int _state;
  int _frames;
  int _startTime;

public:
  void begin();
  void setState(int state);
  void update();
};
