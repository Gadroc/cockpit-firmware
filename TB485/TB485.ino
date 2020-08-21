#include <U8g2lib.h>
#include <Wire.h>

/**
 * Sketch to implement a RS-485 bus controller.  This sketch requires an arduino with two UART/Serial Ports.
 * It has been tested on Arduino MEGA and Arudino Leonardos.
 */
#include "DcsBios.h"

// UART Serial that is connected to your PC.
#define PC_SERIAL Serial

// UART Serial that is connected to RS-485 transeiver
#define BUS_SERIAL Serial1

// Pin connected to RS-485 transeiver transmit / receive pins.
#define BUS_TX_PIN 4

DcsBiosRs485Controller controller(BUS_SERIAL, BUS_TX_PIN, PC_SERIAL);
U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C u8g2(U8G2_R0);


void setup() {
  PC_SERIAL.begin(250000);
  BUS_SERIAL.begin(250000);

  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso20_tr);
    u8g2.drawStr(0,32,"TB485");
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.drawStr(80,32,"Rev1");
  } while ( u8g2.nextPage() );

  delay(1000);

  while (!PC_SERIAL || !BUS_SERIAL) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  u8g2.clearDisplay();
}

void loop() {
  controller.process();
}
