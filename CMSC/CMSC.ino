#include <DcsBios.h>
#include <DcsBiosHcms.h>

#include "CMSCFont.h"

#define TX_ENABLE_PIN        2
#define RS485_ADDRESS        2

#define DATA_PIN             20
#define REGISTER_SELECT_PIN  4
#define CLOCK_PIN            21
#define CHIP_ENABLE_PIN      23
#define RESET_PIN            22

#define JMR_BUTTON_PIN       10
#define MWS_BUTTON_PIN       12

#define ML_INDICATOR_PIN     13
#define PRI_INDICATOR_PIN    8
#define UKN_INDICATOR_PIN    7

#define PRI_BUTTON_PIN       6
#define SEP_BUTTON_PIN       9

#define BACKLIGHT_PIN        3

#define BRIGHTNESS_PIN       A0
#define VOLUME_PIN           A1

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosRs485Device dcsBiosDevice(Serial, TX_ENABLE_PIN, RS485_ADDRESS);

/* Instantiate HCMS-2913 display */
HcmsDisplay hcms(DATA_PIN, REGISTER_SELECT_PIN, CLOCK_PIN, CHIP_ENABLE_PIN, RESET_PIN, 3, 24, CmscFont);
HcmsBrightness hcmsBrightnessControl(0x10ea, hcms);
HcmsTextDisplay jmrDisplay(0x1096, hcms, 0, 8);
HcmsTextDisplay chaffFlareDisplay(0x108e, hcms, 8, 8);
HcmsTextDisplay mwsDisplay(0x12b0, hcms, 16, 8);

/* Create Inputs */
ActionButton jmrButton("CMSC_JMR", "INC", JMR_BUTTON_PIN);
ActionButton mwsButton("CMSC_MWS", "INC", MWS_BUTTON_PIN);
ActionButton priButton("CMSC_PRI", "INC", PRI_BUTTON_PIN);
ActionButton sepButton("CMSC_SEP", "INC", SEP_BUTTON_PIN);

/* Create Indicators */
Led mlIndicator(0x1012, 0x100, 8, ML_INDICATOR_PIN);
Led priIndicator(0x1012, 0x200, 9, PRI_INDICATOR_PIN);
Led uknIndicator(0x1012, 0x0400, 10, UKN_INDICATOR_PIN);

/* Setup Backlighting */
DimmableLed cmscPanelBacklight(0x114c, 0xffff, 0, BACKLIGHT_PIN);

/* Setup Brightness Knob */
Potentiometer brightnessKnob("CMSC_BRT", BRIGHTNESS_PIN);

/* Setup Volume Knob */
Potentiometer volumeKnob("CMSC_RWR_VOL", VOLUME_PIN);

/*
 * Your setup routine needs to initialize your communications
 * and all of your input devices.
 */
void setup() {
  // Initialize the serial port to 250000 baud.
  Serial.begin(250000);

  // Initialize HCMS display
  hcms.begin();

  // Initialize all of your polling inputs.
  PollingInput::begin();  

  pinMode(16, OUTPUT);
}

/**
 * Your main loop needs to process bus data and poll your inputs.
 */
void loop() {
  // Process any incoming data.
  dcsBiosDevice.process();
  
  // Check our inputs and record messages for DCS-BIOS
  PollingInput::pollInputs();
}


/*
 * This method will get called at the end of every data frame.  You
 * can update any outputs here if necessary.  Built in output devices
 * automatcially update, and don't need code here.
 */
void onDcsBiosFrameSync() {
}

/*
 * You need to define 
 * void sendDcsBiosMessage(const char* msg, const char* arg)
 * so that the string msg, followed by a space, the string arg
 * and a newline gets sent to the DCS-BIOS import stream.
 * Typically we will use the built in function of the serial 
 * DcsBiosSerialDevice or DcsBiosRs485Device to send data back.
 * But you can use this method to manually use other communication
 * techniques.
 */
void sendDcsBiosMessage(const char* msg, const char* arg) {
  dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
