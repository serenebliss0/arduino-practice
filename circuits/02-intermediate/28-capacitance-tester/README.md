# Capacitance Tester ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - 555 timer in monostable mode with visual readout

## What You'll Learn
- 555 timer IC in monostable (one-shot) mode
- How pulse width encodes a physical measurement
- The fundamental RC time-constant relationship
- Using LEDs as a visual analog meter / range indicator
- Calibration procedure for analog measurement circuits
- How professional capacitance meters work at the hardware level

## Components Required
- **NE555 or LM555** Timer IC × 1
- **CD4017** Decade Counter IC × 1 (optional — for LED range display)
- **LEDs** × 5 to 10 (different colors recommended: green, yellow, red)
- **Resistors**:
  - 10kΩ × 1 (R_timing — the fixed timing resistor)
  - 10kΩ × 1 (trigger pull-up)
  - 1kΩ × 1 (trigger button series resistor)
  - 220Ω × 5–10 (one per LED)
- **Push button** × 1 (momentary NO, to trigger measurement)
- **Test probes** × 2 (for connecting the unknown capacitor)
- **Capacitors** (known values for calibration):
  - 10nF × 1 (calibration reference)
  - 100nF × 1 (calibration reference)
- **9V battery** × 1
- **Battery connector**
- **Breadboard** and jumper wires

## Circuit Description
Build a hardware capacitance tester that identifies the value of an unknown capacitor without any microcontroller! The circuit exploits a fundamental property of the 555 timer in **monostable mode**: the output pulse duration equals exactly `1.1 × R × C`. With a precisely known timing resistor (R = 10kΩ), the output pulse width is directly proportional to the unknown capacitance (C). A CD4017 decade counter counts clock pulses that fill the monostable pulse window — more pulses means a wider pulse, which means a larger capacitor. The number of LEDs that light up reveals which capacitance range the mystery cap falls into. A brilliant example of converting time into a visual measurement!

## Theory of Operation

### The 555 Timer in Monostable Mode
In monostable mode the 555 timer has **one stable state** (output LOW). A brief LOW trigger pulse on Pin 2 kicks the circuit into its unstable state (output HIGH). It stays HIGH for a precisely calculated time determined by one resistor and one capacitor, then returns to LOW automatically.

**Internal operation:**
1. Normally: output LOW, discharge transistor (Pin 7) holds capacitor at 0V
2. Trigger arrives (Pin 2 goes below 1/3 VCC):
   - SR flip-flop inside 555 sets → output goes HIGH
   - Discharge transistor turns OFF → capacitor C_x starts charging through R_timing
3. Capacitor charges toward VCC
4. When capacitor reaches 2/3 VCC → upper comparator fires → flip-flop resets → output goes LOW
5. Discharge transistor turns ON → capacitor rapidly discharges → circuit ready for next trigger

### Pulse Width Formula
```
t = 1.1 × R_timing × C_unknown
```
Where:
- `t` = output pulse duration in seconds
- `R_timing` = fixed timing resistor in ohms
- `C_unknown` = capacitor under test in farads

This formula arises from the capacitor voltage equation:
```
V(t) = VCC × (1 - e^(-t/RC))
Setting V(t) = (2/3)×VCC:
2/3 = 1 - e^(-t/RC)
e^(-t/RC) = 1/3
-t/RC = ln(1/3) = -1.099
t = 1.1 × R × C  ✓
```

### Measuring an Unknown Capacitor
With R_timing = 10kΩ (fixed, precisely known):
- Measure the output pulse width with an oscilloscope, or
- Count how many clock pulses fit inside the monostable window (this is what the CD4017 does)

**Rearranging for C:**
```
C = t / (1.1 × R)
C = t / (1.1 × 10,000)
C = t / 11,000
```

### CD4017 as a Visual Pulse-Width Meter
A second 555 astable oscillator generates a fixed-frequency clock. The CD4017 counter advances one step per clock pulse. It starts counting when the monostable output goes HIGH (enable is gated) and stops when the output returns LOW. The LED illuminated by the final count indicates which "bin" the capacitor falls into.

## Expected Pulse Widths for Common Capacitor Values

Using **R_timing = 10kΩ**:

| Capacitor Value | Pulse Width (t = 1.1×R×C) | Practical Display          |
|-----------------|---------------------------|----------------------------|
| 100 pF          | 1.1 µs                    | Too short for LED display  |
| 1 nF            | 11 µs                     | Too short for LED display  |
| 10 nF           | 110 µs                    | Marginal — needs fast clock|
| 100 nF          | 1.1 ms                    | LED 1 flickers             |
| 1 µF            | 11 ms                     | LED 1–2 light briefly      |
| 10 µF           | 110 ms                    | LED 1–4 visible            |
| 100 µF          | 1.1 s                     | All LEDs step through      |
| 1000 µF         | 11 s                      | Full sweep, very slow      |

