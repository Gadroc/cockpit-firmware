#include "DcsBios.h"

Led LangingGearHandle(28, 1<<3, 13);

void setup() {
  Serial.begin(250000);
  DcsBiosDevice.begin(&Serial, 8, 0, 2);
}

void loop() {
  DcsBiosDevice.process();
}
