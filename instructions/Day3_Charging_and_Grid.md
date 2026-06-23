# Day 3 — Charge It, and Build the Wall

**Today's goal:** Watch a battery charge the proper way (CC/CV), then combine all the teams into one 8-pack display wall.

> ⚠️ **Charging safety:** charge only on the fire-safe mat, with a facilitator watching. Never leave charging unattended. Stop instantly if anything gets hot or puffy.

---

## Part A — Watch a battery charge (CC/CV)

### Wire the charge path
```
 5V supply ──► TP4056 IN+ / IN-
 TP4056 BAT+ ──► INA219 Vin+ ──► INA219 Vin- ──► Pack(+)
 TP4056 BAT- ──────────────────────────────────► Pack(-)
 Pack(-) ──► common GND  (shared with ESP32 GND)
 INA219: VCC->3V3  GND->GND  SDA->GPIO21  SCL->GPIO22
```
Use a **partly-empty** pack so there's something to watch.

### Run it
1. Open `sketches/day3_01_charge_monitor.ino`.
2. Near the top, set `CHARGE_CURRENT_mA` to the value your facilitator gives you.
3. Upload. Start charging.

### What to watch (annotate the printed graph!)
| Phase | Voltage | Current | Screen says |
|-------|---------|---------|-------------|
| **CC** (constant current) | climbing | roughly flat | "CC (charge)" |
| **CV** (constant voltage) | ~4.2 V, steady | falling/tapering | "CV (taper)" |
| **Full** | ~4.2 V | tiny | "FULL" |

> **Concept:** A lithium charger pushes a **constant current** until the cell reaches 4.2 V, then holds a **constant voltage** while the current tapers off. That two-stage recipe is **CC/CV** — it fills the cell fast but safely.

✅ **Win #1** — you can point to the CC and CV phases on your own screen.

---

## Part B — Build the 8-pack wall

Now the teams combine. All 8 sensors share **one** I2C bus and feed **one** ESP32 + screen.

### Wiring
- Every INA219: **VCC→3V3**, **GND→GND**, **SDA→GPIO21**, **SCL→GPIO22** (all share the same 4 rails).
- Each sensor must have a **different address**. Check the table:

| Pack | Address |
|------|---------|
| 1 | 0x40 |
| 2 | 0x41 |
| 3 | 0x44 |
| 4 | 0x45 |
| 5 | 0x48 |
| 6 | 0x49 |
| 7 | 0x4C |
| 8 | 0x4D |

> Tip: run `day2_02_i2c_scanner.ino` first — you should see **all 8 addresses** listed. If one is missing, fix that sensor before moving on.

### Run it
1. Open `sketches/day3_02_eight_pack_grid.ino`, Upload.
2. **Expected:** a 2×4 grid, each cell showing **P1…P8**, an **SoC %**, and a colour bar (green = full, red = low). Missing sensors show "--".

### Try this
- Swap a charged pack for an empty one — watch its cell change colour.
- Compare two cells' SoC and predict which car will run longest tomorrow.

✅ **End-of-day check** — all available packs shown live on the grid wall.

**Tomorrow:** charge everything to full, then RACE the cars until the batteries die — and find out who delivered the most energy.
