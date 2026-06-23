/*
 * DAY 2 - Sketch 3: Read ONE pack
 * Goal: measure your pack's voltage + current and show it on the OLED,
 *       plus an estimated State of Charge (SoC) %.
 *
 * Wiring:
 *   INA219 VCC->3V3  GND->GND  SDA->GPIO21  SCL->GPIO22
 *   To read VOLTAGE only: jumper Pack(+) to BOTH Vin+ and Vin-, Pack(-) to GND.
 *   To read CURRENT too:  put INA219 in line ->  Pack(+) -> Vin+ -> Vin- -> load.
 *   Common ground: Pack(-) and ESP32 GND must connect.
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#define CS_PIN  5
#define DC_PIN  16
#define RST_PIN 17
#define BLACK  0x0000
#define WHITE  0xFFFF
#define CYAN   0x07FF
#define GREEN  0x07E0
#define YELLOW 0xFFE0
#define RED    0xF800

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);
Adafruit_INA219 ina219(0x40);     // change if your sensor scanned at a different address

// Convert a 1S Li-ion cell voltage to an approximate SoC (0-100%).
// Uses a small lookup table with straight-line interpolation between points.
uint8_t socFromVoltage(float v) {
  const float vt[] = {3.00, 3.50, 3.60, 3.70, 3.80, 3.90, 4.00, 4.10, 4.20};
  const float st[] = {   0,    6,   12,   25,   45,   65,   80,   90,  100};
  const int N = 9;
  if (v <= vt[0])   return 0;
  if (v >= vt[N-1]) return 100;
  for (int i = 0; i < N-1; i++) {
    if (v < vt[i+1]) {
      float frac = (v - vt[i]) / (vt[i+1] - vt[i]);
      return (uint8_t)(st[i] + frac * (st[i+1] - st[i]));
    }
  }
  return 100;
}

uint16_t socColor(uint8_t soc) {
  if (soc >= 70) return GREEN;
  if (soc >= 40) return YELLOW;
  return RED;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  display.begin();
  display.fillScreen(BLACK);

  if (!ina219.begin()) {
    Serial.println("INA219 not found! Run the I2C scanner.");
    display.setTextColor(RED);
    display.setCursor(4, 28);
    display.print("No sensor!");
    while (1) delay(10);
  }
  Serial.println("V,mA,SoC%");   // CSV header for the Serial Plotter/Monitor
}

void loop() {
  float v   = ina219.getBusVoltage_V();   // pack voltage
  float mA  = ina219.getCurrent_mA();     // current (sign shows direction)
  uint8_t soc = socFromVoltage(v);

  // ---- Serial ----
  Serial.print(v, 3);   Serial.print(",");
  Serial.print(mA, 1);  Serial.print(",");
  Serial.println(soc);

  // ---- Display ----
  display.fillScreen(BLACK);
  display.drawRect(0, 0, 96, 64, 0x4208);

  display.setTextSize(1);
  display.setTextColor(CYAN);
  display.setCursor(4, 4);
  display.print("Pack 1");

  display.setTextColor(WHITE);
  display.setCursor(4, 18);
  display.print(v, 2);  display.print(" V");

  display.setCursor(4, 30);
  display.print(mA, 0); display.print(" mA");

  display.setTextColor(socColor(soc));
  display.setCursor(4, 44);
  display.print("SoC ");
  display.print(soc);
  display.print("%");

  // SoC bar along the bottom
  int barW = 88;
  int fillW = barW * soc / 100;
  display.drawRect(4, 56, barW, 5, 0x4208);
  if (fillW > 0) display.fillRect(5, 57, fillW, 3, socColor(soc));

  delay(500);
}
