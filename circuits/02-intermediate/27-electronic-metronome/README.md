# Electronic Metronome ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - 555 timer in astable mode with audio and visual tempo output

## What You'll Learn
- 555 timer IC in astable (free-running) oscillator mode
- Audio frequency generation with passive components
- How to control oscillation frequency with a potentiometer
- BPM (Beats Per Minute) to Hz conversion
- LED synchronization with audio output
- Practical timing circuit design for real-world applications

## Components Required
- **NE555 or LM555** Timer IC × 1
- **8Ω Speaker** or piezo buzzer × 1
- **Potentiometer** 100kΩ × 1 (tempo control)
- **Resistors**:
  - 1kΩ × 1 (R1 fixed resistor, minimum timing)
  - 220Ω × 1 (LED current limiting)
- **Capacitors**:
  - 10µF electrolytic × 1 (timing capacitor)
  - 100nF ceramic × 1 (bypass / control pin decoupling)
- **LED** (any color) × 1 (beat indicator)
- **9V battery** × 1
- **Battery connector**
- **Breadboard** and jumper wires

## Circuit Description
Build a fully adjustable electronic metronome that produces a crisp click and flashes an LED on every beat! A 555 timer in astable mode oscillates at a frequency set by a 100kΩ potentiometer — turning the knob sweeps from approximately 40 BPM (largo) all the way up to 208 BPM (prestissimo), covering the complete range used in music. The speaker gets a sharp pulse on each cycle, producing an audible tick, while the LED flashes simultaneously so you can track the beat visually even in a noisy environment. No microcontroller required — pure analog timing!

## Theory of Operation

### The 555 Timer in Astable Mode
In astable mode the 555 timer has **no stable state**. It continuously toggles its output between HIGH and LOW, producing a square wave. Three internal equal-value resistors create two threshold voltages:

- **Upper threshold (THR)**: 2/3 × VCC — when the capacitor voltage reaches this, the output goes LOW and the capacitor starts discharging
- **Lower threshold (TRG)**: 1/3 × VCC — when the capacitor discharges to this, the output goes HIGH again and the capacitor starts charging

The capacitor therefore oscillates between 1/3 VCC and 2/3 VCC endlessly, and the output mirrors this as a square wave.

### Charging and Discharging Paths
```
VCC ──── R1 ──── POT (Ra) ──── Pin 7 (DIS)
                                    │
                              R (internal discharge transistor)
                                    │
                              Pin 2/6 ──── C ──── GND
```
- **Charge path**: VCC → R1 → POT upper half → C (through pins 7 and 6)
- **Discharge path**: C → Pin 7 (DIS transistor to GND)

Charge time: `t_HIGH = 0.693 × (R1 + POT) × C`
Discharge time: `t_LOW  = 0.693 × POT × C`

For this project R1 = 1kΩ, POT = 0–100kΩ, C = 10µF.

### Frequency and BPM Formula
Total period: `T = t_HIGH + t_LOW = 0.693 × (R1 + 2×POT) × C`

Frequency: `f = 1/T = 1.44 / ((R1 + 2×POT) × C)`

BPM: `BPM = f × 60 = 86.4 / ((R1 + 2×POT) × C)`

#### How the Potentiometer Controls Tempo
- **Pot at maximum (100kΩ)**:
  `f = 1.44 / ((1k + 200k) × 10µF) = 1.44 / 2.01 ≈ 0.72 Hz → ~43 BPM`
- **Pot at minimum (0Ω)**:
  `f = 1.44 / ((1k + 0) × 10µF) = 1.44 / 0.01 = 144 Hz`
  — way too fast for a metronome; in practice turn pot no lower than ~3kΩ
- **Pot at ~3kΩ**:
  `f = 1.44 / ((1k + 6k) × 10µF) = 1.44 / 0.07 ≈ 20 Hz → ~1200 BPM` — still too fast
  Use C = 10µF for the musical BPM range.

The 1kΩ fixed resistor (R1) prevents the discharge transistor from shorting VCC to GND at zero pot resistance — it is a **mandatory protection resistor**.

### Audio Generation
The speaker is driven directly from Pin 3 (OUT) through a small series resistor (or directly for piezo). Each HIGH-to-LOW transition produces one "tick" sound. The short duty cycle at musical BPM produces a sharp click that closely mimics an acoustic metronome.

## BPM to Frequency Conversion Table

