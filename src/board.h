#ifndef _BOARD_H
#define _BOARD_H

#include <Arduino.h>

void blink_led(uint8_t count, uint8_t gap_time);
bool waitSerialAvailable();

#endif