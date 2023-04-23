#ifndef _ECU_H
#define _ECU_H

#include <Arduino.h>

/** Configuration, state and properties of an ECU. */
typedef struct ecu {
  char id[13];
  char id_reverse[13];
  uint8_t next_action; // Set e.g. by the async webserver
} ecu;

extern ecu ECU;

#define ACTION_NOP                0
#define ACTION_START_COORDINATOR 20

#define ASYNC_ACTION_NOP() ECU.next_action = ACTION_NOP;
#define ASYNC_ACTION_START_COORDINATOR() ECU.next_action = ACTION_START_COORDINATOR

#endif