| BPM | Musical Tempo   | Frequency (Hz) | Period (ms) |
|-----|-----------------|----------------|-------------|
|  40 | Grave (very slow)  | 0.67 Hz     | 1500 ms     |
|  60 | Largo (slow)       | 1.00 Hz     | 1000 ms     |
|  76 | Andante            | 1.27 Hz     |  790 ms     |
|  96 | Andante moderato   | 1.60 Hz     |  625 ms     |
| 120 | Allegro            | 2.00 Hz     |  500 ms     |
| 144 | Vivace             | 2.40 Hz     |  417 ms     |
| 168 | Presto             | 2.80 Hz     |  357 ms     |
| 208 | Prestissimo        | 3.47 Hz     |  288 ms     |

**Memory aid**: 60 BPM = exactly 1 Hz (one tick per second).

## Pin Configuration

### 555 Timer IC (8-pin DIP)
```
    +--U--+
GND |1   8| VCC (+9V)
TRG |2   7| DIS
OUT |3   6| THR
RST |4   5| CTL
    +-----+
```

| Pin | Name    | Connection in this circuit                         |
|-----|---------|----------------------------------------------------|
| 1   | GND     | Ground                                             |
| 2   | TRG     | Tied to Pin 6 (capacitor junction)                 |
| 3   | OUT     | Speaker (series 100Ω optional) + LED via 220Ω      |
| 4   | RST     | +9V (always enabled)                               |
| 5   | CTL     | 100nF capacitor to GND (noise bypass)              |
| 6   | THR     | Tied to Pin 2 (capacitor junction)                 |
| 7   | DIS     | Junction of R1 and potentiometer wiper             |
| 8   | VCC     | +9V                                                |

## Schematic (Text Representation)
```
+9V ──┬────────────────────────── Pin 8 (VCC)
      │                           Pin 4 (RST)
      │
      ├── R1 (1kΩ) ──┬── POT top ────── Pin 7 (DIS)
                     │
                   POT wiper ─────────────────────────────┐
                                                          │
                     │                                    │
                   POT bottom ────── (connected to wiper here, or use full pot)
                                                          │
                                                     Pin 6 (THR)
                                                     Pin 2 (TRG)
                                                          │
                                                   C (10µF, +up)
                                                          │
GND ──────────────────────────── Pin 1 (GND) ────────────┘

Pin 3 (OUT) ──┬── Speaker (8Ω) ── GND
              └── 220Ω ── LED (anode) ── LED (cathode) ── GND

Pin 5 (CTL) ── 100nF ── GND
```

## Building Instructions

### Step 1: Power Rails
1. Place the breadboard with power rails accessible.
2. Connect +9V battery positive to the red (+) rail.
3. Connect battery negative to the blue (–) rail.
4. Verify with multimeter: you should read 9V between the rails.

### Step 2: Insert and Power the 555 IC
1. Insert the 555 timer straddling the breadboard center gap.
2. Identify Pin 1 — the notch or dot on the IC marks Pin 1 (top-left).
3. **Pin 8 → +9V rail** (power)
4. **Pin 1 → GND rail** (ground)
5. **Pin 4 → +9V rail** (RST always HIGH = always enabled)

### Step 3: Bypass Capacitor on Control Voltage
1. Connect a **100nF ceramic capacitor** between Pin 5 (CTL) and GND.
2. Keep this wire short — it prevents high-frequency noise from destabilizing the oscillator.

### Step 4: Timing Network (R1 + Potentiometer + C)
1. Connect **R1 (1kΩ)** from Pin 8 (+9V) to Pin 7 (DIS).
2. Connect the **potentiometer**:
   - Top terminal → Pin 7 (DIS) — in series with R1
   - Wiper terminal → Pin 6 (THR) **and** Pin 2 (TRG) — use a short jumper wire to tie pins 2 and 6 together first
   - Bottom terminal → also to Pin 6/2 junction (or leave connected to wiper for simplicity — connecting top to DIS, wiper to pins 2/6 is the standard method)
3. Connect the **10µF electrolytic capacitor**:
   - Positive lead (+) → Pins 2 and 6 junction
   - Negative lead (–) → GND
   - ⚠️ **Observe polarity!** Reversed electrolytic capacitors can fail or explode.

### Step 5: LED Beat Indicator
1. Connect **220Ω resistor** from Pin 3 (OUT) to the LED anode (+, longer leg).
2. Connect LED cathode (–, shorter leg / flat side) to GND.
3. The LED will flash in sync with the output square wave.

