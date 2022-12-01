#include "CapTouch.h"

void CapTouch::begin() {
}

bool CapTouch::update() {
  return touchRead(4) < 10;
}
