#include "PosterBoy.h"

#include <WiFi.h>
#include <HTTPClient.h>

extern char buffer[];

void PosterBoy::send(const char* endpoint, const char* data)
{
	WiFiClient client;
	HTTPClient http;

	strcpy(buffer, "http://192.168.1.116:8000/");
	strcat(buffer, endpoint);
	http.begin(client, buffer);

	http.addHeader("Content-Type", "text/plain");
	int httpResponseCode = http.POST(data);

	http.end();
}
