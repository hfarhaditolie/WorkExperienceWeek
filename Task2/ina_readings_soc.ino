/*
 * INA219 readings + SoC (coulomb counting) for an 8-cell NiMH SERIES pack
 * Shows pack voltage, current, and State of Charge (%) on OLED + Serial.
 *
 * IMPORTANT - series pack facts:
 *   8 x NiMH in series = ~9.6 V nominal (~11.6 V full), still under the
 *   INA219's 26 V limit. Series ADDS VOLTAGE, NOT CAPACITY, so the pack
 *   capacity is still 1100 mAh (the same current flows through every cell).
 *   The SoC of the whole string = the SoC of each cell.
 *
 * How SoC is found (coulomb counting - the right method for flat-curve NiMH):
 *   charge moved (mAh) = current (mA) x time (hours)
 *   SoC change (%)     = charge moved / capacity x 100
 *   NiMH voltage is too flat to read SoC from, so we START from a KNOWN
 *   value (set START_SOC - e.g. 100 right after a full charge) and count.
 *
 * Wiring:
 *   INA219 logic -> ESP32: VCC->3V3 GND->GND SDA->P21 SCL->P22
 *   INA219 in series with the pack: Pack+ -> Vin+ -> Vin- -> Load -> Pack-
 *   OLED (SPI): CK->18 SI->23 OC->5 DC->16 R->17  +->3V3 GND->GND
 *   Common ground: Pack-, load-, ESP32 GND all joined.
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

// ---------------- EDIT THESE for your pack ----------------
const float CAPACITY_mAh = 2000.0;   // ONE cell's capacity = the series pack's capacity
const float START_SOC    = 100.0;    // SoC at power-on (set 100 right after a full charge)
// We want DISCHARGING to make SoC go DOWN. If SoC rises while the car runs
// (or falls while charging), change this from +1.0 to -1.0.
const float CURRENT_SIGN = -1.0;
// ----------------------------------------------------------

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);
Adafruit_INA219 ina219(0x40);

float soc = START_SOC;          // State of Charge in %
float chargeMoved_mAh = 0.0;    // running coulomb count
unsigned long lastMicros = 0;   // for the time step

uint16_t socColor(float s) {
  if (s >= 70) return GREEN;
  if (s >= 40) return YELLOW;
  return RED;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  display.begin();
  display.fillScreen(BLACK);

  if (!ina219.begin()) {
    Serial.println("INA219 not found!");
    display.setTextColor(RED);
    display.setCursor(4, 28);
    display.print("No sensor!");
    while (1) delay(10);
  }

  Serial.println("INA219 detected :)");
  display.setTextColor(GREEN);
  display.setCursor(4, 28);
  display.print("INA219 OK");
  delay(1500);

  soc = START_SOC;                 // begin counting from a known SoC
  lastMicros = micros();

  Serial.print("Start SoC "); Serial.print(soc, 1); Serial.println(" %");
  Serial.println("V,mA,SoC%");
}

void loop() {
  float busV   = ina219.getBusVoltage_V();
  float shuntV = ina219.getShuntVoltage_mV();
  float mA     = ina219.getCurrent_mA();
  float voltage = busV + (shuntV / 1000.0);        // whole-pack voltage

  // ---- Coulomb counting ----
  unsigned long now = micros();
  float dt_h = (now - lastMicros) / 3600000000.0;  // microseconds -> hours
  lastMicros = now;
  float dCharge = CURRENT_SIGN * mA * dt_h;         // mAh moved this step
  chargeMoved_mAh += dCharge;
  soc += (dCharge / CAPACITY_mAh) * 100.0;
  if (soc < 0)   soc = 0;
  if (soc > 100) soc = 100;

  // ---- Serial ----
  Serial.print(voltage, 3); Serial.print(",");
  Serial.print(mA, 1);      Serial.print(",");
  Serial.println(soc, 1);

  // ---- OLED ----
  display.fillScreen(BLACK);
  display.drawRect(0, 0, 96, 64, 0x4208);

  display.setTextSize(1);
  display.setTextColor(CYAN);
  display.setCursor(4, 4);
  display.print("NiMH pack");

  display.setTextColor(WHITE);
  display.setCursor(4, 18);
  display.print(voltage, 2); display.print(" V");

  display.setCursor(4, 30);
  display.print(mA, 0);      display.print(" mA");

  display.setTextColor(socColor(soc));
  display.setCursor(4, 44);
  display.print("SoC ");
  display.print(soc, 0);
  display.print("%");

  // SoC bar along the bottom
  int barW = 88;
  int fillW = (int)(barW * soc / 100.0);
  display.drawRect(4, 56, barW, 5, 0x4208);
  if (fillW > 0) display.fillRect(5, 57, fillW, 3, socColor(soc));

  delay(500);
}
