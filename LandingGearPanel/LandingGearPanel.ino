#include <Wire.h>
#include <Servo.h>
#include "DcsBios.h"
#include "DcsBiosMcp.h"
#include "DcsBiosServo.h"

DcsBiosRs485Device dcsBiosDevice;

// Setup for fast I2C
#define CPU_FREQ 16000000L
#define TWI_FREQ 400000L

Mcp23017 inputPins(B0100001);
Mcp23018 outputPins(B0100000);

Led leftSafe(0x1026, 0x1000, 12, outputPins.getPin(1));
Led noseSafe(0x1026, 0x0800, 11, outputPins.getPin(2));
Led rightSafe(0x1026, 0x2000, 13, outputPins.getPin(3));
Led gearHandleLight(0x1026, 0x4000, 14, outputPins.getPin(0));
DimmableLed backlight(0x114c, 0xffff, 0, 3);

Switch2Pos gearLever("GEAR_LEVER", inputPins.getPin(0));
Switch3Pos landingLights("LANDING_LIGHTS", inputPins.getPin(1), inputPins.getPin(2));

ActionButton antiSkidSwitchPush("ANTI_SKID_SWITCH", "PUSH", inputPins.getPin(3));
ActionButton antiSkidSwitchOff("ANTI_SKID_SWITCH", "OFF", inputPins.getPin(4));

ServoOutput flapsPosition(0x10a0, 0xffff, 0, 670, 1250);

void setup() {
  Serial.begin(250000);
  dcsBiosDevice.begin(&Serial, 8, 1);

  Wire.begin();
  TWBR = ((CPU_FREQ / TWI_FREQ) - 16) / 2;

  outputPins.begin();
  inputPins.begin();

  flapsPosition.attach(A0);

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

