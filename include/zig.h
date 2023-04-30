#ifndef _ZIG_H
#define _ZIG_H

#include <Arduino.h>

#include "logger.h"

#define CC2530_MAX_SERIAL_BUFFER_SIZE 512

enum class ZigbeeState {
  UP,
  INITIALIZING,
  DOWN
};

class Zig
{
public:
  Zig(Stream * serial, Logger * logger);

  ZigbeeState state;

  bool ping();
  int checkCoordinator(size_t maxRetries=3);

private:
  Stream * stream;
  Logger * logger;

  size_t sendCmd(const byte * cmd,     size_t len);
  size_t sendRaw(const byte * payload, size_t len);
};

#endif
