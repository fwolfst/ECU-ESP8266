#ifndef _LOGGER_H
#define _LOGGER_H

#include "ESPAsyncWebServer.h"

enum class LogLevel {
  DEBUG,
  INFO,
  WARN
};

static const char logMsgJsonFmtDebug[] = "{\"type\":\"log\",\"level\":\"debug\",\"message\":\"%s\"}";
static const char logMsgJsonFmtInfo[]  = "{\"type\":\"log\",\"level\":\"info\",\"message\":\"%s\"}";

/**
 * A (websocket) logger.
 * Implemented to remove dependency to web stuff from users.
 * Will print given messages to all websocket clients if log level permits.
 */
class Logger {
public:
  Logger(AsyncWebSocket * websocket, LogLevel llevel = LogLevel::INFO);
  void debug(const char * message);

  /** Use sprintf like debug message. */
  template<typename...Args>
  void debugf(const char * fmt, Args... args) {
    // implemented here in header because of use of template.
    if (level > LogLevel::DEBUG) {
      return;
    }

    char msg[256 - sizeof(logMsgJsonFmtDebug)] = { 0 };
    snprintf(msg, 256 - sizeof(logMsgJsonFmtDebug), fmt, args...);

    char message[256] = { 0 };
    snprintf(message, 256, logMsgJsonFmtDebug, msg);

    //websocket->printfAll(fmt, args...);
    websocket->printfAll(message);
  }

  void info(const char * message);

  /** Use sprintf like info message. */
  template<typename...Args>
  void infof(const char * fmt, Args... args) {
    // implemented here in header because of use of template.
    if (level > LogLevel::INFO) {
      return;
    }

    char msg[256 - sizeof(logMsgJsonFmtInfo)] = { 0 };
    snprintf(msg, 256 - sizeof(logMsgJsonFmtInfo), fmt, args...);

    char message[256] = { 0 };
    snprintf(message, 256, logMsgJsonFmtInfo, msg);

    //websocket->printfAll(fmt, args...);
    websocket->printfAll(message);
  }

  LogLevel level;

private:
  AsyncWebSocket * websocket;
};

#endif
