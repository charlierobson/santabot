#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "html.h"

#include "StateMachine.h"

const char* host = "sbesp";

WiFiMulti wifiMulti;

WebServer server(80);

StateMachine stateMachine;

int vars[5];

const int BUFFER_SIZE = 20;
char buffer[BUFFER_SIZE];

void setup(void) {
  Serial.begin(115200);

  Serial.println("");

  wifiMulti.addAP("Pottymother", "ec3ecbf5e8");
  wifiMulti.addAP("B2M Solutions ", "badentractfurysnare");

  if (wifiMulti.run() != WL_CONNECTED) {
    ESP.restart();
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(host)) {
    Serial.println("Error setting up MDNS responder!");
    ESP.restart();
  }

  Serial.print("mDNS responder address: ");
  Serial.print(host);
  Serial.println(".local");

  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on("/getState", HTTP_GET, []() {
    sprintf(buffer, "{\"state\":%d,\"vars\":[%d,%d,%d,%d,%d]}", stateMachine.getState(),vars[0],vars[1],vars[2],vars[3],vars[4]);
    server.send(200, "application/json", buffer);
  });
  server.on("/setState", HTTP_POST, []() {
    char* endPtr;

    if (server.args() == 0) {
      return server.send(500);
    }

    server.arg(0).toCharArray(buffer, BUFFER_SIZE);

    Serial.print("POSTed state: ");
    Serial.println(buffer);

    stateMachine.setState(atoi(buffer));
    server.send(200);
  });
  server.on("/setVars", HTTP_POST, []() {
    int i = 0;
    if (server.args() == 0) {
      return server.send(500);
    }

    server.arg(0).toCharArray(buffer, BUFFER_SIZE);

    char* command = strtok(buffer, ",");
    while (command != NULL && i < 5)
    {
      vars[i] = atoi(command);
      ++i;
      command = strtok(0, ",");
    }

    server.send(200);
  });

  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.begin();

  stateMachine.begin(StateMachine::sleep);
}

void loop(void) {
  server.handleClient();
  stateMachine.update();
}