**Best measurement range for this circuit**: 100nF – 100µF (3 decades).

To measure smaller capacitors (pF range), increase R_timing to 1MΩ and use a microcontroller to time the pulse.

## Pin Configuration

### 555 Timer — Monostable Mode (8-pin DIP)
```
    +--U--+
GND |1   8| VCC (+9V)
TRG |2   7| DIS
OUT |3   6| THR
RST |4   5| CTL
    +-----+
```

| Pin | Name | Connection in monostable mode                        |
|-----|------|------------------------------------------------------|
| 1   | GND  | Ground                                               |
| 2   | TRG  | Normally HIGH via 10kΩ pull-up; button pulls LOW     |
| 3   | OUT  | Output pulse → CD4017 enable gate / LED              |
| 4   | RST  | +9V (always enabled)                                 |
| 5   | CTL  | 100nF to GND (noise bypass)                          |
| 6   | THR  | Connected to Pin 7 and timing capacitor (+)          |
| 7   | DIS  | Connected to Pin 6; R_timing connects here to VCC    |
| 8   | VCC  | +9V                                                  |

**Test probe connections (unknown capacitor C_x):**
- Probe A → Pin 6/7 junction
- Probe B → GND

### CD4017 Decade Counter (16-pin DIP)
```
       +--U--+
   Q5  |1  16| VDD (+9V)
   Q1  |2  15| RESET
   Q0  |3  14| CLOCK
   Q2  |4  13| CLOCK ENABLE (CE)
   Q6  |5  12| CARRY OUT
   Q7  |6  11| Q4
   Q3  |7  10| Q9
  VSS  |8   9| Q8
       +-----+
```

Key connections for this project:
- **Pin 14 (CLOCK)**: Clock from astable 555 (or external oscillator)
- **Pin 13 (CE)**: Active LOW enable — connect to monostable output inverted (or use NAND gate)
- **Pin 15 (RESET)**: Pulse HIGH before each measurement to reset to Q0
- **Pins 3,2,4,7,10,1,5** (Q0–Q6): Each drives one LED through 220Ω

## Building Instructions

### Phase 1: Monostable 555 (The Measurement Engine)

#### Step 1: Power Rails
1. Connect +9V to positive breadboard rail, GND to negative rail.

#### Step 2: Insert 555 IC (Monostable)
1. Place 555 on breadboard, Pin 1 at notch/dot.
2. **Pin 8 → +9V**, **Pin 1 → GND**, **Pin 4 → +9V**
3. **100nF** from Pin 5 to GND.

#### Step 3: Trigger Input
1. Connect **10kΩ pull-up** from +9V to Pin 2 (TRG).
2. Connect **push button** from Pin 2 (through a 1kΩ series resistor) to GND.
3. Resting state: Pin 2 = +9V (HIGH) → 555 in stable state.
4. Button pressed: Pin 2 → GND (LOW, below 1/3 VCC) → triggers monostable.

#### Step 4: Timing Resistor
1. Connect **R_timing (10kΩ)** from +9V to **Pin 7 (DIS)**.
2. Short-circuit Pin 7 to Pin 6 (THR) with a jumper.

#### Step 5: Test Probe Connections
1. Connect a wire from the Pin 6/7 junction to **Test Probe A** (alligator clip or header pin).
2. Connect a wire from GND to **Test Probe B**.
3. The unknown capacitor will be inserted between these two probes.

#### Step 6: Output Indicator
1. Connect **220Ω** from Pin 3 to an LED anode (+).
2. LED cathode to GND.
3. This LED lights for the entire monostable pulse duration — very useful for visual feedback during calibration.

### Phase 2: CD4017 LED Range Display (Optional but Recommended)

#### Step 7: Clock Oscillator (Second 555 — Astable)
1. Insert a second 555 IC on the breadboard.
2. Wire it in astable mode:
   - R1 = 1kΩ (Pin 8 → Pin 7)
   - R2 = 10kΩ (Pin 7 → Pins 2 and 6)
   - C = 1µF (Pins 2/6 → GND)
   - `f = 1.44 / ((1k + 20k) × 1µF) ≈ 68 Hz` — gives good resolution for µF-range caps
3. Output on Pin 3.

#### Step 8: CD4017 Counter
1. Insert CD4017, **Pin 16 → +9V**, **Pin 8 → GND**.
2. **Pin 14 (CLOCK)** → astable 555 Pin 3.
3. **Pin 13 (CE)** → Monostable 555 Pin 3 (inverted via NPN transistor or directly with logic inversion).
   - Simplification: Connect CE to GND (always count), and reset counter before each measurement.
4. **Pin 15 (RESET)** → push button (same measurement button via diode, so button press resets counter AND triggers monostable).

