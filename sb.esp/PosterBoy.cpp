#include "PosterBoy.h"

#include <WiFi.h>
#include <HTTPClient.h>

extern char buffer[];

extern char fzm1[];
extern char cf20[];

void PosterBoy::send(const char* server, const char* endpoint, const char* data)
{
	WiFiClient client;
	HTTPClient http;

	strcpy(buffer, server);
	strcat(buffer, endpoint);
	http.begin(client, buffer);

	Serial.println(buffer);
	Serial.println(data);

	http.addHeader("Content-Type", "text/plain");
	int httpResponseCode = http.POST(data);
	Serial.println(httpResponseCode);
	Serial.println();
	http.end();
}
