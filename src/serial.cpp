#include <Arduino.h>

#include "serial.h"

/**
 * Swaps the Serial (intention: to USB).
 * Note: Same as swap_serial_to_zb, but other delays.
*/
void swap_serial_to_usb()
{
  delay(300);
  Serial.swap();
  delay(300);
  empty_serial(); // remove what's in the buffer
}

/** Swaps the Serial (intention: to zigbee module).
 * Note: Same as swap_serial_to_zb, but other delays.
*/
void swap_serial_to_zb()
{
  delay(200); // was 300
  Serial.swap();
  delay(100);
  empty_serial(); // remove what's in the buffer
}

/** deplete the Serial buffer (read all remaining data). */
void empty_serial()
{ // remove any remaining data in serial buffer
  while (Serial.available())
  {
    Serial.read();
  }
}
