#include "logger.h"

Logger::Logger(AsyncWebSocket * logWebsocket, LogLevel logLevel) {
  websocket = logWebsocket;
  level = logLevel;
}

/** Send a debug message if level is DEBUG. */
void Logger::debug(const char * msg) {
  if (level > LogLevel::DEBUG) {
    return;
  }

  websocket->printfAll(msg);
}
 
/* // implemented in header file.
  template<typename...Args>
  void Logger::debugf(const char * fmt, Args... args) {
    if (level > LogLevel::DEBUG) {
      return;
    }

    websocket->printfAll(fmt, args...);
  }
*/

/** Send a info message if level is INFO or DEBUG. */
void Logger::info(const char * msg) {
  if (level > LogLevel::WARN) {
    return;
  }

  websocket->printfAll(msg);
}

