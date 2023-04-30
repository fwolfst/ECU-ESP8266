#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

#include "board.h"
#include "logger.h"
#include "webserver.h"
#include "serial.h"
#include "zig.h"

#define AP_SSID "ECU-ESP8266 (sunharvest)"

#include "ecu.h"

ecu ECU = {
  .id          = "D8A3011B9780",
  .id_reverse  = "80971B01A3D8",
  .ecuid         = { 0xD8, 0xA3, 0x01, 0x1B, 0x97, 0x80 },
  .ecuid_reverse = { 0x80, 0x97, 0x1B, 0x01, 0xA3, 0xD8 },
  .next_action = ACTION_NOP,
  .logger      = Logger(&websocket, LogLevel::DEBUG),
  .zig         = NULL
};

/** Initial setup of board, wifi, webserver, zigbee-module, ... */
void setup()
{
  Serial.begin(115200);
  Serial.print("ECU-ESP8266");

  delay(3000);

  setup_led();
  blink_led(3, 125);

  Serial.print("Starting soft-AP ... ");
  if (!WiFi.softAP(AP_SSID)) {
    Serial.println("Failed!");
    return;
  }

  Serial.println("Ready");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  start_webserver(server, websocket);

  Serial.println(F("Switching serial to Zigbee module now. Last message here, goodbye"));
  // From now on, `Serial.xyz`() goes to the CC25XX zigbee module.
  swap_serial_to_zb();
  ECU.zig = new Zig(&Serial, &ECU.logger);
  ECU.zig->setupBoard();

  delay(200);
  blink_led(4, 200);

  ECU.logger.info("Logger online");
}

/** Consume ECU.next_action out of async web server thread (which does not survive delay()s or yields). */
void doNextAction()
{
  ECU.logger.infof("Handle action: %d", ECU.next_action);

  switch (ECU.next_action)
  {
    case ACTION_NOP:
      break;

    case ACTION_START_COORDINATOR:
      ECU.next_action = ACTION_NOP;
      //coordinator(false);
      ECU.zig->initCoordinator((byte*)&ECU.ecuid, (byte*)&ECU.ecuid_reverse);
      break;

    default:
      ECU.logger.info("Dont know how to handle this command!");
  }

  // We assume that we have only one action in our queue ...
  ECU.next_action = ACTION_NOP;
}

/** Your typical endless loop. */
void loop()
{
  //Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(1000);
  doNextAction();
}


