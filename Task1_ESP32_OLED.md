# Task 1 – Introduction to the ESP32 and OLED Display

## Objective
Become familiar with the ESP32 development environment and the OLED display.

## Activities
- Connect the ESP32 to the OLED display.
- Upload a simple Arduino program.
- Display a welcome message on the OLED screen.
- Verify that the display operates correctly.

This task introduces students to basic hardware connections and microcontroller programming before building the complete system.

---

## Step 1 — Wire the OLED to the ESP32
The OLED uses **SPI** (a fast 4-wire way for chips to send data). Connect the pins exactly as below, **with the USB cable unplugged**:

| OLED pin | ESP32 pin | Meaning |
|----------|-----------|---------|
| GND | GND | Ground (0 V reference) — **connect first** |
| + (VCC) | 3V3 | Power for the display |
| CK | GPIO18 | SPI clock |
| SI | GPIO23 | SPI data (ESP32 → display) |
| OC (CS) | GPIO5 | Chip select |
| DC | GPIO16 | Data/command line |
| R (RST) | GPIO17 | Reset |

```
  ESP32              OLED
  GND   ───────────  GND   (connect first)
  3V3   ───────────  +
  GPIO18 ──────────  CK
  GPIO23 ──────────  SI
  GPIO5  ──────────  OC
  GPIO16 ──────────  DC
  GPIO17 ──────────  R
```

## Step 2 — Set up the software (once)
1. Open the **Arduino IDE**.
2. Install ESP32 board support and select your board (**Tools → Board → ESP32 Arduino → NodeMCU-32S**).
3. Install the libraries (**Tools → Manage Libraries**): **Adafruit GFX**, **Adafruit SSD1331**, and **Adafruit BusIO**.

## Step 3 — The program, explained piece by piece
Below is the full sketch, split into sections. Read the explanation under each block.

### A) The headers (`#include`)
```cpp
#include <Adafruit_GFX.h>       // general drawing functions (text, shapes)
#include <Adafruit_SSD1331.h>   // driver for THIS specific OLED chip (SSD1331)
#include <SPI.h>                // lets the ESP32 talk over the SPI bus
```
**What this does:** `#include` lines pull in **libraries** — ready-made code other people wrote so you don't have to. Think of them as importing tools:
- `SPI.h` = the ability to *send* data fast to the screen.
- `Adafruit_SSD1331.h` = knows how *this exact* OLED chip works.
- `Adafruit_GFX.h` = the drawing toolbox (write text, draw lines/boxes).

### B) Naming the pins (`#define`)
```cpp
#define CS_PIN   5    // OLED "OC" (chip select) is on GPIO5
#define DC_PIN   16   // OLED "DC" is on GPIO16
#define RST_PIN  17   // OLED "R" (reset) is on GPIO17
```
**What this does:** `#define` gives a number a **friendly name**. Instead of remembering "5", we write `CS_PIN`. If we ever rewire, we change it in one place. These are not variables — the compiler just swaps the name for the number before building.

### C) Naming the colours
```cpp
#define BLACK   0x0000   // colours are 16-bit numbers (RGB565 format)
#define WHITE   0xFFFF
#define CYAN    0x07FF
#define YELLOW  0xFFE0
```
**What this does:** The screen uses **numbers to represent colours**. `0x....` means a hexadecimal (base-16) number. We give each a name so the code reads `WHITE` instead of `0xFFFF`.

### D) Creating the display object
```cpp
Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);
```
**What this does:** This makes a **`display` object** — a thing we can give commands to (like `display.print(...)`). We tell it which SPI bus and which pins to use. From now on, anything we want on screen goes through `display`.

### E) `setup()` — runs ONCE when the board powers on
```cpp
void setup() {
  Serial.begin(115200);        // start the Serial link to the computer at 115200 speed
  Serial.println("Starting OLED test...");   // print a message to the Serial Monitor

  display.begin();             // wake up / initialise the OLED
  display.fillScreen(BLACK);   // clear the whole screen to black

  display.setTextSize(1);      // text size (1 = smallest)
  display.setTextColor(WHITE); // colour of the next text
  display.setCursor(8, 12);    // move the "pen" to x=8, y=12 (pixels from top-left)
  display.print("Hello,");     // write this text at the cursor

  display.setTextColor(CYAN);  // change colour for the next line
  display.setCursor(8, 28);    // move down the screen
  display.print("ESP32!");     // write the second line

  Serial.println("Message shown on OLED.");  // confirm in the Serial Monitor
}
```
**What this does, line by line:**
- `void setup() { ... }` — code inside the `{ }` runs **once** at start-up. Perfect for one-time setup like turning the screen on.
- `Serial.begin(115200)` — opens a text channel back to your computer at a **baud rate** (speed) of 115200. You read it in **Tools → Serial Monitor**.
- `Serial.println("...")` — sends a line of text to the computer (handy for messages/debugging). `println` adds a new line; `print` does not.
- `display.begin()` — initialises the OLED so it's ready to draw.
- `display.fillScreen(BLACK)` — paints the whole screen one colour (here, black = blank).
- `display.setTextSize(1)` — sets how big letters are.
- `display.setTextColor(WHITE)` — sets the colour of text drawn **after** this line.
- `display.setCursor(x, y)` — moves the drawing position. `(0,0)` is the **top-left**; x goes right, y goes down. The screen is **96 wide × 64 tall** pixels.
- `display.print("Hello,")` — draws the text starting at the cursor.

### F) `loop()` — runs OVER AND OVER, forever
```cpp
void loop() {
  // Nothing here for this task - the screen stays as it is.
}
```
**What this does:** After `setup()` finishes, the ESP32 runs `loop()` again and again as fast as it can. For this task we leave it empty because the welcome message doesn't need to change. Later (reading sensors), the repeating code goes here.

---

## Step 4 — The full sketch (copy this)
```cpp
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
```

## Step 5 — Upload and verify
1. Plug in the USB-C cable.
2. Choose **Tools → Port** (your ESP32).
3. Click **Upload** (the → arrow).
4. **On the OLED:** you should see a yellow border with **"Hello,"** (white) and **"ESP32!"** (cyan).
5. **On the computer:** open **Tools → Serial Monitor**, set the speed to **115200** (bottom-right). You should see:
   ```
   Starting OLED test...
   Message shown on OLED.
   ```

✅ **Task complete** when both the OLED shows the message **and** the Serial Monitor shows the two lines.

## Troubleshooting
| Problem | Likely cause / fix |
|---------|--------------------|
| Screen stays black | Check GND and 3V3, and the 5 SPI pins (CK/SI/OC/DC/R) against the table. |
| Upload fails | Wrong board or port selected; try holding the **BOOT** button during upload. |
| Serial Monitor shows nothing or gibberish | Set the speed to **115200**. |
| Text in wrong place | Remember `(0,0)` is top-left; the screen is only 96×64 pixels. |

## What you learned
- **Headers (`#include`)** bring in libraries (ready-made code).
- **`#define`** gives names to pins and colours.
- **`setup()`** runs once; **`loop()`** runs forever.
- **`Serial`** sends messages back to your computer for checking.
- **`display.` commands** (`begin`, `fillScreen`, `setCursor`, `print`) draw on the OLED.

**Next task:** connect the INA219 sensor and read your first voltage and current.
