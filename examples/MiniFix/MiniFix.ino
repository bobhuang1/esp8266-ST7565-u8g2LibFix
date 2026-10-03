// Drives a "Mini 12864" ST7565 panel with the stock u8g2 library: no patched
// library file needed. The two Mini-panel corrections (column offset 3 and
// display start line 0x60) are applied right after begin().
//
// Pins below match the ESP-12 sketches in this account; change them for your
// wiring.

#include <Arduino.h>
#include <U8g2lib.h>

U8G2_ST7565_64128N_F_4W_SW_SPI display(U8G2_R0, /* clock=*/ 14, /* data=*/ 12, /* cs=*/ 13, /* dc=*/ 15, /* reset=*/ 16);

// Set to true to invert the panel (what the patched file's INVERSE_DISPLAY did).
const bool INVERT_DISPLAY = false;

void applyMini12864Fix(U8G2& u8g2) {
  u8g2.getU8x8()->x_offset = 3;          // stock 64128N driver uses 4
  u8g2.sendF("c", 0x60);                 // display start line; stock init sends 0x40
  if (INVERT_DISPLAY) {
    u8g2.sendF("c", 0xa7);               // display reverse
  }
}

void setup() {
  display.begin();
  applyMini12864Fix(display);

  display.clearBuffer();
  display.setFont(u8g2_font_helvR08_tf);
  display.drawFrame(0, 0, 128, 64);      // all four edges should be visible
  display.drawStr(10, 30, "Mini 12864 OK");
  display.sendBuffer();
}

void loop() {
}
