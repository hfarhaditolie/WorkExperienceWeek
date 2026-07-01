/*
 * INA219 readings -> OLED + Serial Monitor
 * Shows voltage, current and power, live, on both the screen and Serial.
 * Good for watching charge current flow into a battery via the TP4056.
 *
 * Wiring:
 *   INA219 logic -> ESP32:  VCC->3V3  GND->GND  SDA->P21  SCL->P22
 *   INA219 in series:       TP4056 B+ -> Vin+ -> Vin- -> Battery(+)
 *                           TP4056 B- -> Battery(-) -> common GND
 *   OLED (SPI):  CK->18  SI->23  OC->5  DC->16  R->17  +->3V3  GND->GND
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
Adafruit_INA219 ina219(0x40);     // change if your sensor scanned at another address

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);             // SDA=21 (P21), SCL=22 (P22)
  display.begin();
  display.fillScreen(BLACK);

  // Try to find the sensor and SAY SO on screen + Serial.
  if (!ina219.begin()) {
    Serial.println("ERROR: INA219 not found! Check VCC, GND, SDA=21, SCL=22.");
    display.setTextColor(RED);
    display.setCursor(4, 24);
    display.print("No sensor!");
    display.setTextColor(WHITE);
    display.setCursor(4, 40);
    display.print("Check wiring");
    while (1) delay(10);          // stop here
  }

  // Found it - confirm on both outputs.
  Serial.println("INA219 detected at 0x40 :)");
  display.setTextColor(GREEN);
  display.setCursor(4, 28);
  display.print("INA219 OK");
  delay(1500);

  Serial.println("V,mA,mW");      // CSV header for the Serial Monitor / Plotter
}

void loop() {
  // --- Read the sensor ---
  float busV   = ina219.getBusVoltage_V();      // voltage at the load/battery side
  float shuntV = ina219.getShuntVoltage_mV();   // tiny drop across the shunt (mV)
  float mA     = ina219.getCurrent_mA();        // current (sign shows direction)
  float mW     = ina219.getPower_mW();          // power
  float voltage = busV + (shuntV / 1000.0);     // true battery voltage

  // --- Serial Monitor ---
  Serial.print(voltage, 3); Serial.print(",");
  Serial.print(mA, 1);      Serial.print(",");
  Serial.println(mW, 1);

  // --- OLED ---
  display.fillScreen(BLACK);
  display.drawRect(0, 0, 96, 64, 0x4208);

  display.setTextSize(1);
  display.setTextColor(CYAN);
  display.setCursor(4, 4);
  display.print("INA219");

  display.setTextColor(WHITE);
  display.setCursor(4, 18);
  display.print(voltage, 2); display.print(" V");

  display.setCursor(4, 32);
  display.print(mA, 0);      display.print(" mA");

  display.setCursor(4, 46);
  display.print(mW, 0);      display.print(" mW");

  delay(500);
}
