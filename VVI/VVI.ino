#include <DcsBios.h>

DcsBiosRs485Device dcsBiosDevice(Serial, 2, 1);

DirectStepperDriver vviNeedleDriver(10, 11, A3);
AcceleratedStepperOutput vviNeedleOutput(vviNeedleDriver, 400, 8, 65535, 1257);
Stepper vviNeedle(0x106e, 0xffff, 0, -1500, 1500, vviNeedleOutput);

DimmableLed backlight(0x114a, 0xffff, 0, 3);

void setup() {
    Serial.begin(250000);
    PollingInput::initInputs();
}

void loop() {
    dcsBiosDevice.process();
    PollingInput::pollInputs();
    StepperOutput::runSteppers();
}

void onDcsBiosFrameSync() {
}

void sendDcsBiosMessage(const char msg[], const char arg[]) {
    dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
