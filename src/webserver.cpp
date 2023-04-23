#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

#include "webserver.h"

#include "ecu.h"

extern AsyncWebServer server;

void start_webserver() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("request");
    ASYNC_ACTION_START_COORDINATOR();
    request->send(200, "text/html", "Hello World <a href=\"/\">refresh</a>");
  });

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("request");
    request->send(200, "text/html", "Hello World; test");
  });

  server.begin();
}
