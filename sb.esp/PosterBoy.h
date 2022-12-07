#ifndef POSTERBOY_H
#define POSTERBOY_H

#include <Arduino.h>

class PosterBoy
{
public:
	static int send(const char* server, const char* endpoint, const char* data);
};

#endif
