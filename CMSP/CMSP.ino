#include <DcsBios.h>
#include <Wire.h>
#include <DcsBiosMcp.h>
#include <LiquidCrystal.h>
#include "CmspDisplayLine.h"

DcsBiosRs485Device dcsBiosDevice;

//DirectOutputPin testPin(13);

// Setup for fast I2C
#define CPU_FREQ 16000000L
#define TWI_FREQ 400000L

LiquidCrystal cmspDisplay(12, 11, 5, 4, 3, 2);
DirectAnalogOutput backlightOutput(10);
DimmableLed cmspDisplayBacklight(0x10e6, 0xffff, 0, &backlightOutput);
Potentiometer cmspDisplayBrightness("CMSP_BRT", A0, 10, 60);

CmspDisplayLine cmspDisplayLine1(0x1000, &cmspDisplay, 0);
CmspDisplayLine cmspDisplayLine2(0x1014, &cmspDisplay, 1);

DimmableLed panelBacklight(0x1150, 0xffff, 0, 9);

Mcp23017 inputPins(B0100000);

Switch2Pos cmspArw1("CMSP_ARW1", inputPins.getPin(0));
Switch2Pos cmspArw2("CMSP_ARW2", inputPins.getPin(1));
Switch2Pos cmspArw3("CMSP_ARW3", inputPins.getPin(2));
Switch2Pos cmspArw4("CMSP_ARW4", inputPins.getPin(3));

Switch3Pos cmspUpdn("CMSP_UPDN", inputPins.getPin(5), inputPins.getPin(6));
Switch2Pos cmspRtn("CMSP_RTN", inputPins.getPin(4));

Switch3Pos cmspMws("CMSP_MWS", inputPins.getPin(11), inputPins.getPin(7));
Switch3Pos cmspJmr("CMSP_JMR", inputPins.getPin(12), inputPins.getPin(8));
Switch3Pos cmspRwr("CMSP_RWR", inputPins.getPin(13), inputPins.getPin(9));
Switch3Pos cmspDisp("CMSP_DISP", inputPins.getPin(14), inputPins.getPin(10));

Switch2Pos cmspJtsn("CMSP_JTSN", inputPins.getPin(15));

const uint8_t cmspModePins[5] = { A1, A2, A3, 7, 13 };
SwitchMultiPos cmspMode("CMSP_MODE", cmspModePins, 5);

void setup() {
  backlightOutput.write(255);
  cmspDisplay.begin(16,2);
  cmspDisplay.clear();
  
  Serial.begin(250000);
  dcsBiosDevice.begin(&Serial, 8, 0);

  Wire.begin();
  TWBR = ((CPU_FREQ / TWI_FREQ) - 16) / 2;

  inputPins.begin();

  PollingInput::initInputs();
}

void loop() {
  dcsBiosDevice.process();
  PollingInput::pollInputs();
}

void onDcsBiosFrameSync() {
}

void sendDcsBiosMessage(const char* msg, const char* arg) {
  dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
