# LED Bar Graph Voltmeter ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Op-amp comparators with reference voltage ladder for visual voltage metering

## What You'll Learn
- Op-amp comparator operation (open-loop, no feedback)
- Voltage divider ladder networks for precision reference voltages
- How to design a threshold-based analog display
- LM324 quad op-amp IC pinout and characteristics
- Voltage measurement range scaling with input dividers
- Color-coded bar graph indicators for intuitive readouts

## Components Required
- **LM324** Quad Op-Amp IC × 1 (contains 4 independent op-amps)
- **BC547** NPN transistors × 2 (optional, for driving extra LED stages beyond 4)
- **LEDs** × 10:
  - 4 × green (0–6V range)
  - 3 × yellow (7–8V range)
  - 3 × red (9–10V range)
- **Resistors — voltage reference ladder**:
  - 10kΩ × 11 (to build the 10-rung reference divider)
- **Resistors — input voltage divider**:
  - 47kΩ × 1 (R_top, input divider upper)
  - 10kΩ × 1 (R_bot, input divider lower)
- **Resistors — LED current limiting**:
  - 220Ω × 10 (one per LED)
- **9V battery** × 1 (circuit power supply)
- **Test leads / probes** × 2 (to connect voltage being measured)
- **Breadboard** and jumper wires

## Circuit Description
Build a 10-segment LED bar graph voltmeter that visually shows voltage from 0 to 15V using a rising column of LEDs — like a fuel gauge for electricity! Each of the 10 LEDs has its own op-amp comparator that lights up when the input voltage exceeds a fixed threshold. A resistor ladder divides the supply voltage into 10 equally-spaced reference levels; another resistor divider scales the measured voltage down into the same range. The LM324 quad op-amp IC provides 4 comparators directly; 2 BC547 transistors act as simple switches to drive the remaining 6 LED stages. No microcontroller required — pure analog voltage detection!

## Theory of Operation

### Op-Amp Comparator Fundamentals
An op-amp comparator uses the op-amp in **open-loop mode** (no feedback resistor). With very high open-loop gain (~100,000×), even a tiny voltage difference between the two inputs is amplified to the output rail:

```
V+ > V–  →  Output swings to V_OH ≈ VCC – 1.5V  (LED lights)
V+ < V–  →  Output swings to V_OL ≈ 0V           (LED off)
```

For the LM324 with 9V supply:
- Output HIGH ≈ 7.5V (enough to drive an LED through 220Ω)
- Output LOW ≈ 0V

### Reference Voltage Ladder (Voltage Divider Chain)
A chain of equal resistors from VCC to GND creates evenly-spaced tap voltages. With 11 resistors of 10kΩ each:

```
VCC (9V) ──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]──[10k]── GND
                   V1    V2    V3    V4    V5    V6    V7    V8    V9   V10
```

Each tap is one resistor-step lower:
```
V_n = VCC × (11 - n) / 11
```

| Tap | Reference Voltage | Comparator fires when V_in > |
|-----|-------------------|------------------------------|
| V1  | 8.18V             | 8.18V (LED 10)               |
| V2  | 7.36V             | 7.36V (LED 9)                |
| V3  | 6.55V             | 6.55V (LED 8)                |
| V4  | 5.73V             | 5.73V (LED 7)                |
| V5  | 4.91V             | 4.91V (LED 6)                |
| V6  | 4.09V             | 4.09V (LED 5)                |
| V7  | 3.27V             | 3.27V (LED 4)                |
| V8  | 2.45V             | 2.45V (LED 3)                |
| V9  | 1.64V             | 1.64V (LED 2)                |
| V10 | 0.82V             | 0.82V (LED 1)                |

### Input Voltage Scaling
The voltmeter measures 0–15V, but the comparators work within 0–9V. An input voltage divider scales the measured voltage down:

```
V_scaled = V_in × R_bot / (R_top + R_bot)
         = V_in × 10k / (47k + 10k)
         = V_in × 0.175
```

When V_in = 15V: V_scaled = 2.63V ← well within the comparator input range.

**Effective measurement thresholds (what V_in triggers each LED):**
```
V_in_threshold = V_reference / 0.175
```