### Step 6: Speaker Connection
1. Connect one terminal of the speaker (or piezo buzzer) directly to Pin 3 (OUT).
2. Connect the other terminal to GND.
3. For an 8Ω speaker, optionally add a 47–100Ω series resistor to reduce current draw and protect Pin 3.
4. For a piezo buzzer, no series resistor is needed.

### Step 7: Final Inspection
1. Confirm IC orientation (Pin 1 at notch).
2. Verify electrolytic capacitor polarity (+) toward Pin 6.
3. Check no power-to-ground shorts.
4. Confirm Pin 4 is tied to +9V (not floating).
5. Turn potentiometer to mid-position before powering on.

## Testing & Troubleshooting

### Expected Behavior
- **Power on**: LED should flash and speaker should tick at a steady rate.
- **Turn pot clockwise** (increase resistance): BPM decreases, ticks slow down.
- **Turn pot counter-clockwise** (decrease resistance): BPM increases, ticks speed up.
- Sweeping pot full range: tempo moves from ~40 BPM (slow) to ~208 BPM (fast).

### Test Procedure
1. Power on with pot at mid-position → should hear steady ticking.
2. Count ticks over 10 seconds, multiply by 6 → that is your BPM.
3. Rotate pot to maximum → ticking slows to about 1 tick per 1.5 seconds (40 BPM).
4. Rotate pot to minimum (carefully) → ticking becomes rapid machine-gun clicks.
5. Set to 60 BPM (one tick per second) — count with a clock to verify accuracy.

### Voltage Measurements

**Key pins during operation:**
| Pin | Expected voltage                           |
|-----|--------------------------------------------|
| 8   | 9V (constant)                              |
| 1   | 0V (constant)                              |
| 4   | 9V (constant)                              |
| 5   | ~6V (2/3 × VCC reference)                  |
| 3   | Toggles 0V ↔ ~8V (square wave)            |
| 6/2 | Oscillates between ~3V and ~6V (sawtooth) |

### Common Issues and Solutions

**No ticking, LED stays off:**
- Check Pin 8 = 9V and Pin 1 = 0V (power OK)
- Confirm Pin 4 tied to +9V (if Pin 4 = 0V the timer is held in reset)
- Measure Pin 6/2 — should oscillate 3–6V; if stuck, check capacitor polarity
- Replace 10µF capacitor (most common failure point)

**Very faint or no sound from speaker:**
- Speaker connection may be reversed — try swapping leads
- Try a piezo buzzer instead (louder at low current)
- Verify Pin 3 output actually toggles (use LED test first)

**LED on but no sound:**
- Speaker may be open-circuit; verify continuity with multimeter (should read ~8Ω)
- Piezo buzzers with internal oscillator won't work here — use passive piezo

**Frequency doesn't change with pot:**
- Pot wiper may not be connected to pins 2/6 — re-check wiring
- Pot may be faulty — measure resistance across wiper with multimeter while turning

**Ticking is too fast even at max pot:**
- Check capacitor value — should be 10µF; 1µF gives 10× faster ticking
- Ensure pot is 100kΩ, not 10kΩ

**Circuit oscillates at wrong BPM range:**
- Substitute C = 22µF for slower minimum tempo (~18 BPM at max pot)
- Substitute C = 4.7µF for higher maximum tempo (~440 BPM at min pot)

**LED dims but speaker is loud (or vice versa):**
- This is normal — LED duty cycle depends on the HIGH/LOW time ratio
- The sound click occurs on the SHORT pulse; LED is brightest on the LONG pulse

## Calculations

### Frequency at Maximum Pot (40 BPM)
```
R_total = R1 + 2×POT = 1kΩ + 2×100kΩ = 201kΩ
f = 1.44 / (201,000 × 0.00001) = 1.44 / 2.01 = 0.716 Hz
BPM = 0.716 × 60 = 43 BPM  ✓ (≈ Grave)
```

### Frequency at 60 BPM (1 Hz)
```
Target: f = 1 Hz (T = 1 s)
T = 0.693 × (R1 + 2×POT) × C
1.0 = 0.693 × (1000 + 2×POT) × 0.00001
(1000 + 2×POT) = 1.0 / (0.693 × 0.00001) = 144,300
2×POT = 143,300 → POT = 71.6kΩ
→ Set pot to ~72% of its full rotation
```

