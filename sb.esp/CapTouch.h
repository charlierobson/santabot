#include <Arduino.h>

class CapTouch {
public:
  void begin();
  void update();

  bool isTouched();
};
