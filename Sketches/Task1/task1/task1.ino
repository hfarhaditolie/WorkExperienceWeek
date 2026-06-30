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

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting OLED test...");

  display.begin();
  display.fillScreen(BLACK);

  display.drawRect(0, 0, 96, 64, YELLOW);   // a yellow border around the screen

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8, 12);
  display.print("Hello,");

  display.setTextColor(CYAN);
  display.setCursor(8, 28);
  display.print("ESP32!");

  Serial.println("Message shown on OLED.");
}

void loop() {
  // Static screen for this task.
}