/*
 * Simple interface to the Fly Sky IBus RC system.
 */

#include <Arduino.h>
#include "FlySkyIBus.h"

FlySkyIBus IBus;

void FlySkyIBus::begin(HardwareSerial& serial)
{
  serial.begin(115200);
  begin((Stream&)serial);
}

void FlySkyIBus::begin(Stream& stream)
{
  this->stream = &stream;
  this->state = WAIT_SYNC0;
  this->last = millis();
  this->ptr = 0;
  this->chksum = 0;
  this->lchksum = 0;
}

void FlySkyIBus::loop(void)
{
  while (stream->available() > 0)
  {
    uint32_t now = millis();
    if (now - last >= PROTOCOL_TIMEOUT)
    {
      state = WAIT_SYNC0;
    }
    last = now;
    
    uint8_t v = stream->read();
    switch (state)
    {
      case WAIT_SYNC0:
        if (v == PROTOCOL_SYNC0)
        {
          state = WAIT_SYNC1;
        }
        break;
        
      case WAIT_SYNC1:
        if (v == PROTOCOL_SYNC1)
        {
          state = GET_DATA;
          ptr = 0;
          chksum = 0xFFFF - PROTOCOL_SYNC0 - PROTOCOL_SYNC1;
        }
        break;

      case GET_DATA:
        buffer[ptr++] = v;
        chksum -= v;
        if (ptr == PROTOCOL_LENGTH)
        {
          state = GET_CHKSUML;
        }
        break;
        
      case GET_CHKSUML:
        lchksum = v;
        state = GET_CHKSUMH;
        break;

      case GET_CHKSUMH:
        // Validate checksum before copying data to channels
        if (chksum == (v << 8) + lchksum)
        {
          // Valid - extract channel data
          for (uint8_t i = 0; i < PROTOCOL_LENGTH; i += 2)
          {
            channel[i / 2] = buffer[i] | (buffer[i + 1] << 8);
          }
        }
        state = WAIT_SYNC0;
        break;

      default:
        break;
    }
  }
}

uint16_t FlySkyIBus::readChannel(uint8_t channelNr)
{
  if (channelNr < PROTOCOL_CHANNELS)
  {
    return channel[channelNr];
  }
  else
  {
    return 0;
  }
}
