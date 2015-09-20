#include <DcsBios.h>

DcsBiosRs485Device dcsBiosDevice(Serial, 2, 0);

DirectOutputPin standbyFlagOff(9);
//Led standbyFlag(0x0000, 0x1000, 11, 9);

DirectStepperDriver needleStepper(12, 13, A0);
AcceleratedStepperOutput needleOutput(needleStepper, 200, 8, 1257, 8000);

IntegerListener altNeedlePosition(0x107e, 0xffff, 0);
IntegerListener alt1000FtPosition(0x1082, 0xffff, 0);
IntegerListener alt10000FtPosition(0x1080, 0xffff, 0);
DimmableLed backlight(0x114a, 0xffff, 0, 3);
Switch3Pos altElectPneu("ALT_ELECT_PNEU", 6, 5);

void setup() {
    Serial.begin(250000);
    needleStepper.enable();
    standbyFlagOff.set();
}

void loop() {
    dcsBiosDevice.process();
    PollingInput::pollInputs();
    StepperOutput::runSteppers();
}

void onDcsBiosFrameSync() {
    unsigned long alt = ((alt10000FtPosition.getData() / 6553) * 10000) + 
                        ((alt1000FtPosition.getData() / 6553) * 1000) + 
                        (altNeedlePosition.getData() / 65);
    needleOutput.setTargetPosition(alt * 1.6);
}

void sendDcsBiosMessage(const char* msg, const char* arg) {
    dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
