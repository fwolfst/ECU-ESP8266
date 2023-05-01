#ifndef _ECU_H
#define _ECU_H

#include <Arduino.h>
#include "logger.h"
#include "zig.h"

typedef struct ecu_id { byte x[6]; } ecu_id;

/** Configuration, state and properties of an ECU. */
typedef struct ecu {
  char id[13];
  char id_reverse[13];
  ecu_id ecuid;
  ecu_id ecuid_reverse;
  uint8_t next_action; // Set e.g. by the async webserver
  Logger logger;
  Zig * zig;
} ecu;

extern ecu ECU;

#define ACTION_NOP                0
#define ACTION_START_COORDINATOR 20
#define ACTION_PING_COORDINATOR  22
#define ACTION_CHECK_COORDINATOR 25

#define ASYNC_ACTION_NOP() ECU.next_action = ACTION_NOP;
#define ASYNC_ACTION_START_COORDINATOR() ECU.next_action = ACTION_START_COORDINATOR
#define ASYNC_ACTION_PING_COORDINATOR()  ECU.next_action = ACTION_PING_COORDINATOR
#define ASYNC_ACTION_CHECK_COORDINATOR()  ECU.next_action = ACTION_CHECK_COORDINATOR

#endif
