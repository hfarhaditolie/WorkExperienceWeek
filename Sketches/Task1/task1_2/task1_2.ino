/*
 * Team Names - animated intro on the OLED
 * Each name slides in from the right, pauses in the middle, then slides
 * off to the left. Colours change with each name.
 *
 * EDIT the names list below to your team!
 *
 * Wiring (OLED -> ESP32, SPI):
 *   GND->GND  +->3V3  CK->18  SI->23  OC->5  DC->16  R->17
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
#define MAGENTA 0xF81F
#define RED     0xF800

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);

// ---------------- EDIT THIS: your team ----------------
const char* teamName = "Team Voltage";        // shown at the top
const char* members[] = {                     // the names that animate
  "Alice",
  "Ben",
  "Chloe",
  "Dev"
};
const int NUM_MEMBERS = sizeof(members) / sizeof(members[0]);
// ------------------------------------------------------

// A few colours to cycle through, one per name.
uint16_t colours[] = { CYAN, YELLOW, GREEN, MAGENTA, RED, WHITE };
const int NUM_COLOURS = sizeof(colours) / sizeof(colours[0]);

const int CHAR_W = 6;     // each size-1 character is about 6 pixels wide
const int SCREEN_W = 96;  // screen width in pixels

// Draw the fixed title bar at the top (stays still).
void drawTitle() {
  display.fillRect(0, 0, 96, 12, BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  int titleW = strlen(teamName) * CHAR_W;
  display.setCursor((SCREEN_W - titleW) / 2, 2);   // centred
  display.print(teamName);
  display.drawFastHLine(0, 12, 96, 0x4208);        // a thin line under the title
}

// Draw one name at a given x position (used for the slide animation).
void drawNameAt(const char* name, int x, uint16_t colour) {
  display.fillRect(0, 13, 96, 51, BLACK);   // clear the name area only
  display.setTextSize(2);                   // bigger text for the names
  display.setTextColor(colour);
  display.setCursor(x, 30);
  display.print(name);
}

// Animate one name: slide in from the right, hold, slide out to the left.
void animateName(const char* name, uint16_t colour) {
  int nameW = strlen(name) * CHAR_W * 2;    // size-2 chars are ~12 px wide
  int centreX = (SCREEN_W - nameW) / 2;

  // 1) Slide IN: from off-screen right to the centre
  for (int x = SCREEN_W; x >= centreX; x -= 4) {
    drawNameAt(name, x, colour);
    delay(15);
  }
  // 2) HOLD in the middle so people can read it
  delay(900);
  // 3) Slide OUT: from centre off-screen to the left
  for (int x = centreX; x >= -nameW; x -= 6) {
    drawNameAt(name, x, colour);
    delay(12);
  }
}

void setup() {
  Serial.begin(115200);
  display.begin();
  display.fillScreen(BLACK);
  drawTitle();
  Serial.println("Team names animation running.");
}

void loop() {
  drawTitle();   // keep the title visible
  for (int i = 0; i < NUM_MEMBERS; i++) {
    uint16_t colour = colours[i % NUM_COLOURS];   // pick a colour for this name
    animateName(members[i], colour);
  }
}
