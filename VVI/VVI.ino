#include <DcsBios.h>
#include <DcsBiosStepper.h>

DcsBiosRs485Device dcsBiosDevice;

Stepper vviNeedle(0x106e, -1500, 1500, 11, 10, A3);
DimmableLed backlight(0x114a, 0xffff, 0, 3);

void setup() {
    Serial.begin(250000);
    dcsBiosDevice.begin(&Serial, 2, 2);

    vviNeedle.setMaxSpeed(1600);
    vviNeedle.begin(true);

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
