/*
 * Simple interface to the Fly Sky IBus RC system.
 */

#include <inttypes.h>

class HardwareSerial;
class Stream;

class FlySkyIBus
{
public:
  void begin(HardwareSerial& serial);
  void begin(Stream& stream);
  void loop(void);
  uint16_t readChannel(uint8_t channelNr);

private:
  enum State
  {
    WAIT_SYNC0,
    WAIT_SYNC1,
    GET_DATA,
    GET_CHKSUML,
    GET_CHKSUMH
  };

  static const uint8_t PROTOCOL_SYNC0 = 0x20; // Actually packet length
  static const uint8_t PROTOCOL_SYNC1 = 0x40; // Actually command byte
  static const uint8_t PROTOCOL_LENGTH = 28;
  static const uint8_t PROTOCOL_TIMEOUT = 2;
  static const uint8_t PROTOCOL_CHANNELS = PROTOCOL_LENGTH / 2;

  uint8_t state;
  Stream* stream;
  uint32_t last;
  uint8_t buffer[PROTOCOL_LENGTH];
  uint8_t ptr;
  uint16_t channel[PROTOCOL_CHANNELS];
  uint16_t chksum;
  uint8_t lchksum;
};

extern FlySkyIBus IBus;
