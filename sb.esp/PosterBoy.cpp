#include "PosterBoy.h"

#include <WiFi.h>
#include <HTTPClient.h>

extern char fzm1[];
extern char cf20[];

char url[200];

int PosterBoy::send(const char* server, const char* endpoint, const char* data)
{
	WiFiClient client;
	HTTPClient http;

	strcpy(url, server);
	strcat(url, endpoint);
	http.begin(client, url);

	Serial.print("Poster boy sending ");
	Serial.print(data);
	Serial.print(" to ");
	Serial.print(url);

	http.addHeader("Content-Type", "text/plain");
	int httpResponseCode = http.POST(data);

	Serial.print(" rx'd code ");
	Serial.println(httpResponseCode);
	Serial.println();
	http.end();

	return httpResponseCode;
}
