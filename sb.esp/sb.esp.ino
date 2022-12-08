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

char fzm1[75];
char cf20[75];
char name[75];
char var3[75];
char var4[75];

int vars[5];

char* svars[] = {
	fzm1, cf20, name, var3, var4
};

const int BUFFER_SIZE = 1024;
char buffer[BUFFER_SIZE];

void setup(void) {
  Serial.begin(115200);

  Serial.println("");

  for(int i = 0; i < 5; ++i)
    strcpy(svars[i], "--");

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

  server.enableCORS(true);

  IPAddress adr = WiFi.localIP();
  if (adr[0] == 192) {
	strcpy(svars[0],"http://192.168.1.116:8000/"); // fz-m1
	strcpy(svars[1],"http://192.168.1.171:8081/"); // cf-20
  }
  else {
	strcpy(svars[0],"http://172.16.5.67:8000/");
	strcpy(svars[1],"http://172.16.5.66:8081/");
  }

  server.on("/", HTTP_GET, []() {
    IPAddress adr = WiFi.localIP();
    sprintf(buffer, "%d.%d.%d.%d", adr[0], adr[1], adr[2], adr[3]);
    server.send(200, "text/html", buffer);
  });

  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });

  server.on("/getState", HTTP_GET, []() {
    sprintf(buffer, "{\"state\":%d,\"vars\":[%d,%d,%d,%d,%d],\"stringvars\":[\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"]}",
	 stateMachine.getState(),
	 vars[0],vars[1],vars[2],vars[3],vars[4],
	 svars[0],svars[1],svars[2],svars[3],svars[4]);
    server.send(200, "application/json", buffer);
  });

  server.on("/setState", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    if (server.args() == 0) {
      return server.send(500);
    }

    server.arg(0).toCharArray(buffer, BUFFER_SIZE);

    Serial.print("Recevied POST /state: ");
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

  server.on("/setConfig", HTTP_POST, []() {
    int i = 0;
    if (server.args() == 0) {
      return server.send(500);
    }

    server.arg(0).toCharArray(buffer, BUFFER_SIZE);

    char* command = strtok(buffer, ",");
    while (command != NULL && i < 5)
    {
      strcpy(svars[i], command);
      ++i;
      command = strtok(0, ",");
    }

    server.send(200);
  });

  server.on("/print", HTTP_POST, []() {
    if (server.args() == 0) {
      return server.send(500);
    }

    server.arg(0).toCharArray(name, BUFFER_SIZE);

    Serial.print("Recevied POST /print: ");
    Serial.println(name);

    server.send(200);
  });

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
