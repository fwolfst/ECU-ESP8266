#ifndef _ZIG_H
#define _ZIG_H

#include <Arduino.h>

#include "logger.h"
#include "ecu.h"

#define CC2530_MAX_SERIAL_BUFFER_SIZE 512

// PINS
#define ZB_RESET          14 // D5
#define ZB_TX             15 // D8

enum class ZigbeeState {
  UP,
  INITIALIZING,
  DOWN
};

// Class has following responsabilities:
// - communication with serial/zb like sending and reading
// - setup routines for the modules and healthchecks
// - communication with inverter, like pairing and polling
//   -> these are slightly too many

/**
 * Zig/bee class to setup the cc2530 module and coordinator.
*/
class Zig
{
public:
  Zig(Stream * serial, Logger * logger);

  ZigbeeState state;

  void setupBoard();
  void resetHard();

  /** TODO: use types for ids */
  int checkCoordinator(const byte * ecuid_reverse, size_t maxRetries=3);
  void sendNormalOp();
  void initCoordinator(const byte * ecuid, const byte * ecuid_reverse);
  bool pingCoordinator();
  /* TODO is typedef passed by ref? */
  bool pairing(inverterSerialNumber_t inverterSn, ecu_id_reverse_t ecu_id_reverse, ecu_id_t ecu_id);
  bool pairInverter();
  

private:
  Stream * stream;
  Logger * logger;

  size_t sendCmd(const byte * cmd,     size_t len);
  size_t sendRaw(const byte * payload, size_t len);
};

#endif
