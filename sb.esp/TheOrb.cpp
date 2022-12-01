#include "TheOrb.h"

void TheOrb::begin() {
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  ledcSetup(0, 100, 8);
  ledcAttachPin(18, 0);
  ledcSetup(1, 100, 8);
  ledcAttachPin(19, 1);

  setSpeed(0);
}

void TheOrb::update() {

}

void TheOrb::setSpeed(int speed) {
  _speed = speed;

  if (_speed < 0) {
    _speed = -_speed;

    ledcWrite(0, speed);
    ledcWrite(1, 0);
  }
  else {
    ledcWrite(0, 0);
    ledcWrite(1, speed);
  }
}
