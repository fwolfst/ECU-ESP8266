#include "zig.h"

/** Create a Zig instance. `serial` should be the Serial that is written to
 * (CC2530 module). */
Zig::Zig(Stream * serial, Logger * logLogger)
{
  stream = serial;
  logger = logLogger;
  state  = ZigbeeState::DOWN;
}

/** Return byte for some kind of checksum (maybe CRC, all bytes XORed). */
inline uint8_t checksum(const unsigned char * msg, size_t len) {
  uint8_t buff = msg[0];
  for(size_t i = 1; i < len; i++) {
    buff ^= msg[i];
  }
  return buff;
}

/** Debug a message as hex, if the logger has debug level. */
static void debugHex(Logger * logger, const char * message, const unsigned char * payload, size_t len) {
  if (logger->level > LogLevel::DEBUG) {
    return;
  }

  char debugMsg[len * 2 + 1] = {0};
  for(size_t i = 0; i < len; ++i) {
    sprintf(debugMsg + (i * 2), "%02X", payload[i]);
  }

  logger->debugf("%s: %s", message, debugMsg);
  logger->debugf("Checksum %02X", checksum(payload, len));
}

/** Send ping command to module and return whether the right answer came back. */
// If the ping command fails we have to restart the coordinator
bool Zig::ping()
{
  logger->debug("Zig::ping()");
  
  // Ping-command 00210120 (00: length; payload: 2101 ; 20: checksum)
  unsigned char pingCmd[] = { 0x00, 0x21, 0x01 };

  // Emtpty the stream
  while (stream->available()) {
    stream->read();
  }

  send(pingCmd, 3);

  unsigned char inputFrame[CC2530_MAX_SERIAL_BUFFER_SIZE];

  stream->setTimeout(1200);
  size_t len = stream->readBytes(inputFrame, sizeof(inputFrame));

  logger->debugf("read %d bytes: ", len); // answer should be answer is FE02 6101 79 07 1C
  debugHex(logger, "Read: ", inputFrame, len);

return true;
  /*
  // search for
  char ping_answer[] = {0xFE 0x02 0x61 0x01};

  // if found
  if (strstr(inMessage, "FE026101") == NULL)
  {
     logger->info("Zig: no ping answer");
     return false;
  }
  else
  {
    logger->info("Zig: ping ok");
    return true;
  }
*/
}


/** Sends the message via zigbee module. Attaching a checksum at the end. */
uint8_t Zig::send(const unsigned char * msg, size_t len) {
  logger->debugf("Sending %d bytes (size %d)", len, sizeof(msg));

  uint8_t msg_sent = stream->write(msg, len);
  uint8_t crc_sent = stream->write(checksum(msg, len));

  debugHex(logger, "Sending: ", msg, len);

  return msg_sent + crc_sent;
}
