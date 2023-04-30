#ifndef _ZIG_H
#define _ZIG_H

#include <Arduino.h>

#include "logger.h"

#define CC2530_MAX_SERIAL_BUFFER_SIZE 512

// PINS
#define ZB_RESET          14 // D5
#define ZB_TX             15 // D8

enum class ZigbeeState {
  UP,
  INITIALIZING,
  DOWN
};

/**
 * Zig/bee class to setup the cc2530 module and coordinator.
*/

class Zig
{
public:
  Zig(Stream * serial, Logger * logger);

  ZigbeeState state;

  bool ping();
  int checkCoordinator(size_t maxRetries=3);
  void initCoordinator(byte * ecuid, byte * ecuid_reverse);
  void resetHard();
  void setupBoard();

private:
  Stream * stream;
  Logger * logger;

  size_t sendCmd(const byte * cmd,     size_t len);
  size_t sendRaw(const byte * payload, size_t len);
};

#endif
