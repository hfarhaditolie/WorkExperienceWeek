/*
 * DAY 4 - Sketch 1: Race logger
 * Goal: while the cars run off the packs, log each pack's voltage + current,
 *       count the energy delivered (coulomb counting = mAh), and show it live.
 *       A pack is "DEAD" when it falls to the safe cutoff voltage.
 *
 * The grid shows SoC % per pack. The Serial Monitor logs a CSV you can paste
 * into a spreadsheet to plot discharge curves and compare packs.
 *
 * Wiring: same as Day 3 - all 8 INA219 on the I2C bus. The car is the load:
 *   Pack(+) -> INA219 Vin+ -> Vin- -> Car motor/ESC -> Pack(-) (common GND).
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
#define GREY   0x4208

#define NUM_PACKS 8
const float DEAD_VOLTAGE = 3.00;     // safe discharge cutoff - do not go lower!

Adafruit_INA219 ina[NUM_PACKS] = {
  Adafruit_INA219(0x40), Adafruit_INA219(0x41),
  Adafruit_INA219(0x44), Adafruit_INA219(0x45),
  Adafruit_INA219(0x48), Adafruit_INA219(0x49),
  Adafruit_INA219(0x4C), Adafruit_INA219(0x4D)
};
bool  present[NUM_PACKS];
bool  dead[NUM_PACKS];
float deliveredmAh[NUM_PACKS];       // coulomb count: how much charge each pack gave
unsigned long lastTime;

#define GRID_COLS 2
#define GRID_ROWS 4
#define HEADER_H  11
#define CELL_W   (96 / GRID_COLS)
#define CELL_H   ((64 - HEADER_H) / GRID_ROWS)

uint8_t socFromVoltage(float v) {
  const float vt[] = {3.00, 3.50, 3.60, 3.70, 3.80, 3.90, 4.00, 4.10, 4.20};
  const float st[] = {   0,    6,   12,   25,   45,   65,   80,   90,  100};
  const int N = 9;
  if (v <= vt[0])   return 0;
  if (v >= vt[N-1]) return 100;
  for (int i = 0; i < N-1; i++)
    if (v < vt[i+1]) {
      float frac = (v - vt[i]) / (vt[i+1] - vt[i]);
      return (uint8_t)(st[i] + frac * (st[i+1] - st[i]));
    }
  return 100;
}
uint16_t socColor(uint8_t soc) {
  if (soc >= 70) return GREEN;
  if (soc >= 40) return YELLOW;
  return RED;
}

void drawCell(uint8_t i, int soc, bool isDead) {
  uint8_t col = i % GRID_COLS, row = i / GRID_COLS;
  int x = col * CELL_W, y = HEADER_H + row * CELL_H;
  display.drawRect(x, y, CELL_W, CELL_H, GREY);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x + 2, y + 2);
  display.print('P'); display.print(i + 1);

  if (soc < 0) { display.setTextColor(GREY); display.setCursor(x + 18, y + 2); display.print("--"); return; }

  if (isDead) {
    display.setTextColor(RED);
    display.setCursor(x + 16, y + 2);
    display.print("DEAD");
  } else {
    uint16_t c = socColor(soc);
    display.setTextColor(c);
    display.setCursor(x + 18, y + 2);
    display.print(soc); display.print('%');
    int barX = x + 2, barY = y + CELL_H - 5, barW = CELL_W - 4;
    int fillW = (barW - 2) * soc / 100;
    display.drawRect(barX, barY, barW, 3, GREY);
    if (fillW > 0) display.fillRect(barX + 1, barY + 1, fillW, 1, c);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  display.begin();
  display.fillScreen(BLACK);

  for (uint8_t i = 0; i < NUM_PACKS; i++) {
    present[i] = ina[i].begin();
    dead[i] = false;
    deliveredmAh[i] = 0;
  }
  lastTime = millis();

  // CSV header: paste the Serial output into a spreadsheet to plot it
  Serial.print("time_s");
  for (uint8_t i = 0; i < NUM_PACKS; i++) {
    Serial.print(",P"); Serial.print(i + 1); Serial.print("_V");
    Serial.print(",P"); Serial.print(i + 1); Serial.print("_mA");
    Serial.print(",P"); Serial.print(i + 1); Serial.print("_mAh");
  }
  Serial.println();
}

void loop() {
  unsigned long now = millis();
  float dt_h = (now - lastTime) / 3600000.0;   // elapsed time in hours
  lastTime = now;

  display.fillScreen(BLACK);
  display.setTextSize(1);
  display.setTextColor(CYAN);
  display.setCursor(2, 2);
  display.print("RACE!");

  Serial.print(now / 1000);

  for (uint8_t i = 0; i < NUM_PACKS; i++) {
    if (!present[i]) { Serial.print(",,,"); drawCell(i, -1, false); continue; }

    float v  = ina[i].getBusVoltage_V();
    float mA = fabs(ina[i].getCurrent_mA());

    if (v <= DEAD_VOLTAGE) dead[i] = true;
    if (!dead[i]) deliveredmAh[i] += mA * dt_h;   // coulomb counting

    Serial.print(","); Serial.print(v, 3);
    Serial.print(","); Serial.print(mA, 1);
    Serial.print(","); Serial.print(deliveredmAh[i], 1);

    drawCell(i, socFromVoltage(v), dead[i]);
  }
  Serial.println();

  delay(1000);
}
