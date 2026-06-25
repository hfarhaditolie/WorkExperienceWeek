# Student Wiring Guide — Battery, Charging, and the Race

## Introduction
In this project you will build a small battery system from scratch. You will **charge** lithium cells safely, **connect cells in series** to make a higher-voltage battery pack, use that pack to **drive a toy car** around a track, and **measure the current** flowing in (charging) and out (driving) using a sensor and a microcontroller.

This guide explains **every component** — what it is, what it does, the voltage and current it works at, and how it wires to the others. Then it walks through the three tasks:
- **Task A — Charge each cell** (one cell per charger).
- **Task B — Build the series pack** (1 → 4 cells) to power the car.
- **Task C — Measure** charge and discharge current with the sensor.

> ⚠️ **Read the Safety section before touching any battery.** Lithium cells can overheat, vent, or catch fire if shorted, over-charged, or wired wrong.

---

## SAFETY FIRST (read this!)
- **Never short a battery** — never connect + directly to − with wire. The only thing between + and − should be a *load* (a motor) or a *charger*. A short can burn wires instantly and damage the cell.
- **Charge only with the TP4056**, one cell at a time, to a maximum of **4.2 V**. Never connect a series pack to a TP4056.
- **Check polarity** with a multimeter before powering anything. Reversed lithium is dangerous.
- **Always connect ground (−) first**, then power, then signal wires. Only wire with power disconnected.
- **Watch the car's voltage limit** — adding cells in series multiplies the voltage. Too much voltage will burn out the car's motor (see Task B).
- Charge and test on a **fire-safe surface, with a supervisor present**. Stop instantly if anything gets hot, swells, or smells.

---

# The Components

## 1. ESP32 Microcontroller (×1)
**What it is:** A small programmable computer — the "brain" of the project. It reads the sensor and shows the numbers.
**What it does:** Runs your code, talks to the current sensor over a 2-wire bus (I2C), and prints/displays the readings.
**Operating voltage & current:** Runs on **3.3 V logic**; powered from **5 V via its USB port** (it makes its own 3.3 V inside). Its pins output **3.3 V** and only a few milliamps — they are for *signals*, never for powering a motor.
**Wires to:**
- **USB-C cable → laptop or power supply** (to power the ESP32).
- **3V3 pin →** sensor VCC. **GND pin →** sensor GND and the common ground.
- **P21 →** sensor SDA, **P22 →** sensor SCL (the I2C data/clock lines).

## 2. Breadboard (×2)
**What it is:** A board full of holes for building circuits without soldering.
**What it does:** Connects components. **Holes in the same row (group of 5) are joined together.** The two long side strips are the **power rails** (+ red and − blue).
**Operating voltage & current:** Fine for low-current signals and the 3.3 V/5 V logic side. ⚠️ **Not for high motor current** — thin breadboard contacts and jumpers can overheat above ~1–2 A. The high-current path to the car should use **direct, thicker wires**, not the breadboard rails.
**Wires to:** Everything on the logic/charging side plugs into it. Use one breadboard for the **ESP32 + sensor (logic)** and keep the **high-current battery/motor path** on direct wiring.

## 3. Li-ion Batteries (×4) and Battery Holders (×4)
**What it is:** Rechargeable 18650 lithium cells; the holders clip each cell and give you + and − wires.
**What it does:** Store energy. One cell powers the logic/charging tests; several in series make a bigger pack for the car.
**Operating voltage & current:** Each cell is **3.7 V nominal**, **4.2 V full**, and must **not go below ~3.0 V** (empty). Capacity ~**2000 mAh (2 Ah)**. Can deliver several amps.
**Wires to:**
- For **charging:** holder + and − → TP4056 **B+ / B−** (one cell per charger).
- For the **pack:** holders are joined **in series** (see Task B).
- Holder **−** is part of the **common ground**.

