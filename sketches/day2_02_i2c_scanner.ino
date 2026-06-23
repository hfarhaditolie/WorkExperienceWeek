/*
 * DAY 2 - Sketch 2: I2C Scanner
 * Goal: find your INA219 sensor on the I2C bus.
 * Open  Tools -> Serial Monitor  (set speed to 115200).
 * A correctly wired INA219 shows up at address 0x40.
 *
 * Wiring (INA219 -> ESP32):
 *   VCC -> 3V3    GND -> GND
 *   SDA -> GPIO21 SCL -> GPIO22
 *   (Connect GND / common ground FIRST.)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#define CS_PIN   5
#define DC_PIN   16
#define RST_PIN  17
#define BLACK   0x0000
#define WHITE   0xFFFF
#define GREEN   0x07E0
#define RED     0xF800

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);          // SDA=21, SCL=22
  display.begin();
  display.fillScreen(BLACK);
}

void loop() {
  Serial.println("Scanning I2C bus...");
  display.fillScreen(BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2, 2);
  display.print("I2C devices:");

  int count = 0;
  int y = 16;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      count++;
      Serial.print("  Found device at 0x");
      Serial.println(addr, HEX);
      display.setTextColor(GREEN);
      display.setCursor(8, y);
      display.print("0x");
      display.print(addr, HEX);
      y += 12;
    }
  }

  if (count == 0) {
    Serial.println("  None found! Check wiring + common ground.");
    display.setTextColor(RED);
    display.setCursor(8, 16);
    display.print("None - check");
    display.setCursor(8, 28);
    display.print("wiring!");
  }
  Serial.print("Done. Total: ");
  Serial.println(count);

  delay(2000);
}