#### Step 9: LED Array
1. Connect LEDs (with 220Ω series resistors) to CD4017 outputs Q0–Q9:
   - Q0 (Pin 3) → LED 1 (green)
   - Q1 (Pin 2) → LED 2 (green)
   - Q2 (Pin 4) → LED 3 (green)
   - Q3 (Pin 7) → LED 4 (yellow)
   - Q4 (Pin 10) → LED 5 (yellow)
   - Q5 (Pin 1) → LED 6 (red)
   - Q6 (Pin 5) → LED 7 (red)
2. All LED cathodes to GND.

### Step 10: Final Verification
1. Confirm both 555 ICs have power (Pins 8 = 9V, Pins 1 = 0V).
2. Confirm electrolytic capacitors (if any) have correct polarity.
3. Verify test probes are connected to the monostable timing nodes only (not power).
4. Do NOT connect a capacitor to the probes yet.

## Calibration Procedure

Calibration ensures your tester's LED boundaries correspond to known capacitance values.

### Calibration Steps
1. **Zero test (no capacitor)**: Press button without anything on probes. Output LED should give a tiny brief flash (stray capacitance only). If it gives a long pulse, check wiring.

2. **Calibrate with 100nF**:
   - Connect known 100nF capacitor to test probes.
   - Press trigger button.
   - Observe monostable output LED duration: should be `1.1 × 10k × 100nF = 1.1 ms` (very brief).
   - CD4017 should advance only 1 or 2 LEDs (depending on clock frequency).

3. **Calibrate with 10µF**:
   - Connect known 10µF electrolytic capacitor (observe polarity! + to Probe A).
   - Expected pulse: `1.1 × 10k × 10µF = 110 ms`.
   - Should advance 7–8 LEDs on CD4017.

4. **Mark the scale**: Use tape next to the LEDs to label approximate capacitance values at each LED position.

5. **Adjust clock frequency** to match desired scale: slower clock = more LED steps per unit time = finer resolution for large caps.

### Calibration Table (with 68 Hz clock, R = 10kΩ)

| Capacitor | Pulse Width | LED Count (approx.) |
|-----------|-------------|---------------------|
| 100nF     | 1.1 ms      | 0–1                 |
| 1µF       | 11 ms       | 1                   |
| 10µF      | 110 ms      | 7–8                 |
| 47µF      | 517 ms      | Full sweep          |
| 100µF     | 1.1 s       | Multiple sweeps     |

## Testing & Troubleshooting

### Expected Behavior
- **No capacitor on probes, button pressed**: Extremely brief output pulse (< 0.1ms stray).
- **100nF on probes, button pressed**: Output LED flashes for ~1ms.
- **10µF on probes, button pressed**: Output LED stays on for ~110ms; several CD4017 LEDs step through.
- **100µF on probes, button pressed**: Output stays HIGH for ~1.1 seconds; all LEDs cycle.
- The monostable LED should always turn OFF after the timing interval (no latching).

### Common Issues and Solutions

**Output stays HIGH permanently (LED never turns off):**
- Pin 4 (RST) may be LOW or floating → connect firmly to +9V
- Capacitor on probes is too large (>1000µF gives >11 second pulse) — wait for it
- Check Pin 6/7 junction: capacitor charge path may be open (capacitor not connected)
- Short circuit in test probe wires

**Output never goes HIGH (no response to button):**
- Check trigger: Pin 2 should momentarily go below 3V when button pressed
- Measure Pin 2 at rest: should be at 9V (pull-up working)
- Verify Pin 4 = +9V (not 0V which would hold reset)
- Check that R_timing is actually connected between +9V and Pin 7 (not to GND)

**Pulse width doesn't match expected formula:**
- R_timing value may not be exactly 10kΩ — measure with multimeter
- Timing capacitor (on probes) may have higher actual value than labeled (±20% common)
- 555 timer formula assumes ideal components; real deviation is typically < 5%

**CD4017 LEDs all light at once:**
- Reset pin (Pin 15) may be held HIGH → connect to GND via 10kΩ, only pulse HIGH to reset
- Check CLOCK and CE connections

**CD4017 doesn't count:**
- Verify clock 555 is oscillating: measure Pin 3 of astable 555 — should toggle
- Check CE (Pin 13) is LOW during count window
- Confirm RESET (Pin 15) is LOW during measurement

**Measurement not repeatable (different LED each time):**
- Normal for borderline values! Electrolytic capacitors have ±20% tolerance
- Charge previous capacitor to 0V before re-testing (discharge through 10kΩ resistor)
- Ensure full trigger pulse: press button firmly and fully

## Calculations

