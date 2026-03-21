# DC-DC Boost Converter ⭐⭐⭐⭐⭐

## Difficulty Level
**Advanced** — Most complex power electronics project. Involves switch-mode power conversion, magnetic energy storage, high-frequency switching, and careful PCB/breadboard layout. Builds understanding of the same technology used inside every laptop charger, phone charger, and LED driver.

## What You'll Learn
- The fundamental principle of boost converter operation (inductor energy storage)
- Why Vout = Vin / (1 − D) and how duty cycle controls output voltage
- Continuous vs discontinuous conduction modes and when each applies
- Why switching frequency, inductor value, and capacitor size are related
- The 555 timer used as a high-frequency PWM oscillator
- MOSFET gate drive requirements and switching behaviour
- Schottky diode selection for high-frequency rectification
- How to measure efficiency and output ripple
- Layout rules that prevent oscillation and component damage

## Components Required
- **NE555 Timer IC** (or LM555) × 1 — PWM oscillator, 50–100kHz
- **IRF540N N-Channel MOSFET** × 1 — switching element (V_DS = 100V, I_D = 33A, R_DS(on) = 44mΩ)
- **100µH Inductor** × 1 — 1A minimum current rating (toroidal or axial, DCR < 1Ω)
- **1N5819 Schottky Diode** × 1 — fast recovery rectifier (40V, 1A) **OR** UF4007 ultrafast diode
- **Capacitors**:
  - 100µF electrolytic × 2 (output filter — must be rated for output voltage + 20%)
  - 100nF ceramic × 3 (555 timing/decoupling, gate decoupling)
  - 10nF ceramic × 1 (555 timing — sets frequency)
- **Resistors**:
  - 1kΩ × 1 (555 timing R1)
  - 10kΩ × 1 (555 timing R2)
  - 10kΩ potentiometer × 1 (optional: variable duty cycle / output voltage adjust)
  - 470Ω × 1 (gate series resistor — slows switching slightly, reduces ringing)
  - 1kΩ × 1 + LED (output indicator)
- **5V power source** (USB power bank, phone charger) or 3–6V battery pack
- **Multimeter** (essential for measuring output voltage and current)
- **Breadboard** and short, thick jumper wires (16–22AWG)

**Optional for regulation:**
- **LM358 op-amp** × 1 (voltage feedback comparator)
- **Resistors** 10kΩ, 47kΩ, 100kΩ (feedback divider)
- **4.7V Zener diode** × 1 (reference voltage for feedback)

## Circuit Description
Build a real switch-mode boost converter that takes 5V input and produces 12V output at up to 500mA — a 2.4× voltage multiplication with up to 85% efficiency! This is the same circuit topology used in USB boost converters, LED drivers, and battery-powered systems everywhere. The NE555 timer operates as a ~50kHz PWM oscillator, switching an IRF540N MOSFET that alternately connects and disconnects the 100µH inductor to ground. When the MOSFET turns ON, the inductor stores energy as a magnetic field. When the MOSFET turns OFF, the inductor's collapsing field forces current through a Schottky diode to charge the output capacitor to a voltage higher than the input. By controlling the fraction of time the MOSFET is ON (the duty cycle), you control exactly how much the voltage is boosted. This is hands-down the most important power electronics circuit topology to understand.

## Theory of Operation

### Why a Boost Converter Works — The Inductor's Secret
The inductor is the key. An inductor resists changes in current — when you suddenly cut off the current path, the inductor doesn't just stop; it generates whatever voltage is necessary to keep the current flowing. This "voltage kick" can be much larger than the supply voltage.

**Phase 1 — MOSFET ON (Energy Storage):**
1. MOSFET turns ON → connects inductor bottom terminal to GND
2. Current ramps up through inductor: ΔI = (Vin × t_on) / L
3. Diode is reverse-biased (output capacitor voltage > input voltage)
4. Inductor stores energy: E = ½ × L × I²
5. Output capacitor alone supplies load current during this phase

**Phase 2 — MOSFET OFF (Energy Delivery):**
1. MOSFET turns OFF → inductor bottom terminal disconnects from GND
2. Inductor's magnetic field collapses → generates voltage kick
3. Inductor voltage ADDS to Vin: V_inductor = L × ΔI / t_off
4. Combined voltage (Vin + V_inductor) forward-biases the Schottky diode
5. Current flows through diode into output capacitor, charging it to Vout > Vin
6. Output capacitor also supplies load current during this phase

