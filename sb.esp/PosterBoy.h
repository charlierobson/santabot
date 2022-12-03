#ifndef POSTERBOY_H
#define POSTERBOY_H

#include <Arduino.h>

class PosterBoy
{
public:
	static void send(const char* endpoint, const char* data);
};

#endif