### Pulse Width for 10µF (worked example)
```
R_timing = 10,000 Ω
C_unknown = 10 µF = 10 × 10⁻⁶ F

t = 1.1 × R × C
t = 1.1 × 10,000 × 10 × 10⁻⁶
t = 1.1 × 0.1
t = 0.11 seconds = 110 ms  ✓
```

### Solving for Unknown Capacitance (inverting the formula)
```
Measured pulse width = 55 ms
R_timing = 10kΩ (known)

C = t / (1.1 × R)
C = 0.055 / (1.1 × 10,000)
C = 0.055 / 11,000
C = 5 × 10⁻⁶ F = 5 µF
```

### Choosing R_timing for a Desired Range
```
Target: measure 1nF to 1µF
Upper end: t_max = 1.1 × R × 1µF = 100 ms (comfortable visual LED)
R = 100 ms / (1.1 × 1µF) = 0.1 / (1.1 × 10⁻⁶) = 90,909 Ω ≈ 100kΩ

Target: measure 1µF to 1000µF
Upper end: t_max = 1.1 × R × 1000µF ≤ 5s
R = 5 / (1.1 × 1000 × 10⁻⁶) = 5 / 0.0011 = 4,545 Ω ≈ 4.7kΩ
```

### LED Current
```
V_pin3 (HIGH) ≈ 8V
V_LED ≈ 2V
I = (8 – 2) / 220 = 27 mA per LED  ✓
```

## Key Concepts Learned
- **Monostable Operation**: One stable state, triggered into timed unstable state
- **RC Time Constant**: τ = R×C; the capacitor reaches 63% of VCC in one time constant
- **Indirect Measurement**: Measuring time (easy) to determine capacitance (hard to measure directly)
- **Calibration**: Using known reference values to map readings to physical quantities
- **Counter as Timer**: CD4017 converts a time interval into a count (digital representation)
- **Component Tolerances**: Real capacitors can be ±5% to ±20% from labeled value
- **Decade Counter**: Counts 0–9 and repeats; each output is HIGH for exactly one clock period

## Applications
- **Sorting unlabeled capacitors**: Identify mystery caps salvaged from old PCBs
- **Checking capacitor condition**: Electrolytic caps lose capacitance as they age
- **Quality control**: Verify capacitors are within tolerance before using in precision circuits
- **Education**: Demonstrate RC time constant in a tangible, visible way
- **Workshop tool**: Cheaper than a dedicated LCR meter for simple capacitance checks
- **Comparator**: Quickly sort a mixed pile into approximate value groups

## Next Steps
1. **Add LCD readout**: Use an Arduino to measure the monostable pulse width with `pulseIn()`, calculate C = t/(1.1×R), and display the value in nF or µF on a 16×2 LCD.
2. **Auto-ranging**: Add a multiplexer (CD4051) to switch between several R_timing values automatically for a 6-decade range (10pF – 10,000µF).
3. **ESR measurement**: Combine with an AC signal source to also measure equivalent series resistance of electrolytic caps.
4. **Frequency counter upgrade**: Replace LED array with a proper digital counter display for precise pulse-width readout.
5. **Inductance tester**: Replace the timing capacitor with a known cap and the timing resistor with the unknown inductor — same monostable principle applies to L/R circuits.

## Experimentation Ideas

### Changing the Measurement Range
By swapping R_timing, you shift the entire measurement window:

| R_timing | Best range    | Notes                             |
|----------|---------------|-----------------------------------|
| 1 MΩ     | 10pF – 100nF  | Needs oscilloscope to see pulse   |
| 100kΩ    | 100pF – 10µF  | LED display marginal              |
| 10kΩ     | 1nF – 1000µF  | **This project's sweet spot**     |
| 1kΩ      | 10µF – 100mF  | Very large electrolytic caps      |

### Dual-Range Switch
Add a rotary switch with three positions selecting R_timing = 100kΩ, 10kΩ, or 1kΩ. Label positions "small", "medium", "large". Now your tester spans nearly 7 decades of capacitance.

### Polarity Guard
Electrolytic capacitors must be inserted with correct polarity. Add a diode in series with Probe A — if the cap is reversed the diode blocks current and the circuit safely gives no reading instead of damaging the capacitor.

## Safety Notes
- ✅ 9V circuit — safe to handle
- ⚠️ **Test capacitor polarity**: Always connect electrolytic capacitor + to Probe A (higher potential)
- ⚠️ **Discharge capacitors before testing**: A pre-charged cap (especially large µF) can give incorrect readings; discharge through a 10kΩ resistor first
- ⚠️ **Do not test capacitors still in a live circuit**: Remove capacitor from board first
- ⚠️ **High-voltage caps**: Never test capacitors from mains-voltage circuits (400V+) — discharge them safely first
- ✅ The 555 timer limits current to safe levels for all common capacitor types

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

Measure the invisible! 🔬⚡
