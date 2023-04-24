#ifndef _ZIGBEE_H
#define _ZIGBEE_H

// States of the coordinator
#define ZIGBEE_STATE_INITIALIZING 11
#define ZIGBEE_STATE_DOWN          0
#define ZIGBEE_STATE_UP            1

#define CC2530_MAX_SERIAL_BUFFER_SIZE 512

// PINS
#define ZB_RESET          14 // D5
#define ZB_TX             15 // D8

bool coordinator(bool normal);
void setup_zigbee_board();

#endif