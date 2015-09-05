#include <DcsBios.h>
#include <AccelStepper.h>

DcsBiosRs485Device dcsBiosDevice;
//DcsBiosSerialDevice dcsBiosDevice;

DirectOutputPin standbyFlagOff(9);
//Led standbyFlag(0x0000, 0x1000, 11, 9);

DirectOutputPin needleEnable(A0);
DirectOutputPin needleStep(12);
DirectOutputPin needleDir(13);
void forward() {
    needleDir.clear();
    needleStep.set();
    delayMicroseconds(10);
    needleStep.clear();
}
void backward() {
    needleDir.set();
    needleStep.set();
    delayMicroseconds(10);
    needleStep.clear();
}
AccelStepper stepper(forward, backward);

IntegerListener altNeedlePosition(0x107e, 0xffff, 0);
IntegerListener alt1000FtPosition(0x1082, 0xffff, 0);
IntegerListener alt10000FtPosition(0x1080, 0xffff, 0);
DimmableLed backlight(0x114a, 0xffff, 0, 3);
Switch3Pos altElectPneu("ALT_ELECT_PNEU", 6, 5);

void setup() {
    Serial.begin(250000);
    dcsBiosDevice.begin(&Serial, 2, 2);

    needleEnable.clear();
    stepper.setMaxSpeed(2400.0);
    stepper.setAcceleration(1000.0);

    standbyFlagOff.set();
}

void loop() {
    dcsBiosDevice.process();
    PollingInput::pollInputs();
    stepper.run();
}

void onDcsBiosFrameSync() {
    unsigned long alt = ((alt10000FtPosition.getData() / 6553) * 10000) + 
                        ((alt1000FtPosition.getData() / 6553) * 1000) + 
                        (altNeedlePosition.getData() / 65);
    stepper.moveTo(alt * 1.6);
}

void sendDcsBiosMessage(const char* msg, const char* arg) {
    dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
