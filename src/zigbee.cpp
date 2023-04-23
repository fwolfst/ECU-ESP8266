#include <Arduino.h>

#include "zigbee.h"
#include "board.h"
#include "ecu.h"

// forward decl
void coordinator_init();
uint8_t checkCoordinator();
void sendNO();
void sendZigbee(char *command);
void readZigbee();
void ZBhardReset();
inline void empty_serial();

extern ecu ECU;


/** Initializes the Zigbee coordinator, returning true iff successfull. */
bool coordinator(bool normal)
{ // if true we send the extra command for normal operations
  Serial.println("Starting zigbee coordinator");
  Serial.println("ECU is");
  Serial.println(ECU.id);

  coordinator_init();

  if (normal)
    sendNO();

  // now check if running
  delay(1000);                               // to give the ZB the time to start
  if (checkCoordinator() == ZIGBEE_STATE_UP) // can be 0 1 or 2
  {
    blink_led(5, 100);
    return true;
  }
  else
  {
    return false;
  }
}

/** Send normal command. */
void sendNO()
{
  char initCmd[254] = {0}; //  we have to send the 10th command

  char lastCmd[][100] = {
      "2401FFFF1414060001000F1E",
      "FBFB1100000D6030FBD3000000000000000004010281FEFE",
  };

  // send command 9 this is "2401FFFF1414060001000F1E", + ecu_id_reverse + FBFB1100000D6030FBD3000000000000000004010281FEFE"
  strncat(lastCmd[0], ECU.id_reverse, sizeof(lastCmd[0]) - strlen(lastCmd[0]) - 1);

  // Serial.println("initCmd 9 after ecu_reverse = " + String(lastCmd));
  delayMicroseconds(250);

  // Prepend with length (in hex as string)
  snprintf(initCmd, sizeof(initCmd), "%02X%s", strlen(lastCmd[0]) / 2 - 2, lastCmd[0]);

  delayMicroseconds(250);

  // put in the CRC at the end of the command now done by sendZigbee()
  Serial.flush();

  sendZigbee(initCmd);
  // delay(1000); // give the inverter the chance to answer
  // check if anything was received
  waitSerialAvailable();

  readZigbee();

  // zero out
  // memset(&initCmd, 0, sizeof(initCmd)); // zero out all buffers we could work with "messageToDecode"
  delayMicroseconds(250);
  // memset(&lastCmd, 0, sizeof(lastCmd)); // zero out all buffers we could work with "messageToDecode"
  delayMicroseconds(250);
}