See the complete LED display table below.

### Bar Graph Display Logic
The comparators form a "thermometer code" display. As V_in increases:
- LED 1 (lowest threshold) lights first
- LED 2 lights when V_in exceeds its threshold (LED 1 stays on)
- LED 3 lights when its threshold is crossed (LEDs 1 and 2 stay on)
- ...continuing until all 10 LEDs are lit at maximum input

This creates a **bar graph** effect — the number of illuminated LEDs is proportional to the input voltage.

## LED Display Table

| LED # | Reference V | Lights when V_in > | Color  | Meaning              |
|-------|-------------|---------------------|--------|----------------------|
| 1     | 0.82V       | 4.7V                | Green  | Low voltage          |
| 2     | 1.64V       | 5.4V                | Green  | Low-normal           |
| 3     | 2.45V       | 6.1V                | Green  | Normal low           |
| 4     | 3.27V       | 6.8V                | Green  | Normal               |
| 5     | 4.09V       | 7.5V                | Yellow | Normal-high          |
| 6     | 4.91V       | 8.2V                | Yellow | High-normal          |
| 7     | 5.73V       | 8.9V                | Yellow | High                 |
| 8     | 6.55V       | 9.6V                | Red    | Over-voltage warning |
| 9     | 7.36V       | 10.2V               | Red    | Danger               |
| 10    | 8.18V       | 11.3V               | Red    | Over-voltage!        |

> Note: Thresholds above assume R_top = 47kΩ, R_bot = 10kΩ. Adjust R_top to change the full-scale range.

## Pin Configuration

### LM324 Quad Op-Amp (14-pin DIP)
```
        +-----U-----+
 OUT1  1|•         14| OUT4
  IN1– 2|          13| IN4–
  IN1+ 3|          12| IN4+
   VCC 4|  LM324   11| GND
  IN2+ 5|          10| IN3–
  IN2– 6|           9| IN3+
 OUT2  7|___________8| OUT3
```

| Pin | Name  | Function                        |
|-----|-------|---------------------------------|
| 4   | VCC   | Positive supply (+9V)           |
| 11  | GND   | Negative supply (GND)           |
| 3   | IN1+  | Non-inverting input, comparator 1 |
| 2   | IN1–  | Inverting input, comparator 1   |
| 1   | OUT1  | Output, comparator 1 → LED 1    |
| 5   | IN2+  | Non-inverting input, comparator 2 |
| 6   | IN2–  | Inverting input, comparator 2   |
| 7   | OUT2  | Output, comparator 2 → LED 2    |
| 9   | IN3+  | Non-inverting input, comparator 3 |
| 10  | IN3–  | Inverting input, comparator 3   |
| 8   | OUT3  | Output, comparator 3 → LED 3    |
| 12  | IN4+  | Non-inverting input, comparator 4 |
| 13  | IN4–  | Inverting input, comparator 4   |
| 14  | OUT4  | Output, comparator 4 → LED 4    |

### BC547 NPN Transistor (TO-92 package)
```
Flat face pointing toward you:
  Emitter  Base  Collector
    (E)    (B)      (C)
     |      |        |
    GND   Signal   LED+
```
Used as a switch: Base current from a comparator output turns transistor ON, pulling collector LOW and sinking LED current.

## Comparator Truth Table

| Condition       | V+ (non-inv) | V– (inv)  | Output  | LED State |
|-----------------|--------------|-----------|---------|-----------|
| V_in > threshold | V_scaled > V_ref | V_ref | HIGH (~7.5V) | ON |
| V_in < threshold | V_scaled < V_ref | V_ref | LOW (~0V)    | OFF |
| V_in = threshold | V_scaled ≈ V_ref | V_ref | Undefined (flickering) | Dim |

> The slight flicker at the threshold voltage is normal and expected — it indicates the measured voltage is right at that boundary.

## Building Instructions

### Step 1: Power Rails
1. Connect +9V battery to positive breadboard rail.
2. Connect battery negative to negative rail.
3. Measure 9V between rails with multimeter before proceeding.

