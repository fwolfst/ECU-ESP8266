#ifndef _ZIG_COORDINATOR_H
#define _ZIG_COORDINATOR_H

class Zig; // forward decl
class Logger; // forward decl

namespace ZigZig {
  class Coordinator {
    public:
      Coordinator(Zig * zig, Logger * logger);

    private:
      Zig * zig;
      Logger * logger;
  };
}

#endif