/** Send initialization commands to coordinator */
void coordinator_init()
{
  // zigbeeUp = ZIGBEE_STATE_INITIALIZING;
  yield();

  // init the coordinator takes the following procedure
  // 1st we send a resetcommand 4 times Sent=FE0141000040
  // then we send the following commands
  // 34:  0 Sent=FE03260503010321
  // 35: Received=FE0166050062
  // 39:  1 Sent=FE0141000040
  // 40: Received=FE064180020202020702C2
  // 44:  2 Sent=FE0A26050108FFFF80971B01A3D856
  // 45: Received=FE0166050062
  // 49:  3 Sent=FE032605870100A6
  // 50: Received=FE0166050062
  // 54:  4 Sent=FE 04 26058302 D8A3 DD  should be ecu_id the fst 2 bytes
  // 55: Received=FE0166050062
  // 59:  5 Sent=FE062605840400000100A4
  // 60: Received=FE0166050062
  // 64:  6 Sent=FE0D240014050F0001010002000015000020
  // 65: Received=FE0164000065
  // 69:  7 Sent=FE00260026
  // 74:  8 Sent=FE00670067
  // 75:  Received=FE0145C0098D
  // received FE00660066 FE0145C0088C FE0145C0098D F0F8FE0E670000FFFF80971B01A3D8000007090011
  //     9 for normal operastion we send cmd 9
  // 79: Finished. Heap=26712
  // now we can pair if we want to or else an extra command for retrieving data

  char initCmd[254] = {0};

  // commands for setting up coordinater
  char initBaseCommand[][254] = {
      "2605030103",                     // ok   this is a ZB_WRITE_CONFIGURATION CMD //changed to 01
      "410000",                         // ok   ZB_SYS_RESET_REQ
      "26050108FFFF",                   // + ecu_id_reverse,  this is a ZB_WRITE_CONFIGURATION CMD
      "2605870100",                     // ok
      "26058302",                       // + ecu_id.substring(0,2) + ecu_id.substring(2,4),
      "2605840400000100",               // ok
      "240014050F00010100020000150000", // AF_REGISTER register an application’s endpoint description
      "2600",                           // ok ZB_START_REQUEST
  };
  //  "6700", // the checkZigbeeRadio we can skip this, instead do checkZigbeeRadio

  // we start with a hard reset of the zb module
  ZBhardReset();
  delay(500);

  // construct some of the commands
  // ***************************** command 2 ********************************************
  // command 2 this is 26050108FFFF we add ecu_id reversed
  strcat(initBaseCommand[2], ECU.id_reverse);
  delayMicroseconds(250);
  // Serial.println("initCmd 2 constructed = " + String(initBaseCommand[2]));  // ok

  // ***************************** command 4 ********************************************
  // command 4 this is 26058302 + ecu_id_short
  strncat(initBaseCommand[4], ECU.id, 2);
  strncat(initBaseCommand[4], ECU.id + 2, 2);
  delayMicroseconds(250);
  // Serial.println("initCmd 4 constructed = " + String(initBaseCommand[4]));

  // send the rest of the commands
  for (int y = 0; y < 8; y++)
  {
  // cmd 0 tm / 9 alles ok
  // add sln at the beginning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(initCmd, sizeof(initCmd), "%02X%s", strlen(initBaseCommand[y]) / 2 - 2, initBaseCommand[y]);
#pragma GCC diagnostic pop

    delayMicroseconds(250);

    sendZigbee(initCmd);
    blink_led(1, 50);
    // delay(1000); // give the inverter the chance to answer
    // check if anything was received
    waitSerialAvailable();
    readZigbee();
  }
  // now all the commands are send
}

/** Send the 2700 command,
 * The answer can mean that the coordinator is up, not yet
 * started or no answer (ZIGBEE_STATE_UP, ZIGBEE_STATE_DOWN).
 */
uint8_t checkCoordinator()
{
   char *tail;

   // the response = 67 00, status 1 bt, IEEEAddr 8bt, ShortAddr 2bt, DeviceType 1bt, Device State 1bt
   //  FE0E 67 00 00 FFFF 80971B01A3D8 0000 0709001
   // status = 00 means succes, IEEEAddress= FFFF80971B01A3D8, ShortAdr = 0000, devicetype=07 bits 0 to 2

   // Device State 09 started as zigbeecoordinator

   // Check the radio, send FE00670067
   // when ok the returned string = FE0E670000FFFF + ECU_ID REVERSE + 00000709001


     char checkCommand[10]; // we send 2700 to the zb
     strncpy(checkCommand, "002700", 7);

     // first clean up the serial port
     empty_serial();

/*
     for (int x = 0; x < 3; x++)
     {
       sendZigbee(checkCommand);
       if (waitSerialAvailable())
       {
         readZigbee();
       }
       else
       {
         // TODO readCounter = 0;
       } // when nothing available we don't read
       
       // we get this : FE0E670000 FFFF80971B01A3D8 0000 07090011
       //    received : FE0E670000FFFF80971B01A3D600000709001F when ok

       // check if ecu_id_reverse is in the string, then split it there + 2 bytes
       if (strstr(inMessage, ECU.id_recerse))
       {
         // ws.textAll("found ecu id");
         tail = split(inMessage, ECU.id_recerse + 4);
         // ws.textAll("tail=" + String(tail));
         // Serial.println("\nhealth received : " + String(inMessage) );
         if (strstr(tail, "0709"))
         {
           return ZIGBEE_STATE_UP;
         }
       }
       delay(700);
     }
     */

  // if we come here 3 attempts failed
  return ZIGBEE_STATE_DOWN;
}

