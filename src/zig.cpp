#include "zig.h"
#include <string.h> // for memmem

typedef struct cmd {
  size_t length;
  const byte * cmd;
} cmd_t;

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
  for (size_t i = 1; i < len; i++)
  {
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

/** Send Coordinator command to module and return whether the right answer came back. */
// If the Coordinator command fails we have to restart the coordinator
bool Zig::pingCoordinator()
{
  logger->debug("Zig::pingCoordinator()");

  // Ping-command 00210120 (00: length; payload: 2101 ; 20: checksum)
  unsigned char pingCmd[] = { 0x21, 0x01 };

  emptyStream();

  sendCmd(pingCmd, 2);

  unsigned char inputFrame[CC2530_MAX_SERIAL_BUFFER_SIZE];

  stream->setTimeout(1200);
  size_t len = stream->readBytes(inputFrame, sizeof(inputFrame));

  //logger->debugf("read %d bytes: ", len); // answer should be answer is FE02 6101 79 07 1C
  debugHex(logger, "Read: ", inputFrame, len);

  // Original code searched only for
  // char ping_answer[] = {0xFE 0x02 0x61 0x01}; 
  const byte answer_ok[] = { 0xFE, 0x02, 0x61, 0x01, 0x79, 0x07, 0x1C };

  if (memcmp(inputFrame, answer_ok, sizeof(answer_ok)) == 0) {
    logger->info("Zig: pingCoordinator(): true!");
    return true;
  }
  else {
    logger->info("Zig: pingCoordinator(): false!");
    return false;
  }
}

/** Send <head><size>cmd<checksum>. Returns number of bytes sent. */
size_t Zig::sendCmd(const byte * cmd, size_t len) {
  size_t total_len = len + 3; // +head +size +checksum
  logger->debugf("Sending %d bytes (msg pointer size %d)", total_len, sizeof(cmd));

  // Construct the message: FE<size>cmd<checksum>
  byte out[total_len] = { 0 };

  out[0] = 0xFE;             // Head
  out[1] = len - 2;          // Size

  memcpy(&out[2], cmd, len); // Payload
  byte checkSum = checksum(out + 1, len + 1);// - 1;
  out[len + 2] = checkSum;   // Checksum

  uint8_t bytes_sent = stream->write(out, total_len);
  stream->flush();

  debugPrintPayload(logger, "Sent: ", out, total_len);

  return bytes_sent;
}

/** Sends the message via zigbee module. In contrast to `sendCmd` which
 * wraps the packet in head, length and checksum, this method sends bytes as-is.
 * Returns the number of bytes send (ideally == len).
*/
size_t Zig::sendRaw(const byte * msg, size_t len)
{
  logger->debugf("Sending %d bytes (size %d)", len, sizeof(msg));

  uint8_t msg_sent = stream->write(msg, len);
  uint8_t crc_sent = stream->write(checksum(msg, len));

  debugHex(logger, "Sending: ", msg, len);

  return msg_sent + crc_sent;
}

int Zig::checkCoordinator(byte * ecuid_reverse, size_t maxRetries)
{
  logger->debug("checkCoordinator()");
  // Basically the 2700 command
  // The answer can mean that the coordinator is up, not yet started or no answer
  // we evaluate that.

  // The response = 67 00, status 1 bt, IEEEAddr 8bt, ShortAddr 2bt, DeviceType 1bt, Device State 1bt
  //   FE0E 67 00 00 FFFF 80971B01A3D8 0000 0709001
  // status = 00 means succes, IEEEAddress= FFFF80971B01A3D8, ShortAdr = 0000, devicetype=07 bits 0 to 2

  // Device State 09 started as zigbeecoordinator

  // Check the radio, send FE00670067
  //   when OK the returned string = FE0E670000FFFF + ECU_ID REVERSE + 00000709001
  //     - so we check if we have this

  
  // strncpy(checkCommand, "00270027", 9); (raw)

  const unsigned char checkCmd[] = { 0x27, 0x00 };

  // first clean up the serial port
  emptyStream();

  // Try a couple of times
  for (size_t x = 0; x < maxRetries; ++x)
  {
    sendCmd(checkCmd, sizeof(checkCmd));

    unsigned char inputFrame[CC2530_MAX_SERIAL_BUFFER_SIZE];

    // TODO Do we need manual waiting? Or does the timeout actually work?
    stream->setTimeout(1200);
    size_t len = stream->readBytes(inputFrame, sizeof(inputFrame));

    logger->debugf("read %d bytes: ", len); // answer should be answer is FE02 6101 79 07 1C
    debugHex(logger, "Read: ", inputFrame, len);

    // We get this : FE0E670000 FFFF80971B01A3D8 0000 07090011
    //    received : FE0E670000 FFFF80971B01A3D6 0000 0709001F when ok

    // We lookg for 07 09, but after the ecu-id
    byte * ecuid_pos = (byte *) memmem((void*)inputFrame, len, (void*) ecuid_reverse, 6);
    if (ecuid_pos) {
      logger->debugf(" - contains the reverse ecu id %hhX", ecuid_reverse);

      size_t offset = ecuid_pos - inputFrame;
      byte marker[] = { 0x07, 0x09 };

      if (memmem(ecuid_pos + 2, len - (offset + 2), marker, 2 )) {
        logger->debugf(" - contains marker! %hhX", marker);

        //return true;
      }
    }

    logger->info("Retrying...");
  }

  // if we come here 3 attempts failed
  return 2;
}


// TODO this one is important and TODO :)
void Zig::initCoordinator(byte * ecuid, byte * ecuid_reverse) {
  /* Initializing the coordinator takes the following procedure:
  *
  * 1st we send a resetcommand 4 times Sent=FE0141000040
  * then we send the following commands
  *  0 Sent=FE03260503010321
  *  Received=FE0166050062
  *  1 Sent=FE0141000040
  *  Received=FE064180020202020702C2
  *  2 Sent=FE0A26050108FFFF80971B01A3D856
  *  Received=FE0166050062
  *  3 Sent=FE032605870100A6
  *  Received=FE0166050062
  *  4 Sent=FE 04 26058302 D8A3 DD  should be ecu_id the fst 2 bytes
  *  Received=FE0166050062
  *  5 Sent=FE062605840400000100A4
  *  Received=FE0166050062
  *  6 Sent=FE0D240014050F0001010002000015000020
  *  Received=FE0164000065
  *  7 Sent=FE00260026
  *  8 Sent=FE00670067
  *  Received=FE0145C0098D
  *  received FE00660066 FE0145C0088C FE0145C0098D F0F8FE0E670000FFFF80971B01A3D8000007090011
  *  now we can pair if we want to or else an extra command for retrieving data (normal operation)
  *  9 for normal operation we send cmd 9
  *  Finished. Heap=26712
  */

  logger->info("Zigbee Coordinator  initialization");
  state = ZigbeeState::INITIALIZING;

  yield();

  /*
    // @patience4711
    // commands for setting up coordinater
    char initBaseCommand[][254] = {
      "2605030103",   // ok   this is a ZB_WRITE_CONFIGURATION CMD //changed to 01
      "410000",       // ok   ZB_SYS_RESET_REQ
      "26050108FFFF", // + ecu_id_reverse,  this is a ZB_WRITE_CONFIGURATION CMD
      "2605870100",   //ok
      "26058302",     // + ecu_id.substring(0,2) + ecu_id.substring(2,4),
      "2605840400000100", //ok
      "240014050F00010100020000150000", //AF_REGISTER register an application’s endpoint description
      "2600",         //ok ZB_START_REQUEST
    };
  */

  /*
  const byte initBaseCommand[][254] = {
    { 0x26, 0x05, 0x03, 0x01, 0x03 },       // ZB_WRITE_CONFIGURATION CMD //changed to 01
    { 0x41, 0x00, 0x00 },                   // ZB_SYS_RESET_REQ
    { 0x26, 0x05, 0x01, 0x08, 0xFF, 0xFF }, // + ecu_id_reverse, ZB_WRITE_CONFIGURATION CMD
    { 0x26, 0x05, 0x87, 0x01, 0x00 },       // ok
    { 0x26, 0x05, 0x83, 0x02 },             // + ecu_id.substring(0,2) + ecu_id.substring(2,4),
    { 0x26, 0x05, 0x84, 0x04, 0x00, 0x00, 0x01, 0x00 }, // ok
    { 0x24, 0x00, 0x14, 0x05, 0x0F, 0x00, 0x01, 0x01, 0x00, 0x02, 0x00, 0x00, 0x15, 0x00, 0x00 }, // AF_REGISTER register an application’s endpoint description
    { 0x26, 0x00 },                         // ZB_START_REQUEST
  };
  */

  // TODO this could be done in a loop, if we knew the inidividual entries sizes.
  const byte init_cmd1[] = { 0x26, 0x05, 0x03, 0x01, 0x03 };
  const byte init_cmd2[] = { 0x41, 0x00, 0x00 };
  const byte init_cmd3[] = { 0x26, 0x05, 0x01, 0x08, 0xFF, 0xFF, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66}; // 0x66 placeholder for ecu_id_reverse;
  const byte init_cmd4[] = { 0x26, 0x05, 0x87, 0x01, 0x00 };
  const byte init_cmd5[] = { 0x26, 0x05, 0x83, 0x02, 0x66, 0x66 }; // 0x66 placeholder of first two bytes of ecu id
  const byte init_cmd6[] = { 0x26, 0x05, 0x84, 0x04, 0x00, 0x00, 0x01, 0x00 };
  const byte init_cmd7[] = { 0x24, 0x00, 0x14, 0x05, 0x0F, 0x00, 0x01, 0x01, 0x00, 0x02, 0x00, 0x00, 0x15, 0x00, 0x00 };
  const byte init_cmd8[] = { 0x26, 0x00 };

  // ** (size, pointer_to_cmd?)
  cmd_t commands[] = {
    { sizeof(init_cmd1), init_cmd1 },
    { sizeof(init_cmd2), init_cmd2 },
    { sizeof(init_cmd3), init_cmd3 },
    { sizeof(init_cmd4), init_cmd4 },
    { sizeof(init_cmd5), init_cmd5 },
    { sizeof(init_cmd6), init_cmd6 },
    { sizeof(init_cmd7), init_cmd7 },
    { sizeof(init_cmd8), init_cmd8 },
  };

  // hard reset of the zb module
  resetHard();
  delay(500);

  unsigned char inputFrame[CC2530_MAX_SERIAL_BUFFER_SIZE] = { 0 };
  size_t read_len;

  memcpy((void*) init_cmd3 + 6, ecuid_reverse, 6);
  memcpy((void*) init_cmd5 + 4, ecuid, 2);

  for (size_t cmd_idx = 0; cmd_idx < sizeof(commands) / sizeof(cmd_t); cmd_idx++) {
    logger->infof("Sending initCoordinator cmd %d", cmd_idx);
    sendCmd(commands[cmd_idx].cmd, commands[cmd_idx].length);

    read_len = stream->readBytes(inputFrame, sizeof(inputFrame));
    debugHex(logger, "response: ", inputFrame, read_len);
  }
}

/** Hard reset the cc25xx module*/
void Zig::resetHard()
{
  logger->info("Zigbee: Hard Reset");

  digitalWrite(ZB_RESET, LOW);
  delay(500);
  digitalWrite(ZB_RESET, HIGH);

  delay(2000); // wait for the cc2530 to reboot
}

/** Set pin modes accordingly to speak with our CC25XX */
void Zig::setupBoard()
{
  pinMode(ZB_TX, OUTPUT);
  digitalWrite(ZB_TX, LOW);

  // resetpin cc2530
  pinMode(ZB_RESET, OUTPUT);
  digitalWrite(ZB_RESET, HIGH);
}