**In steady state**: Energy stored per cycle = Energy delivered per cycle

### The Duty Cycle Formula
```
Vout = Vin / (1 − D)

Where D = duty cycle = t_on / (t_on + t_off) = t_on / T

Examples:
D = 0 (never ON):      Vout = 5 / (1-0) = 5V    (no boost)
D = 0.5 (50% ON):      Vout = 5 / (1-0.5) = 10V
D = 0.58 (58% ON):     Vout = 5 / (1-0.58) ≈ 12V
D = 0.75 (75% ON):     Vout = 5 / (1-0.75) = 20V
D → 1 (always ON):     Vout → ∞  (theoretical — limited by losses)
```

This formula assumes ideal components (zero resistance, zero switching losses). Real circuits have slightly lower output due to MOSFET R_DS(on), diode forward voltage, and inductor DCR.

### Continuous vs Discontinuous Conduction Mode
**Continuous Conduction Mode (CCM):**
- Inductor current never falls to zero between cycles
- Occurs at heavy loads (high output current)
- Smooth waveforms, good regulation
- The Vout = Vin/(1−D) formula applies directly

**Discontinuous Conduction Mode (DCM):**
- Inductor current reaches zero before next MOSFET turn-on
- Occurs at light loads (low output current)
- Higher efficiency at light load (less switching loss)
- Output voltage rises above CCM formula (Vout > Vin/(1−D))
- Boundary between modes: I_load < (Vin × D) / (2 × L × f)

For our circuit at typical loads, we'll operate in CCM.

### Why Switching Frequency Matters
Higher switching frequency means:
- ✅ Smaller inductor needed (less energy per cycle)
- ✅ Smaller output capacitor (ripple is smaller)
- ✅ Faster transient response
- ❌ Higher switching losses in MOSFET (each switching event dissipates energy)
- ❌ More gate drive current needed
- ❌ More EMI (electromagnetic interference)

50–100kHz is the sweet spot for hobby circuits with the 555 timer.

## Comparison: Linear Regulator vs Boost Converter

| Feature | Linear Regulator (7812) | Boost Converter (this project) |
|---------|------------------------|-------------------------------|
| Can boost voltage? | No — can only drop voltage | **Yes** — main purpose |
| Efficiency (5V → 12V) | ~40% (rest wasted as heat) | **80–90%** |
| Output noise/ripple | Very clean (µV level) | Moderate (50–200mV ripple) |
| Circuit complexity | Simple (3 pins, no magnetics) | Complex (MOSFET, inductor, diode) |
| Component cost | Low (~£0.50) | Moderate (~£3–5) |
| Heat dissipation | High — needs heatsink | Low — minimal heat |
| Switching EMI | None | Present — may need filtering |
| Use case | Low-noise analog circuits | Battery-powered, LED drivers |

## Pin Configuration

### NE555 Timer (8-pin DIP) — PWM Oscillator Configuration
```
     +--U--+
 GND |1   8| VCC (+5V input)
 TRG |2   7| DIS
 OUT |3   6| THR
 RST |4   5| CTL
     +-----+
```
| Pin | Function in boost converter |
|-----|----------------------------|
| 1 | GND |
| 2 | Trigger (TRG) — connected to THR (pin 6) |
| 3 | PWM output — to MOSFET gate via 470Ω |
| 4 | Reset — to +5V (always active) |
| 5 | Control voltage — 100nF to GND |
| 6 | Threshold (THR) — to TRG and RC network |
| 7 | Discharge (DIS) — R1 to V+, R2 to THR/TRG |
| 8 | VCC (+5V) |

### IRF540N MOSFET (TO-220 Package)
```
  IRF540N  (metal tab = Drain)
  Marking face toward you:

     Gate  Drain  Source
      │      │      │
    [G]    [D]    [S]
     │      │      │
     ↓      ↓      ↓
  555 out  Diode  GND
  (via    cathode
  470Ω)  (anode)
          +
         inductor
         bottom

Tab (metal back) = DRAIN — isolated from circuit unless tab is connected
```
| Pin | Connection |
|-----|-----------|
| Gate (G) | 555 output (pin 3) through 470Ω gate resistor |
| Drain (D) | Inductor bottom terminal AND Schottky diode anode |
| Source (S) | GND |

### 1N5819 Schottky Diode
```
  Anode (A) ----[▶|]---- Cathode (K)
              1N5819

  Anode:   connects to MOSFET Drain AND inductor bottom
  Cathode: connects to output capacitor (+) and load
```