## 4. TP4056 Charger (×4)
**What it is:** A small lithium-charging board with a USB input and status LEDs.
**What it does:** Charges **one** lithium cell using the correct **CC/CV** method, stopping automatically at **4.2 V**.
**Operating voltage & current:** Input **5 V**; charge cutoff **4.2 V**; **max 1 A** charge current. **Red LED = charging, blue/green = full.** *(Flashing red = no/loose battery.)*
**Wires to:**
- **USB-C or IN+ / IN− →** 5 V (USB cable or power supply).
- **B+ / B− →** the **battery** being charged.
- **OUT+ / OUT− →** (optional) a protected output to a load — not used for plain charging.
> ⚠️ **One cell per TP4056. Never connect it to a 2/3/4-cell series pack** — it can only reach 4.2 V, so it cannot charge a higher-voltage stack.

## 5. INA219 Current Sensor (×4)
**What it is:** A digital sensor that measures voltage and current and reports them to the ESP32.
**What it does:** Sits **in series** in a wire so all the current flows through it; measures that current and the voltage, and sends the numbers to the ESP32 over I2C.
**Operating voltage & current:** Chip powered from **3.3 V** (the ESP32). Can measure bus voltage up to **26 V** and current up to about **±3.2 A** (with its standard 0.1 Ω shunt). ⚠️ **Above ~3 A it clips and the shunt overheats** — check your motor's current.
**Wires to:**
- **Logic side → ESP32:** VCC→3V3, GND→GND, SDA→P21, SCL→P22.
- **Measurement side (Vin+ / Vin−) → in series** with the current path: current goes **in Vin+ and out Vin−**.
  - Charging: `TP4056 B+ → Vin+ → Vin− → Battery+`.
  - Discharging: `Pack+ → Vin+ → Vin− → Car+`.

## 6. Jumper Wires (×4 sets)
**What it is:** Pre-made wires with pin ends for breadboard connections.
**What it does:** Carry signals and low currents between parts.
**Operating voltage & current:** Fine for logic and ~1 A. ⚠️ Thin jumpers can overheat on the **high-current motor path** — use thicker wire there.
**Wires to:** Everything on the logic side; colour-code if you can (red = +, black = −).

## 7. USB-C Cable (×5)
**What it is:** A standard USB-C lead.
**What it does:** Carries **5 V power** to the ESP32 and to the TP4056 chargers.
**Operating voltage & current:** **5 V**; carries up to several amps (far more than needed here).
**Wires to:** Laptop/power supply → ESP32, and 5 V source → each TP4056.

## 8. Power Supply (×1)
**What it is:** A mains-powered source of low-voltage DC.
**What it does:** Provides **5 V** to run several TP4056 chargers (and/or the ESP32) at once.
**Operating voltage & current:** Output **5 V**; current rating must cover everything you run together (each TP4056 draws up to ~1 A, so 4 charging at once ≈ 4 A → use a **5 V / ≥5 A** supply).
**Wires to:** 5 V out → the chargers' inputs (in parallel) and/or the ESP32.
> ⚠️ Confirm the supply is genuinely **5 V** before connecting TP4056s — a higher voltage will damage them.

## 9. Toy Cars (×2) and Controllers (×2)
**What it is:** The cars are the **load** you drive with the battery pack. The controllers are the handheld radio remotes.
**What it does:** The car's motor turns battery energy into movement. The controller sends steering/throttle by radio.
**Operating voltage & current:** ⚠️ **Check the car's rated voltage** (toy cars are often ~4.8–6 V). The motor may pull **1–3 A or more**, especially when starting or stalled. **Controllers use their own batteries** (usually AA or a 9 V) and are **not** wired into your battery system.
**Wires to:** The series pack (through the INA219) connects to the car's **motor / battery terminals**: `Pack+ → INA219 → Car+`, `Car− → Pack−`.
> ⚠️ **Do not exceed the car's rated voltage.** A 4-cell series pack is ~14.8 V — if the car is rated for 6 V, this will quickly burn out the motor. See Task B.

