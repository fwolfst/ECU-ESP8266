#include "zig_coordinator.h"

#include "zig.h"
#include "logger.h"

namespace ZigZig {
  Coordinator::Coordinator(Zig * zi, Logger * log) {
    zig = zi;
    logger = log;
  }
}
