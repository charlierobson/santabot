#include "FStop.h"

void FStop::begin()
{
	pinMode(21, INPUT_PULLUP);
}

bool FStop::engaged() {
	return digitalRead(21) == HIGH;
}