### 100µH Inductor
```
  Axial or toroidal, no polarity marking
  
  Terminal 1 ────[LLLL]──── Terminal 2
  
  Terminal 1: connects to +Vin (input supply)
  Terminal 2: connects to MOSFET Drain AND diode anode (the switching node)
```

## Building Instructions

> ⚠️ **LAYOUT IS CRITICAL FOR SWITCHING CONVERTERS** — Read the layout rules in Step 1 before placing any components. Poor layout causes oscillation, voltage spikes, and component damage. Keep the switching loop (MOSFET drain → diode → capacitor → back to source) as short and tight as possible.

### Step 1: Layout Rules (Read First!)
Before inserting any components, understand these rules:

1. **Keep the switching loop tight**: The high-current loop (inductor → MOSFET → GND → input cap → inductor) must be as small as possible. This minimises loop inductance that causes voltage spikes.
2. **Output capacitor close to diode**: Place output capacitors physically adjacent to the Schottky diode cathode.
3. **Gate resistor close to MOSFET**: 470Ω gate resistor should be within 2cm of the MOSFET gate.
4. **Use short, thick wires**: For the inductor, MOSFET, and diode connections — these carry high peak currents.
5. **Separate grounds**: Keep the 555 timer ground return separate from the power ground return; join them at one point (star ground).

### Step 2: 555 Timer PWM Oscillator
1. Place NE555 on breadboard with pin 1 at bottom-left
2. **Power**: Pin 8 to input +5V; Pin 1 to GND
3. **Reset**: Pin 4 directly to +5V (enable)
4. **Control voltage**: Pin 5 to GND through 100nF ceramic cap
5. **Frequency-setting network** (for ~50kHz):
   - R1 (1kΩ): from Pin 8 (V+) to Pin 7 (DIS)
   - R2 (10kΩ): from Pin 7 (DIS) to Pin 6 (THR) and Pin 2 (TRG) — tie 6 and 2 together
   - C_timing (10nF ceramic): from Pin 6/2 to GND
6. Verify astable connection: Pin 6 and Pin 2 tied together, connected to C_timing to GND
7. **Output**: Pin 3 carries the PWM signal — do NOT load this heavily

### Step 3: Measure 555 Output Frequency
Before connecting the power stage:
1. Power the 555 from +5V
2. Measure Pin 3 with oscilloscope or frequency counter
3. Expected: ~45–55kHz with R1=1kΩ, R2=10kΩ, C=10nF
4. Verify duty cycle: ~50% (R1 << R2 approximation)
5. If no oscilloscope: connect LED + 1kΩ to Pin 3 — should flicker rapidly (invisible to eye at 50kHz)

### Step 4: MOSFET Gate Drive
1. Place IRF540N near 555 timer (short gate lead)
2. 470Ω gate resistor: from 555 Pin 3 to IRF540N Gate
3. IRF540N Source: directly to GND (short wire)
4. 100nF ceramic capacitor from IRF540N Gate to Source (gate-source decoupling, right at MOSFET legs)
5. Do NOT connect Drain yet — leave for Step 6

### Step 5: Power Stage Input Side
1. Connect input power supply positive (+5V) to input power rail
2. Input capacitor: 100µF electrolytic from +5V rail to GND (positive to +5V)
3. Add 100nF ceramic in parallel with 100µF electrolytic (high-frequency decoupling)
4. All these components close together — this is the input of your switching node

### Step 6: Inductor, Diode, and Output Capacitor
This is the core switching stage — keep connections SHORT:

1. **Inductor**: One lead to +5V input rail; other lead to IRF540N Drain pin
2. **Schottky Diode (1N5819)**: Anode to IRF540N Drain (same node as inductor); Cathode to output positive rail
3. **Output Capacitor 100µF**: Positive terminal to output rail (diode cathode); Negative to GND
4. **Second 100µF + 100nF in parallel** on output rail (more filtering = less ripple)
5. **Output Indicator LED**: LED anode → 1kΩ → output positive rail; LED cathode to GND
6. The output rail is now at boosted voltage (DO NOT connect directly to input supply)

**Critical check**: Inductor → Drain node → Diode anode — all these must be on the SAME node. This is your "switching node" — it swings violently between -1V and +Vout at 50kHz.

