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
  uint8_t send(const unsigned char * msg, size_t len);

private:
  Stream * stream;
  Logger * logger;
};

#endif
