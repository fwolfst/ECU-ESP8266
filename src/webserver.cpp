#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

#include "webserver.h"
#include "html/home.html.h"
#include "js/js.h"
#include "css/mypico.css.h"

#include "ecu.h"


void handle_websocket_event(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    client->text("Hello from ECU_ESP8266");
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    //Serial.println("Client disconnected");
  }
}

/** Register routes and callbacks, start webserver. */
void start_webserver(AsyncWebServer &server, AsyncWebSocket &websocket)
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    ASYNC_ACTION_START_COORDINATOR();
    request->send_P(200, "text/html", HOMEPAGE_HTML); });

  server.on("/js.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    ASYNC_ACTION_START_COORDINATOR();
    request->send_P(200, "text/html", JS_JS); }); //mimetype? TODO

  server.on("/mypico.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    ASYNC_ACTION_START_COORDINATOR();
    request->send_P(200, "text/css", MYPICO_CSS); });

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send(200, "text/html", "Hello World; test"); });

  server.on("/coordinator", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    ASYNC_ACTION_START_COORDINATOR();
    request->send_P(200, "text/html", HOMEPAGE_HTML); });

  websocket.onEvent(handle_websocket_event);
  server.addHandler(&websocket);

  server.begin();
}
