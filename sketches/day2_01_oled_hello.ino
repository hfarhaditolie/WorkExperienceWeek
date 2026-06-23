/*
 * DAY 2 - Sketch 1: OLED Hello
 * Goal: prove the SSD1331 OLED display works.
 * If you see the splash text, your display wiring (SPI) is correct.
 *
 * Wiring (OLED -> ESP32):
 *   GND -> GND      +  -> 3V3
 *   CK  -> GPIO18   SI -> GPIO23
 *   OC  -> GPIO5    DC -> GPIO16    R -> GPIO17
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#define CS_PIN   5
#define DC_PIN   16
#define RST_PIN  17

#define BLACK   0x0000
#define WHITE   0xFFFF
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define GREEN   0x07E0

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  display.begin();
  display.fillScreen(BLACK);

  display.drawRect(0, 0, 96, 64, YELLOW);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8, 12);
  display.print("Battery");
  display.setTextColor(CYAN);
  display.setCursor(8, 26);
  display.print("Monitor");

  display.setTextColor(GREEN);
  display.setCursor(8, 46);
  display.print("Day 2 :)");

  Serial.println("OLED hello running. If you see text, SPI is good!");
}

void loop() {
  // Nothing to do - the screen stays as it is.
}
