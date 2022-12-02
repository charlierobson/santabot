#include "FStop.h"

void FStop::begin()
{
	pinMode(34, INPUT_PULLUP);
}

bool FStop::engaged() {
	return digitalRead(34) == HIGH;
}