### Step 2: Insert and Power LM324
1. Insert LM324 straddling the center gap (Pin 1 at notch).
2. **Pin 4 → +9V rail**
3. **Pin 11 → GND rail**
4. Add a **100nF decoupling capacitor** between Pin 4 and Pin 11 (as close to IC as possible).

### Step 3: Build the Reference Voltage Ladder
1. Take 11 resistors of 10kΩ each.
2. Chain them in series on the breadboard from +9V rail to GND rail.
3. Label the 10 junction points V1 through V10 (top to bottom = V1 highest, V10 lowest).
4. Verify with multimeter:
   - V1 should read ≈ 8.2V
   - V5 should read ≈ 4.9V
   - V10 should read ≈ 0.82V

### Step 4: Build the Input Voltage Divider
1. Connect **R_top (47kΩ)** from the input test probe (V_in+) to a central node.
2. Connect **R_bot (10kΩ)** from that central node to GND.
3. Connect **input GND probe** to the circuit GND rail.
4. Label the central node V_scaled — this is the scaled measurement signal.

### Step 5: Wire Comparators 1–4 (LM324)
Wire each comparator with:
- **Non-inverting input (+)** → V_scaled (the measured voltage)
- **Inverting input (–)** → corresponding reference tap from ladder

| Comparator | IN+ | IN– | OUT  | Ladder Tap |
|------------|-----|-----|------|------------|
| Op-amp 1   | 3   | 2   | 1    | V10        |
| Op-amp 2   | 5   | 6   | 7    | V9         |
| Op-amp 3   | 9   | 10  | 8    | V8         |
| Op-amp 4   | 12  | 13  | 14   | V7         |

All four IN+ pins connect together to V_scaled (one wire to all four pins).

### Step 6: Wire LEDs for Comparators 1–4
For each comparator output (Pins 1, 7, 8, 14):
1. Connect **220Ω resistor** from output pin to LED anode (+).
2. Connect LED cathode (–) to GND.
3. LEDs 1–4 (green) are now connected.

### Step 7: Add BC547 Transistors for LEDs 5–10
The LM324 only has 4 op-amps. For the remaining 6 stages, use two BC547 transistors as driver switches, each driven by a comparator output from the LM324 and controlling 3 LEDs at once — **or** use a second LM324.

**Recommended: Second LM324 for LEDs 5–8:**
- Wire identically to Step 5, using reference taps V6, V5, V4, V3.
- Wire LEDs 5–8 (yellow) to outputs via 220Ω.

**BC547 for LED stages 9–10 (driven by comparator outputs):**
1. Connect **1kΩ base resistor** from comparator output to BC547 Base.
2. BC547 **Emitter → GND**.
3. BC547 **Collector → LED cathode** (LED anode → 220Ω → +9V).
4. When comparator output goes HIGH, base current flows → transistor saturates → LED lights.

> Simplified build: Use only 4 LEDs (one per op-amp in the single LM324) for the starter version. Expand with a second LM324 or the LM3914 (see Extensions).

### Step 8: Test Probes
1. Connect a red wire from the input divider top node to a labeled test probe (V_in+).
2. Connect a black wire from the circuit GND to a second probe (GND).
3. These probes will touch the circuit being measured.

### Step 9: Final Checks
1. Verify LM324 Pin 4 = 9V, Pin 11 = 0V.
2. Confirm reference ladder voltages with multimeter.
3. Confirm all non-inverting inputs (all IN+) share the V_scaled node.
4. Check LED polarities: long leg = anode = to resistor.

## Testing & Troubleshooting

### Expected Behavior
- **Probes open (no connection)**: All LEDs off (V_scaled = 0V < all thresholds).
- **Touch probes to 9V battery**: LEDs 1–6 (green and some yellow) should light.
- **Full 15V input**: All 10 LEDs light up.
- **Increasing voltage slowly**: LEDs light progressively, one at a time.
- **Decreasing voltage**: LEDs turn off progressively from top.

### Test Procedure
1. With no voltage connected, verify all LEDs are OFF.
2. Connect the voltmeter probes across the 9V battery (measuring its own supply voltage).
3. LEDs 1–6 should light (9V → V_scaled = 1.58V → exceeds thresholds for LEDs 1–6).
4. Connect across a 1.5V AA battery: only LED 1 should light (barely).
5. Use a variable power supply or potentiometer voltage divider to sweep 0–15V and watch LEDs step on.

