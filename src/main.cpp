#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);

#include "board.h"
#include "zigbee.h"
#include "webserver.h"
#include "serial.h"

#define AP_SSID "ECU-ESP8266 (sunharvest)"

/*
 * Feature Ideas:
 *   -> Real Captive Portal for easier access.
 *   -> MultiMode to feed into another network
 *   -> Button to switch on / off Wifi
 *   -> LCD Display
 */

#include "ecu.h"

ecu ECU = {
  .id          = "D8A3011B9780",
  .id_reverse  = "80971B01A3D8",
  .next_action = ACTION_NOP
};

/** Initial setup of board, wifi, webserver, zigbee-module, ... */
void setup()
{
  Serial.begin(115200);
  Serial.print("ECU-ESP8266");

  delay(3000);

  setup_led();
  blink_led(2, 200);

  Serial.print("Starting soft-AP ... ");
  if (!WiFi.softAP(AP_SSID)) {
    Serial.println("Failed!");
    return;
  }

  Serial.println("Ready");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  start_webserver();

  Serial.println(F("Switching serial to Zigbee module now. Last message here, goodbye"));
  // From now on, `Serial.xyz`() goes to the CC25XX zigbee module.
  swap_serial_to_zb();
}

/** Consume ECU.next_action out of async web server thread (which does not survive delay()s or yields). */
void doNextAction()
{
  Serial.print("Handle action: ");
  Serial.println(ECU.next_action);

  switch (ECU.next_action)
  {
    case ACTION_NOP:
      break;

    case ACTION_START_COORDINATOR:
      coordinator(true);
      break;

    default:
      Serial.println("Dont know how to handle this command!");
  }

  // We assume that we have only one action in our queue ...
  ECU.next_action = ACTION_NOP;
}

/** Your typical endless loop. */
void loop()
{
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
  doNextAction();
}


