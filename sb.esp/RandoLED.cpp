#include "RandoLED.h"

#include <FastLED.h>

void RandoLED::begin()
{
	pinMode(14, INPUT);
	digitalWrite(14, LOW);
	pinMode(32, INPUT);
	digitalWrite(32, LOW);
	pinMode(33, INPUT);
	digitalWrite(33, LOW);
	pinMode(25, INPUT);
	digitalWrite(25, LOW);
	pinMode(26, INPUT);
	digitalWrite(26, LOW);
	pinMode(12, INPUT);
	digitalWrite(12, LOW);
	pinMode(13, INPUT);
	digitalWrite(13, LOW);
}

void RandoLED::update() {
	EVERY_N_MILLISECONDS(300) {
		int n = random();
		pinMode(14, n&1 ? INPUT : OUTPUT);
		pinMode(32, n&2 ? INPUT : OUTPUT);
		pinMode(33, n&4 ? INPUT : OUTPUT);
		pinMode(25, n&8 ? INPUT : OUTPUT);
		pinMode(26, n&16 ? INPUT : OUTPUT);
		pinMode(12, n&32 ? INPUT : OUTPUT);
		pinMode(13, n&64 ? INPUT : OUTPUT);
	}
}
