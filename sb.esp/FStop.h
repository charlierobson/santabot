#ifndef FSTOP_H
#define FSTOP_H

#include <Arduino.h>

class FStop
{
public:
	void begin();
	bool engaged();
};

#endif