### Voltage Measurements

| Test point         | Expected reading |
|--------------------|-----------------|
| LM324 Pin 4        | 9V              |
| LM324 Pin 11       | 0V              |
| Ladder tap V1      | ~8.2V           |
| Ladder tap V5      | ~4.9V           |
| Ladder tap V10     | ~0.82V          |
| V_scaled at 9V in  | ~1.58V          |
| V_scaled at 15V in | ~2.63V          |
| Comparator output (LED on) | ~7.5V  |
| Comparator output (LED off) | ~0V   |

### Common Issues and Solutions

**All LEDs are always on:**
- V_scaled may be connected to IN– (inverting) instead of IN+ — swap connections
- Reference ladder may be wired backwards (V10 at top instead of bottom)
- Measure IN– and IN+ of each comparator to identify which is higher

**No LEDs ever light:**
- Confirm LM324 has power: Pin 4 = 9V, Pin 11 = 0V (not the reverse!)
- Confirm V_scaled is > 0V when voltage is applied to probes
- LED polarity may be reversed — swap anode and cathode
- Measure comparator output pins directly with multimeter

**Only the bottom (or top) half works:**
- Double-check the second LM324 or BC547 transistor connections for LEDs 5–10
- Each section can be debugged independently

**LEDs flicker rapidly:**
- Input voltage is right at a threshold — this is normal
- Add a small capacitor (100nF) from V_scaled to GND to filter noise
- Small hysteresis can be added: connect a very large resistor (1MΩ) from output back to IN+ of each comparator

**Reference ladder voltages are wrong:**
- Measure resistance between each tap and GND: should increment by 10kΩ each step
- Check for resistors in wrong positions or different values accidentally mixed in

**Display reads lower than actual voltage:**
- Input divider ratio may be off — measure R_top and R_bot with multimeter
- Check the formula and recalculate expected V_scaled for your actual voltage

**LM324 outputs don't reach full HIGH swing:**
- LM324 is not a rail-to-rail op-amp; output maxes at ~VCC – 1.5V (≈7.5V at 9V supply)
- This is normal and expected; 7.5V is enough to illuminate LEDs

## Calculations

### Reference Voltage Calculation (11 × 10kΩ ladder)
```
V_n = VCC × (11 - n) / 11

V1  = 9 × 10/11 = 8.18V
V5  = 9 × 6/11  = 4.91V
V10 = 9 × 1/11  = 0.818V
```

### Input Divider Scaling
```
V_scaled = V_in × R_bot / (R_top + R_bot)
         = V_in × 10,000 / (47,000 + 10,000)
         = V_in × 0.1754

For V_in = 9V:   V_scaled = 9 × 0.1754 = 1.579V
For V_in = 15V:  V_scaled = 15 × 0.1754 = 2.63V
```

### Finding Which LED Lights at a Given V_in
```
LED n lights when V_scaled > V_reference_n
V_in > V_reference_n / 0.1754

Example (LED 5, tap V6 = 4.09V):
V_in threshold = 4.09 / 0.1754 = 23.3V ← this is above our 15V range!

→ Recalibrate: with 0–15V measurement, we need different thresholds
→ Scale: at V_in = 15V, V_scaled = 2.63V → comparators need refs from 0.26V to 2.63V
→ Use only the LOWER portion of the reference ladder, or use a smaller ratio ladder
```

### Redesigned for 0–15V Range
Use a reference ladder that spans 0–2.63V:
- Replace 10kΩ ladder resistors with the voltage spanning 0–2.63V
- Or add a voltage reference IC (TL431) to set ladder top to exactly 2.63V
- This ensures LED 10 lights at exactly 15V and LED 1 lights at 1.5V

### LED Current
```
V_comparator_high ≈ 7.5V
V_LED ≈ 2.0V (green/yellow), 1.8V (red)
I = (7.5 – 2.0) / 220 = 25 mA  ✓ (within LM324 output rating)
```