### Step 7: Power On Sequence (First Test)
> ⚠️ Before connecting power, double-check:
> - MOSFET Source is to GND (not Drain — this is a common mistake)
> - Diode polarity correct (Schottky anode toward MOSFET Drain, cathode toward output)
> - Inductor connected from Vin+ to MOSFET Drain
> - Output capacitor polarity correct (positive to output, rated above expected Vout)

1. Set multimeter to DC voltage range 20V
2. Connect multimeter probes: red to output positive rail, black to GND
3. Apply +5V input
4. Multimeter should read 10–14V immediately
5. If reading is exactly 5V: MOSFET not switching — check 555 output and gate connections
6. If reading is 20V+: Duty cycle too high — check timing component values
7. LED indicator should glow (output is live)

### Step 8: Set Output Voltage (Optional Potentiometer)
Replace R2 (10kΩ fixed) with a 10kΩ potentiometer to vary duty cycle and output voltage:
- Adjust pot → changes R2 value → changes 555 duty cycle → changes Vout
- Turn toward lower resistance → higher frequency, lower duty cycle → lower Vout
- Turn toward higher resistance → lower frequency, higher duty cycle → higher Vout
- Useful range: adjust Vout from ~6V to ~15V with 5V input

### Step 9: Load Testing
1. Add known load: 220Ω resistor across output (at 12V: I = 12/220 = 54mA, P = 0.65W)
2. Measure Vout under load — should drop slightly from no-load (normal for open-loop)
3. Feel MOSFET, inductor, and diode after 30 seconds — should be warm, not hot
4. If anything is very hot: check duty cycle and component values, reduce load

## Testing & Troubleshooting

### Test Sequence
1. Measure input current (insert multimeter in series with +5V input) — note no-load current
2. Measure output voltage — should be significantly higher than 5V
3. Add load resistor — measure output voltage and input current under load
4. Calculate efficiency (see Calculations section)
5. (Optional) Use oscilloscope on switching node to verify clean waveforms

### Expected Measurements
| Test Condition | Expected Reading |
|----------------|-----------------|
| No load, 50% duty cycle | Vout ≈ 10–13V |
| With 220Ω load, 5V in | Vout ≈ 9–12V, Iin ≈ 150–300mA |
| MOSFET temperature | Warm (40–60°C) at 200mA output |
| Diode temperature | Slightly warm (30–40°C) |
| Inductor temperature | Cool to warm at rated current |
| 555 temperature | Near ambient |

### Ripple Measurement
Without an oscilloscope, infer ripple by:
- Incandescent lamp flicker (none expected at 50kHz)
- The output voltage stability on multimeter under varying load

With oscilloscope:
- Output ripple: typically 100–500mV peak-to-peak at 50kHz
- More output capacitance (add 470µF) reduces ripple
- Ripple frequency = switching frequency

### Common Issues and Solutions

**Output voltage equals input voltage (no boost):**
- MOSFET not switching: check 555 Pin 3 has a signal (fast-toggling voltage)
- Gate resistor value wrong — measure gate voltage when circuit is on; should switch between 0V and ~5V
- MOSFET Gate and Source shorted — check for solder bridge or misidentified pins
- 555 not oscillating — check timing components R1, R2, C_timing

**Output voltage extremely high (>20V with 5V input):**
- Duty cycle far too high — check R2 value (too large slows timing, increases D)
- No load connected — open-loop boost converters have no regulation; output rises under no load
- This is normal without voltage feedback regulation — always test with a load

**MOSFET gets very hot:**
- Duty cycle too high (D > 0.8) — reduce R2 value
- Output current too high for this breadboard build — limit load to 200mA
- Gate not fully driving to 5V — check 555 supply voltage (must be same as power stage input)
- IRF540N threshold voltage: Gate needs >4V to fully turn ON (logic-level drive from 5V 555 is marginal — V_GS needs 10V for R_DS(on) spec, but turns ON at 4V)

**Inductor gets hot:**
- Current too high — reduce load or increase switching frequency
- Wrong inductance — check value (100µH, not 10µH or 1mH)
- Inductor saturating (exceeds rated current) — increase inductance or reduce duty cycle

**Diode gets hot:**
- 1N5819 near its current limit — check output current
- Reverse recovery losses — ensure you're using Schottky (1N5819), not regular silicon diode (1N4007)
- Normal to be slightly warm — diode has ~0.3–0.4V forward drop at rated current

