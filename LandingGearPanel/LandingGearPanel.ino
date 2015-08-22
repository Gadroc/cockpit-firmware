#include <Wire.h>
#include <Servo.h>
#include "DcsBios.h"
#include "DcsBiosMcp.h"
#include "DcsBiosServo.h"

// Setup for fast I2C
#define CPU_FREQ 16000000L
#define TWI_FREQ 400000L

Mcp23017 inputPins(B0100001);
Mcp23018 outputPins(B0100000);

Led leftSafe(28, 1<<0, outputPins.getPin(1));
Led noseSafe(28, 1<<1, outputPins.getPin(2));
Led rightSafe(28, 1<<2, outputPins.getPin(3));
Led gearHandleLight(28, 1<<3, outputPins.getPin(0));
PwmLed backlight(0, 3);

Switch2Pos gearLever("GEAR_LEVER", inputPins.getPin(0));
Switch3Pos landingLights("LANDING_LIGHTS", inputPins.getPin(1), inputPins.getPin(2));

char* ANTISKID_MESSAGE = "ANTI_SKID_SWITCH";
ActionButton antiSkidSwitchRelease(ANTISKID_MESSAGE, "RELEASE", inputPins.getPin(3));
ActionButton antiSkidSwitchPush(ANTISKID_MESSAGE, "PUSH", inputPins.getPin(3));
ActionButton antiSkidSwitchOff(ANTISKID_MESSAGE, "OFF", inputPins.getPin(4));

ServoOutput flapsPosition(10, 708, 1250);

void setup() {
  Wire.begin();
  TWBR = ((CPU_FREQ / TWI_FREQ) - 16) / 2;

  outputPins.begin();
  inputPins.begin();

  flapsPosition.begin(A0);
  
  Serial.begin(250000);
  DcsBiosDevice.begin(&Serial, 8, 0, 2);
}

void loop() {
  DcsBiosDevice.process();
}