/**
 * Return the integer that is encoded by a hex string.
 * E.g. '0x11' -> 17, '2A' -> 42).
 */
inline int HexStrToInt(char *str)
{
  return (int)strtol(str, 0, 16);
}

/**
 * Calculate the checksum of a message and copies it into dst (max length 3).
 */
void checkSum(char *message, char *dst)
{
  char bufferCRC[3] = {0};
  char bufferCRC_2[3] = {0};

  strncpy(bufferCRC, message, 2); // as starting point perhaps called "seed" use the first two chars from "message"
  delayMicroseconds(250);         // give memset a little bit of time to empty all the buffers

  for (uint8_t i = 1; i <= (strlen(message) / 2 - 1); i++)
  {
    strncpy(bufferCRC_2, message + i * 2, 2); // use every iteration the next two chars starting with char 2+3
    delayMicroseconds(250);                   // give memset a little bit of time to empty all the buffers
    sprintf(bufferCRC, "%02X", HexStrToInt(bufferCRC) ^ HexStrToInt(bufferCRC_2));
    delayMicroseconds(250); // give memset a little bit of time to empty all the buffers
  }

  strncpy(dst, bufferCRC, 3);
}

/**
 * Send data (command) to zigbee radio.
 * The command needs to follow the conventions (length should already be prepended).
 * A CRC checksum will be added at the end, and Serial flushed.
 */
void sendZigbee(char *command)
{
  char bufferSend[3];

  // Fill buffer in chunks of 2
  if (Serial.availableForWrite() > (uint8_t)strlen(command))
  {
    Serial.write(0xFE); // we have to send "FE" at start of each command
    for (uint8_t i = 0; i <= strlen(command) / 2 - 1; i++)
    {
      // we use 2 characters to make a byte
      strncpy(bufferSend, command + i * 2, 2);
      delayMicroseconds(250);                // give memset a little bit of time to empty all the buffers
      Serial.write(HexStrToInt(bufferSend)); // turn the two chars to a byte and send this
    }

    // Add CRC checksum
    checkSum(command, bufferSend);
    Serial.write(HexStrToInt(bufferSend));

    Serial.flush(); // wait till the full command was sent
  }
}

/** format (hex str) the incoming byte and add it to inBuffer */
void appendIncomingByte(char *inBuffer, const byte inByte)
{
  char oneChar[3] = {0};
  sprintf(oneChar, "%02X", inByte);
  strncat(inBuffer, oneChar, 2); // append
}

void cleanIncoming(char *buffer)
{
  // with swaps we get F8 sometimes, this removes it
  if (buffer[0] == 'F' && buffer[1] == '8')
  {
    strcpy(buffer, &buffer[2]);
  }
}

/** deplete the Serial buffer (read all remaining data)*/
inline void empty_serial()
{
  while (Serial.available())
  {
    Serial.read();
  }
}

/** read zigbee radio at swapped serial.
 * this function is somewhat unefficient but that seems an advantage due to the
 * slow reaction from the inverter
 */
void readZigbee()
{
  uint8_t readCounter = 0;
  char inMessage[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};

  delayMicroseconds(250);

  while (Serial.available())
  {
    // here we have the danger that when readcounter reaches 512, there are 1024 bytes processed
    // the length of a poll answer is usually not more than 223
    if (readCounter < CC2530_MAX_SERIAL_BUFFER_SIZE / 2)
    {
      appendIncomingByte(inMessage, Serial.read());
      readCounter += 1;
    }
    else
    {
      // Serial.read(); // we read from serial to empty the buffer but do not process
      ESP.wdtFeed();
      empty_serial(); // remove all excess data in the buffer at once
    }
    if (Serial.available() == 0)
      delay(120); // we wait if there comes more data
  }

  // Now we should have catched inMessage

  cleanIncoming(inMessage); // check for F8 and remove it
}

/** hard reset the cc25xx */
void ZBhardReset()
{
  digitalWrite(ZB_RESET, LOW);
  delay(500);
  digitalWrite(ZB_RESET, HIGH);
  delay(2000); // wait for the cc2530 to reboot
}