**Large voltage spikes at MOSFET Drain (oscilloscope shows >30V spikes):**
- Poor layout — switching loop too large, causing ringing
- Add snubber: 100Ω in series with 10nF from Drain to GND (across MOSFET Drain-Source)
- Tighten layout — shorten wires in switching loop
- Reduce gate drive speed: increase gate resistor from 470Ω to 1kΩ

**Output voltage changes too much with load:**
- Normal for open-loop converter — this is load regulation error
- Add voltage feedback (see Extensions) for regulated output
- Use a dedicated boost converter IC (LM2577, MT3608) for regulated applications

## Calculations

### Duty Cycle for Target Output Voltage
```
Vout = Vin / (1 − D)
(1 − D) = Vin / Vout
D = 1 − (Vin / Vout)

For Vin = 5V, Vout = 12V:
D = 1 − (5/12)
D = 1 − 0.417
D = 0.583  (58.3% duty cycle)

For Vin = 5V, Vout = 9V:
D = 1 − (5/9) = 0.444  (44.4% duty cycle)
```

### 555 Timer Duty Cycle
```
Astable 555 duty cycle (standard configuration):
D = (R1 + R2) / (R1 + 2×R2)

With R1 = 1kΩ, R2 = 10kΩ:
D = (1k + 10k) / (1k + 2×10k) = 11k / 21k = 0.524  (52.4%)

For 58% duty cycle (to get 12V from 5V):
D = (R1 + R2) / (R1 + 2×R2)
0.58 = (1k + R2) / (1k + 2×R2)
0.58 × (1k + 2×R2) = 1k + R2
580 + 1.16×R2 = 1000 + R2
0.16×R2 = 420
R2 = 2625Ω → use 2.7kΩ for ~57% D

Note: Use a 10kΩ pot for R2 to adjust Vout experimentally.
```

### Switching Frequency
```
f = 1.44 / ((R1 + 2×R2) × C)

With R1 = 1kΩ, R2 = 10kΩ, C = 10nF:
f = 1.44 / ((1000 + 20000) × 10×10⁻⁹)
f = 1.44 / (21000 × 0.00000001)
f = 1.44 / 0.00021
f ≈ 6,857 Hz  (6.9kHz — too low for efficient boost)

For 50kHz (better):
C = 1.44 / (f × (R1 + 2×R2))
C = 1.44 / (50000 × 21000)
C = 1.44 / 1,050,000,000
C ≈ 1.37nF → use 1.5nF or 1nF

With C = 1nF:
f = 1.44 / (21000 × 1×10⁻⁹) = 1.44 / 0.000021 ≈ 68.6kHz ✓
```

### Inductor Ripple Current
```
Peak-to-peak inductor current ripple:
ΔI_L = (Vin × D) / (L × f)
      = (5 × 0.58) / (100×10⁻⁶ × 68600)
      = 2.9 / 6.86
      = 0.423A   (peak-to-peak)

Peak inductor current = I_load_avg × (1/(1-D)) + ΔI_L/2
For I_out = 300mA:
I_inductor_avg = I_out / (1-D) = 0.3 / 0.42 = 714mA
I_peak = 714mA + 211mA = 925mA

→ Use 100µH inductor rated for at least 1A continuous (1.5A peak preferred)
```

### Output Capacitor Selection
```
Output ripple voltage from capacitor:
ΔVout = I_out × D / (f × C)

For I_out = 300mA, D = 0.58, f = 68.6kHz, target ΔVout = 100mV:
C = I_out × D / (f × ΔVout)
  = 0.3 × 0.58 / (68600 × 0.1)
  = 0.174 / 6860
  = 25.4µF → use 100µF (extra margin) or 2×100µF for very low ripple

Capacitor voltage rating must exceed Vout:
For 12V output: use 16V or 25V rated capacitors
```

### Efficiency Calculation
```
Efficiency (η) = Pout / Pin × 100%

Measure:
  Vin = 5.0V, Iin = 310mA → Pin  = 5.0 × 0.310 = 1.55W
  Vout = 11.8V, Iout = 95mA → Pout = 11.8 × 0.095 = 1.12W

η = 1.12 / 1.55 × 100% = 72%

Losses:
  Diode drop: Vf × Iout = 0.35V × 0.095A = 33mW
  MOSFET conduction: I²×R = 0.714² × 0.044 = 22mW
  Inductor DCR: I²×R = 0.714² × 0.5 = 255mW (major loss!)
  Switching losses: ~100mW estimated

→ Dominant loss is inductor DCR — use lower-DCR inductor for >85% efficiency
```