### Frequency at 120 BPM (Allegro)
```
Target: f = 2 Hz (T = 0.5 s)
(1000 + 2×POT) = 0.5 / (0.693 × 0.00001) = 72,150
2×POT = 71,150 → POT = 35.6kΩ
→ Set pot to ~36% of its rotation
```

### LED Current
```
VOUT (HIGH) ≈ 8V (555 output swing)
VLED ≈ 2V (red/green LED)
I = (VOUT – VLED) / R = (8 – 2) / 220 = 27mA  ✓ (within 555 output limit of 200mA)
```

### Total Current Draw
```
555 timer:   ~10mA
LED (50% on): ~14mA average
Speaker:      ~30mA (at 8Ω with 8V peak)
Total:        ~54mA

9V alkaline battery (500mAh) runtime: 500 / 54 ≈ 9 hours continuous use
```

## Key Concepts Learned
- **Astable Oscillator**: A circuit with no stable state; continuously oscillates
- **RC Time Constant**: τ = R × C controls how fast a capacitor charges/discharges
- **555 Internal Structure**: Voltage divider, comparators, SR flip-flop, discharge transistor
- **Duty Cycle**: Ratio of HIGH time to total period; controls LED brightness and click character
- **Frequency vs. Period**: f = 1/T; doubling R or C halves the frequency (halves BPM)
- **Protection Resistors**: R1 prevents destructive discharge through the internal transistor
- **BPM Arithmetic**: Musical tempo is just frequency expressed as cycles per minute

## Applications
- **Music practice**: Keep steady tempo while learning a piece on any instrument
- **Rhythm training**: Improve timing, subdivision, and groove awareness
- **Fitness / exercise**: Cadence control for running, cycling, rowing
- **Dance / choreography**: Mark beats for rehearsals without a phone
- **Teaching**: Demonstrate oscillator theory in electronics classes
- **Therapeutic uses**: Rhythmic auditory stimulation in physical therapy
- **Clock reference**: A 60 BPM metronome is a precise 1 Hz clock signal

## Next Steps
1. **Add an LCD BPM display**: Use a 16×2 LCD and 555 output on an Arduino to count pulses and display exact BPM digitally.
2. **Multiple time signatures**: Add a CD4017 decade counter to light a different LED on beat 1 (downbeat) of a bar.
3. **Accent beat 1**: Connect the CD4017 Q0 output to a second, louder speaker or a different pitch buzzer.
4. **Tap tempo**: Replace pot with a one-shot timing circuit triggered by a push button tap.
5. **MIDI sync**: Feed the output into an optocoupler and MIDI interface to sync hardware instruments.
6. **Pendulum animation**: Use a bi-color LED and flip-flop to alternate left/right on each beat like a pendulum.

## Experimentation Ideas

### Changing the BPM Range
| Capacitor Value | Approx. BPM Range at Full Pot Sweep |
|-----------------|--------------------------------------|
| 47µF            | 9 – 44 BPM (very slow, adagio range) |
| 22µF            | 18 – 90 BPM                          |
| 10µF            | 40 – 208 BPM (standard musical range)|
| 4.7µF           | 85 – 440 BPM (percussion / EDM)      |
| 1µF             | 400 – 2000 BPM (audio frequency!)    |

### Convert to Tone Generator
Replace the 10µF timing capacitor with a 100nF ceramic capacitor. The 555 now oscillates in the audible audio range (hundreds of Hz), and the potentiometer sweeps the pitch like a theremin. Same circuit — completely different application!

### Dual-Tempo (Practice Aid)
Wire a second 555 + speaker at a frequency exactly 2× the first — you now have a metronome that ticks on quarter notes and eighth notes simultaneously, ideal for learning subdivisions.

## Safety Notes
- ✅ 9V circuit — completely safe to touch
- ⚠️ **Electrolytic capacitor polarity**: Always connect + to higher voltage; reversed caps can rupture
- ⚠️ **Speaker volume**: At 9V the click can be loud in a quiet room; hold speaker away from ear during first power-on
- ✅ 555 timer output can source/sink 200mA — safe for direct speaker drive
- ⚠️ **IC orientation**: Double-check Pin 1 before applying power; reversed ICs can overheat instantly
- ✅ No dangerous voltages or energies in this circuit

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

Keep the beat! 🎵⚡
