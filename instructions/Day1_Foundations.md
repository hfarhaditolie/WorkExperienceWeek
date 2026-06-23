# Day 1 — Understand the Battery

**Today's goal:** Learn what's actually inside a battery measurement, and measure a real pack by hand. No code yet — today is about the ideas and the multimeter.

## Safety first (read this!)
- A battery can deliver a LOT of current if you short it. **Never** touch the + and − terminals together (or with metal/tools).
- These are lithium cells. **Never** charge above 4.2 V, never let them get hot, never bend or puncture them.
- If a battery is hot, puffy, or smells — tell a facilitator immediately.

## The big ideas
| Word | Meaning | Everyday analogy |
|------|---------|------------------|
| **Voltage (V)** | Electrical "push" | Water *pressure* |
| **Current (A)** | Rate of flow of charge | Water *flow rate* |
| **Capacity (mAh)** | How much charge is stored | Size of the *tank* |
| **C-rate** | Current as a fraction of capacity | How fast you empty the tank |
| **State of Charge (SoC)** | How full the battery is, 0–100% | Fuel gauge |

**Key formula:** `current = C-rate × capacity`
> Example: a 2000 mAh pack at 0.5C → 0.5 × 2000 = **1000 mA = 1 A**.

**SoC from voltage:** a single Li-ion cell reads about **3.0 V when empty** and **4.2 V when full**. So its voltage is a rough fuel gauge.

## Activity 1 — Measure resting voltage
1. Set your multimeter to **DC Volts** (the V with a straight line).
2. Touch **red probe to Pack(+)**, **black probe to Pack(−)**.
3. Record the voltage. Using the curve below, estimate the SoC:

| Voltage | ~SoC |
|---------|------|
| 4.20 V | 100% |
| 4.00 V | 80% |
| 3.90 V | 65% |
| 3.80 V | 45% |
| 3.70 V | 25% |
| 3.60 V | 12% |
| 3.00 V | 0% |

**Write down:** my pack = ____ V → about ____ % SoC.

## Activity 2 — Watch voltage sag under load
1. Connect a small load (a bulb, resistor, or the car's motor) across the pack.
2. Measure the voltage **again, while the load is connected**.
3. Is it lower? By how much? That drop is caused by the battery's **internal resistance** — a real battery isn't a perfect source.

**Write down:** resting ____ V, under load ____ V, drop = ____ V.

## Activity 3 — Calculate your C-rate
Your facilitator will tell you the **charge current** the charger is set to and your **pack capacity**.
- Work out the C-rate: `C-rate = charge current ÷ capacity`.
- Example: 1000 mA ÷ 2000 mAh = **0.5C**.

**Write down:** charger = ____ mA, capacity = ____ mAh → C-rate = ____ C.

## Activity 4 — Meet your kit
Find and identify each part. What do you think each one does?
- **ESP32** — the little computer (microcontroller).
- **OLED display** — the colour screen.
- **INA219** — the sensor that measures voltage and current.
- **TP4056** — the charger.

## End-of-day check ✅
You can state your pack's voltage, its estimated SoC, the C-rate, and name every component.

**Tomorrow:** we make the ESP32 talk to the screen and read your first sensor.
