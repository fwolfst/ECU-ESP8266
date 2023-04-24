#include <Arduino.h>

#include "board.h"

#define LED_ON LOW // sinc
#define LED_OFF HIGH
#define ONBOARD_LED 2 // onboard led was 2

/** Enable onboard LED (output mode). */
void setup_led(){
  pinMode(ONBOARD_LED, OUTPUT);
}

/** blink the builtin LED count times, with gap_time between. */
void blink_led(uint8_t count, uint8_t gap_time)
{
  for (int x = 0; x < count; x++)
  {
    digitalWrite(ONBOARD_LED, LED_ON);
    delay(gap_time);
    digitalWrite(ONBOARD_LED, LED_OFF);
    delay(gap_time);
  }
}

/** Wait until something is available, return false if there is nothing after 2000 ms. */
bool waitSerialAvailable()
{
  // We start with a minimum delay
  // delay(800);
  unsigned long wait = millis();
  while (!Serial.available())
  {
    if (millis() - wait > 2000)
      return false; // return false after 2000 milis time out
  }
  // if we are here there is something available
  delay(500);
  return true;
}