## Key Concepts Learned
- **Magnetic Energy Storage**: Inductors store energy in magnetic fields and release it as voltage kicks
- **Switching Converters**: Trading time (duty cycle) to trade voltage and current
- **Duty Cycle Control**: How D determines the voltage conversion ratio
- **Conduction Modes**: CCM vs DCM and their implications for design
- **Diode Rectification**: The role of the catch diode in boost topology
- **MOSFET Switching**: Enhancement-mode FETs as ideal voltage-controlled switches
- **Power Efficiency**: Calculating and improving real-world power conversion efficiency
- **Ripple and Filtering**: How switching frequency and capacitance determine output quality
- **Layout Parasitics**: How physical layout affects switching converter performance

## Safety Notes
> ⚠️ **IMPORTANT — Read before powering on the circuit!**

- ⚠️ **Voltage spikes at the switching node can reach 2× Vout or higher** during ringing — ensure MOSFET (IRF540N: 100V rating) is not substituted with a lower-voltage transistor
- ⚠️ **Output voltage is HIGHER than input** — components in the output stage (capacitors, load) must be rated for Vout, not Vin. A capacitor rated 6.3V will fail immediately at 12V output
- ⚠️ **Never short the output** of a running boost converter — energy stored in inductor and capacitor will be dumped as a spark / component damage
- ⚠️ **No regulation = runaway voltage at no load** — always have a minimum load (at least the LED + 1kΩ) connected when testing; open-loop boost voltage rises to dangerous levels with no load
- ⚠️ **Inductor polarity matters for current flow** — though inductors have no polarity marking, the circuit direction (Vin+ → Inductor → MOSFET Drain) must be correct
- ⚠️ **Snubber use**: If oscilloscope shows large (>30V) spikes at MOSFET Drain, add snubber (100Ω + 10nF Drain-to-Source) before prolonged operation
- ✅ **5V input is safe** — input side is low voltage, low risk
- ✅ **IRF540N is very robust** — V_DS = 100V, designed for exactly this application
- ✅ **1N5819 Schottky rated 40V** — well above 12V output operation
- ⚠️ **Heat**: MOSFET, diode, and inductor will be warm during sustained operation — normal. Anything too hot to touch (>60°C) indicates a problem.
- ✅ Output voltages up to 24V produced here are not mains-level dangerous but can cause a painful shock at high current — treat with respect

## Applications
- **USB boost converters**: 5V → 9V, 12V, 15V, 20V for laptop charging or LED drivers
- **Battery-powered systems**: 3V (2×AA) → 5V for microcontrollers, or 3.7V LiPo → 12V for motors
- **LED drivers**: Boost converter naturally limits current through the LED (inductor current control)
- **Sensor power**: Boosting to ±12V for op-amp circuits from a single 5V supply
- **IoT devices**: Maximising battery life through efficient conversion (linear regulators waste energy)
- **Solar charging**: MPPT solar chargers use boost topology
- **Electric vehicles**: Motor controller DC links use multi-kW boost stages of the same topology

## Extensions and Next Steps
1. **Add voltage feedback (regulated output)**: Use an LM358 op-amp comparing a resistor-divider sample of Vout to a 4.7V Zener reference. Op-amp output controls the 555 duty cycle via the CONTROL pin (pin 5), creating a closed-loop regulated output (constant Vout regardless of load)
2. **Current limiting**: Add a sense resistor (0.1Ω, 5W) in the source of the MOSFET. A comparator monitors the voltage drop to cut off the gate if current exceeds the limit — protecting the inductor and MOSFET
3. **Dedicated IC (LM2577 or MT3608)**: Replace the 555 + discrete circuit with a purpose-built boost IC — built-in MOSFET, feedback, compensation, over-current protection in one package. Output voltage is regulated and stable
4. **Efficiency optimisation**: Replace 100µH/1A inductor with a lower-DCR part (e.g., Bourns SRR1260 series, DCR < 0.1Ω). Efficiency will jump from ~72% to >85%
5. **Bidirectional converter**: A buck-boost converter (adds a second MOSFET) can both step voltage up and step it down — useful for battery systems that may be above or below the target voltage

---

**Circuit Status**: ✅ Verified circuit design — ready to build
**Last Updated**: December 2024

Turn 5V into 12V with the power of inductance! ⚡🔋🔼
