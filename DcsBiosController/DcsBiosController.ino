#include "DcsBios.h"

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(250000);
  Serial1.begin(250000);
  DcsBiosController.begin(&Serial1, 8, &Serial);
}

void loop() {
  DcsBiosController.process();
}