## 10. Racing Track (×4–10) and Lap Counter (×1)
**What it is:** Track sections that join into a loop, plus a device that counts laps.
**What it does:** Gives a fair, repeatable course; the lap counter records how far each car gets before its battery dies.
**Operating voltage & current:** Passive track (no power). The lap counter is **self-powered** (its own batteries/USB) — follow its own instructions.
**Wires to:** Nothing in the battery circuit — set them up physically around the track.

---

# Task A — Charge each cell (separately)
Because a TP4056 only charges to **4.2 V**, you must charge **each cell on its own charger**, never the series pack.

1. Set the power supply (or USB) to **5 V** into each TP4056 input.
2. For each cell: **B+ → battery +**, **B− → battery −** (check polarity!).
3. With 4 chargers and 4 cells, you can charge **all four at once** — each on its own TP4056, fed from the shared 5 V supply (size it for ~4 A total).
4. **Red LED = charging, blue/green = full.** It stops automatically.

**To measure the charge current** (optional), put an INA219 in series on the B+ line:
`TP4056 B+ → Vin+ → Vin− → Battery+`, and `B− → Battery−`, with the INA219 logic side on the ESP32.

---

# Task B — Build the series pack (1 → 4 cells)
Series wiring **adds the voltages**: each extra cell adds ~3.7 V.

| Cells in series | Pack voltage (nominal) | Pack voltage (full) |
|-----------------|------------------------|---------------------|
| 1 | 3.7 V | 4.2 V |
| 2 | 7.4 V | 8.4 V |
| 3 | 11.1 V | 12.6 V |
| 4 | 14.8 V | 16.8 V |

**How to wire cells in series** (link + of one to − of the next):
```
 Holder1(-) = PACK(-)
 Holder1(+) ── Holder2(-)
 Holder2(+) ── Holder3(-)
 Holder3(+) ── Holder4(-)
 Holder4(+) = PACK(+)
```
Add cells **one at a time** (start with 1, then 2, 3, 4) and watch how the car speeds up.

> ⚠️ **STOP at the car's rated voltage.** If the car is rated ~6 V, going past 2 cells (7.4 V) risks burning the motor — keep higher-cell tests brief or use them only to *measure*, not to run hard. Confirm the car's limit with a facilitator before adding cells.
>
> ⚠️ **Charging vs. running are separate.** Take cells **out of series** to charge them individually on the TP4056. Never charge the assembled pack.

---

# Task C — Measure discharge while running the car
Put one INA219 **in series between the pack and the car**, so the driving current flows through it:
```
 PACK(+) ── INA219 Vin+ ── INA219 Vin- ── Car(+)
 Car(-) ─────────────────────────────────── PACK(-)
 PACK(-) ── ESP32 GND        (common ground for the sensor)
 INA219: VCC→3V3  GND→GND  SDA→P21  SCL→P22
```
The ESP32 then reads the **discharge current** and **pack voltage** and shows them on screen / Serial.

> ⚠️ **Current limit:** measure the car's running current first. If it exceeds ~3 A, the INA219 will clip and its shunt will get hot — use a heavier-duty sensor (INA226) or a thicker shunt for that car.
>
> ⚠️ **High-current wiring:** the pack-to-car path should use **direct, thicker wires**, not thin breadboard jumpers.

---

# Quick wiring summary
| Connection | From | To |
|------------|------|-----|
| Power the brain | USB-C 5 V | ESP32 |
| Sensor power | ESP32 3V3 / GND | INA219 VCC / GND |
| Sensor data | ESP32 P21 / P22 | INA219 SDA / SCL |
| Charge a cell | TP4056 B+ / B− | one battery + / − |
| Charger power | 5 V supply / USB-C | TP4056 input |
| Series pack | Holder + → next Holder − | (chain of 4) |
| Drive the car | Pack+ → INA219 → Car+ ; Car− → Pack− | (through sensor) |
| Common ground | Pack− , Car− , ESP32 GND | all joined |

**Golden rules:** charge one cell at a time (≤4.2 V) · never short + to − · respect the car's voltage limit · keep motor current off the breadboard · ground first, power last.
