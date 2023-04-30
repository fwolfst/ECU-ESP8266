#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include "ESPAsyncWebServer.h"

void start_webserver(AsyncWebServer &server, AsyncWebSocket &websocket);

#endif