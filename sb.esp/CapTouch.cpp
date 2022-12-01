#include "CapTouch.h"

void CapTouch::begin(){
}

void CapTouch::update(){
}

bool CapTouch::isTouched() {
  return touchRead(4) < 10;
}
