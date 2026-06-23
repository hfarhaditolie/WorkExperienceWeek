# Day 4 — The Race (and the Science)

**Today's goal:** Run the cars off your packs until the batteries die, log the data, and figure out *why* some packs last longer than others. Then present what you found.

> ⚠️ **Discharge safety:** never run a pack below **3.0 V** — the sketch flags "DEAD" at that point, and facilitators enforce it. Over-discharging damages lithium cells.

---

## Step 1 — Fair start line
Charge **every** pack to full (4.2 V) using Day 3's charger setup. Everyone starts equal.

## Step 2 — Make a prediction (hypothesis)
Before racing, write down what you think will happen and why. Pick ONE variable to change and keep everything else the same. Examples:
- "The **slower** car's pack will deliver the most energy." (Higher speed = higher current = harder on the battery.)
- "The pack with the **biggest capacity** will run longest."

**Our hypothesis:** _______________________________________________

## Step 3 — Wire the car as the load
```
Pack(+) ──► INA219 Vin+ ──► INA219 Vin- ──► Car motor / speed controller
Pack(-) ──────────────────────────────────► common GND
INA219: VCC->3V3  GND->GND  SDA->GPIO21  SCL->GPIO22  (all 8 on the bus)
```

## Step 4 — Start logging, then RACE 🏎️
1. Open `sketches/day4_01_race_logger.ino`, Upload.
2. Open the **Serial Monitor** (115200). You'll see a CSV stream begin — this is your data.
3. **GO!** Run the cars. The screen shows each pack's SoC dropping; a pack reads **"DEAD"** when it hits 3.0 V.
4. Keep going until the packs are dead. Note how long each car ran / how many laps.

## Step 5 — Get your data out
- Copy everything from the Serial Monitor.
- Paste into a spreadsheet (the first row is the column names).
- The columns per pack are: voltage (`_V`), current (`_mA`), and **delivered charge** (`_mAh`).

> **Concept — coulomb counting:** if you add up *current × time* over the whole race, you get the **total charge the pack delivered** in mAh. That's the `_mAh` column — a real measurement of how much the battery actually gave.

## Step 6 — Analyse
1. **Plot voltage vs. time** for each pack on one chart — these are **discharge curves**. Compare their shapes.
2. **Which pack delivered the most mAh?** Was it the slowest car?
3. Did the data support your hypothesis?

### The big result to look for
A pack pushed **harder** (higher current / faster car) usually delivers **less** total energy before dying — it sags more and hits the cutoff sooner. This is the **Peukert effect**, and it's the exact trade-off engineers manage in EVs, drones, and phones: go fast, or go far — rarely both.

## Step 7 — Present (5 minutes per team)
Cover:
- What you built (sensors + screen + code).
- Your hypothesis and what you changed.
- Your discharge curve and the delivered-mAh result.
- What you concluded — and why.

## Awards 🏆
- **Longest runtime** · **Most laps** · **Best engineered display** · **Best data story**

✅ **You did it** — you built a real battery monitoring system from scratch, charged and discharged lithium cells safely, and used data to uncover a genuine engineering trade-off.
