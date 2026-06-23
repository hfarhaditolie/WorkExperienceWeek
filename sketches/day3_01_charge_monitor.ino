/*
 * DAY 3 - Sketch 1: Charge monitor (one pack)
 * Goal: watch the CC/CV charging phases on the screen while a TP4056 charges
 *       a partly-empty pack.
 *
 * Wiring (charge path):
 *   5V -> TP4056 IN+/IN-
 *   TP4056 BAT+ -> INA219 Vin+ -> INA219 Vin- -> Pack(+)
 *   TP4056 BAT- -> Pack(-) -> common GND
 *   INA219 VCC->3V3 GND->GND SDA->GPIO21 SCL->GPIO22
 *
 * What you should see:
 *   CC  : current roughly constant, voltage climbing
 *   CV  : voltage near 4.2 V, current falling (tapering)
 *   FULL: current very small
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

// --- EDIT THESE for your pack/charger ---
const float CHARGE_CURRENT_mA = 1000.0;   // the fixed current your TP4056 is set to
const float CV_VOLTAGE        = 4.15;     // "near full" voltage threshold
const float TERM_FRACTION     = 0.10;     // "full" when current < 10% of charge current
// ----------------------------------------

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);
Adafruit_INA219 ina219(0x40);

const char* chargePhase(float v, float mA) {
  float a = fabs(mA);
  if (a < CHARGE_CURRENT_mA * TERM_FRACTION && v > CV_VOLTAGE) return "FULL";
  if (v >= CV_VOLTAGE) return "CV (taper)";
  if (a > CHARGE_CURRENT_mA * 0.2)           return "CC (charge)";
  return "idle";
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  display.begin();
  display.fillScreen(BLACK);
  if (!ina219.begin()) {
    display.setTextColor(RED);
    display.setCursor(4, 28);
    display.print("No sensor!");
    while (1) delay(10);
  }
  Serial.println("time_s,V,mA,phase");
}

void loop() {
  float v  = ina219.getBusVoltage_V();
  float mA = ina219.getCurrent_mA();
  const char* phase = chargePhase(v, mA);

  Serial.print(millis() / 1000); Serial.print(",");
  Serial.print(v, 3);  Serial.print(",");
  Serial.print(mA, 1); Serial.print(",");
  Serial.println(phase);

  display.fillScreen(BLACK);
  display.drawRect(0, 0, 96, 64, 0x4208);
  display.setTextSize(1);
  display.setTextColor(CYAN);
  display.setCursor(4, 4);
  display.print("Charging");

  display.setTextColor(WHITE);
  display.setCursor(4, 18);
  display.print(v, 2);  display.print(" V");
  display.setCursor(4, 30);
  display.print(fabs(mA), 0); display.print(" mA");

  display.setTextColor(YELLOW);
  display.setCursor(4, 46);
  display.print(phase);

  delay(1000);
}
