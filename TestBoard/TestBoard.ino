#include "DcsBios.h"

Led EGI(29, 1 << 6, 13);
Switch2Pos MasterCautionButton("UFC_MASTER_CAUTION", 4);
Potentiometer lcpFlightInst("LCP_FLIGHT_INST", A0);
PwmLed instrumentBacklight(0, 3);

void setup() {
  Serial.begin(250000);
  DcsBiosDevice.begin(&Serial, 8, 0, 0);
}

void loop() {
  DcsBiosDevice.process();
}
