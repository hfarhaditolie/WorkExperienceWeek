# Day 2 — Make One Channel Work

**Today's goal:** Get the screen working, then read ONE pack's voltage, current and SoC.

> Golden rule of wiring: **always connect GND (ground) first**, then power, then signal wires. And **only wire with the USB unplugged** — plug in to test.

---

## Part A — Light up the screen (SPI)

### Wire the OLED to the ESP32
| OLED pin | ESP32 pin |
|----------|-----------|
| GND | GND |
| + | 3V3 |
| CK | GPIO18 |
| SI | GPIO23 |
| OC | GPIO5 |
| DC | GPIO16 |
| R | GPIO17 |

### Run it
1. Open `sketches/day2_01_oled_hello.ino` in the Arduino IDE.
2. Choose **Tools → Port** (your ESP32), click **Upload** (→ arrow).
3. **Expected:** a yellow box with "Battery Monitor / Day 2 :)".

✅ **Win #1** — if you see text, your SPI display works!

> **Concept:** SPI is how the ESP32 sends pixels to the screen. It uses a clock (CK), a data line (SI), and a chip-select (OC).

---

## Part B — Find your sensor (I2C)

### Add the INA219 (leave the OLED connected)
| INA219 pin | ESP32 pin |
|------------|-----------|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

```
   ESP32                 INA219
  3V3  ─────────────────  VCC
  GND  ─────────────────  GND     ← connect this one FIRST
  GPIO21 ───────────────  SDA
  GPIO22 ───────────────  SCL
```

### Run it
1. Open `sketches/day2_02_i2c_scanner.ino`, Upload.
2. Open **Tools → Serial Monitor**, set the speed (bottom-right) to **115200**.
3. **Expected:** "Found device at 0x40" in the Serial Monitor, and `0x40` on the screen.

❓ **Nothing found?** Check: GND connected? SDA on 21, SCL on 22? VCC on 3V3? Sensor pushed firmly into the breadboard?

✅ **Win #2** — your ESP32 can find sensors on the I2C bus.

> **Concept:** I2C is a different bus from SPI — only 2 wires (SDA + SCL), and each device has an *address* (yours is 0x40).

---

## Part C — Read your pack

### Add the battery to the sensor
**To measure voltage only** (easiest first test):
```
Pack(+) ──┬── INA219 Vin+
          └── INA219 Vin-      (jumper both together)
Pack(-) ───── GND rail (common ground!)
```

### Run it
1. Open `sketches/day2_03_single_pack.ino`, Upload.
2. **Expected on screen:** Pack voltage (V), current (mA ≈ 0 with no load), and **SoC %** with a colour bar.
3. Open the Serial Monitor — you'll see `V,mA,SoC%` values streaming.

### Try this
- Compare the screen's voltage with your **multimeter** from yesterday. Close? That's calibration.
- Connect a small load and watch the **current** reading change and the voltage sag.

✅ **End-of-day check** — one pack's voltage, current, and SoC live on the screen.

**Tomorrow:** watch a battery charge (CC/CV), then build the full 8-pack wall.