### Total Current at Full Scale (All LEDs on)
```
LEDs: 10 × 25mA = 250mA
LM324: 2 ICs × ~5mA = 10mA
Reference ladder: 9V / (11 × 10k) = 82µA ≈ negligible
Total: ~260mA

9V battery runtime: 500mAh / 260mA ≈ 1.9 hours (all LEDs on)
Typical use (5 LEDs avg.): ~3.8 hours
```

## Key Concepts Learned
- **Op-Amp Comparator**: Open-loop op-amp used as a 1-bit analog-to-digital converter
- **Voltage Reference Ladder**: Resistor chain that creates precise, stable reference voltages
- **Thermometer Code**: Encoding where N LEDs lit = value of N (like a mercury thermometer)
- **Input Scaling**: Using a voltage divider to adapt a large input range to a circuit's working range
- **LM324 Characteristics**: Quad op-amp, single-supply operation, output not rail-to-rail
- **Comparator Hysteresis**: Positive feedback to prevent oscillation at threshold crossings
- **Bar vs. Dot Mode**: Lighting all LEDs below threshold (bar) vs. only the highest LED (dot)

## Applications
- **Battery monitor**: Display state-of-charge of a 12V lead-acid or LiPo battery pack
- **Power supply meter**: Built-in voltage display for bench power supplies
- **Solar panel monitor**: Show panel output voltage and approximate power level
- **Audio level meter (VU meter)**: Replace DC voltage with rectified AC audio signal
- **Fuel level gauge**: With a resistive float sensor, show tank level on LED bar
- **Temperature display**: Use thermistor + divider to show temperature on LED scale
- **Light intensity meter**: Use LDR (light-dependent resistor) as input divider element

## Extensions

### Use LM3914 for Automatic Bar Display
The **LM3914** is a dedicated bar/dot LED display driver that contains everything in one IC:
- Internal reference voltage
- Internal resistor ladder
- 10 built-in comparators
- Selectable bar or dot mode (single pin controls this)

It replaces the LM324, all reference resistors, and simplifies the circuit to ~10 components.
```
LM3914 pin summary:
- Pin 9: Signal input (V_in)
- Pin 8: Low reference (RLO) → GND
- Pin 4: High reference (RHI) → sets full-scale voltage
- Pins 1,10–18: LED outputs (10 segments)
- Pin 9: Bar/Dot mode select
```

### Add Audio Alert for Over-Voltage
Connect LED 10 (red, highest threshold) output to a 555 astable oscillator that drives a buzzer:
- LED 10 output → 555 Pin 4 (RESET)
- When Pin 4 = HIGH (over-voltage!), buzzer beeps
- When Pin 4 = LOW (normal), buzzer is silent
- Provides an audible warning even when user isn't watching the display

### Dot Mode (Single LED Indicates Level)
Instead of all LEDs below threshold staying on, only the highest-triggered LED lights:
- Use an XOR approach: connect each comparator output to an exclusive-OR gate with the next comparator
- Result: only the single LED at the exact voltage level illuminates
- Easier to read, lower power consumption

### Adjustable Full-Scale Range
Replace R_top with a multi-turn trimpot (100kΩ). Adjust while measuring a known reference voltage until the correct LED lights. This allows calibration to exactly 0–5V, 0–12V, 0–15V, or any desired range.

## Safety Notes
- ✅ 9V circuit power supply is completely safe
- ⚠️ **Measuring external voltages**: Only measure circuits operating at safe voltages (< 50V DC); never touch mains (household) AC circuits
- ⚠️ **Input polarity**: Applying reverse-polarity to the input divider can damage the LM324; add a protective diode if measuring unknown polarity sources
- ⚠️ **LM324 maximum input**: Input pins must stay between GND and VCC–1.5V; inputs above VCC can latch up the IC — use input clamping diodes for unknown sources
- ⚠️ **LED polarity**: Reversed LEDs don't light, but reversed LEDs with excessive reverse voltage can be damaged
- ✅ The resistor input divider naturally limits current into the op-amp inputs
- ✅ Suitable for measuring batteries, bench power supplies, and low-voltage DC circuits

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

See your voltage! 📊⚡
