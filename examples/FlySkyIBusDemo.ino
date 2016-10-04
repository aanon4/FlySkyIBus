/*
 * Test FlySky IBus interface on an Arduino Mega.
 *  Connect FS-iA6B receiver to Serial1.
 */
 
#include "FlySkyIBus.h"

void setup() 
{
  Serial.begin(115200);
  IBus.begin(Serial1);
}

void loop() 
{
  IBus.loop();
  Serial.println(IBus.readChannel(0), HEX);
}
