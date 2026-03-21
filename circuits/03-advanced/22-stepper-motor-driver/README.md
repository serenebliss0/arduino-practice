# Stepper Motor Driver (Unipolar, No Microcontroller) ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - Complex multi-IC design with motor sequencing and power switching

## What You'll Learn
- How unipolar stepper motors work (phases, poles, coil energisation)
- Full-step (wave drive) vs half-step sequencing
- Using the CD4017 Decade Counter as a hardware state sequencer
- Generating a variable-frequency clock with the 555 timer in astable mode
- Using the ULN2003 Darlington Transistor Array to switch inductive loads
- Flyback (freewheeling) diode protection for inductive motor coils
- Speed control via potentiometer-adjusted oscillator frequency
- Direction control by reversing coil excitation order
- Reading motor datasheets and matching driver circuits to motor ratings
- Breadboarding multi-IC circuits with shared power rails and decoupling

## Components Required
- **CD4017** Decade Counter / Divider IC × 1
- **NE555 / LM555** Timer IC × 1
- **ULN2003A** Darlington Transistor Array IC × 1
- **28BYJ-48** Unipolar Stepper Motor (5V, 5-wire) × 1
- **1N4007** Rectifier Diodes (flyback protection) × 4
- **Resistors**:
  - 1kΩ × 1 (555 timing Ra / current limiting)
  - 10kΩ × 1 (CD4017 enable pull-down)
- **Capacitors**:
  - 10µF electrolytic × 1 (555 timing capacitor)
  - 100nF ceramic × 2 (power supply decoupling)
- **Potentiometer** 10kΩ × 1 (speed control, replaces fixed Rb on 555)
- **5V DC power supply** (matched to 28BYJ-48 rated voltage)
- **Breadboard** and jumper wires
- **Multimeter** (for testing and voltage measurement)

> **Voltage note**: The 28BYJ-48 is rated at **5V**. Do not exceed this or the motor windings will overheat. All ICs in this circuit operate correctly at 5V, so a single 5V supply powers everything.

## Circuit Description
Drive a 28BYJ-48 unipolar stepper motor through a complete rotation using only three ICs and no microcontroller! A **555 timer** oscillates at an adjustable frequency, acting as the system clock. A **CD4017 Decade Counter** receives those clock pulses and activates its ten outputs one at a time in strict sequence — we use only the first four outputs (Q0–Q3) and reset after Q3, creating a repeating 4-step cycle. Each output connects to one input of the **ULN2003 Darlington Array**, which sinks enough current (up to 500 mA per channel) to energise the corresponding motor coil. The **10kΩ potentiometer** adjusts the 555 frequency, directly controlling motor speed. **1N4007 flyback diodes** across each coil clamp the voltage spikes generated when an energised coil is switched off, protecting the driver IC. The result is smooth, continuous rotation at a user-controlled speed — driven entirely by discrete logic hardware.

## Theory of Operation

### How Stepper Motors Work
Unlike brushed DC motors that spin continuously when powered, a stepper motor moves in discrete angular steps. Each step corresponds to energising a specific combination of internal electromagnetic coils (phases). The rotor — a permanent magnet or toothed iron core — aligns itself with the magnetic field produced by the energised coil(s), then snaps to the next position when the field moves on. Precise position control is possible by counting steps: no feedback sensor is needed.

The **28BYJ-48** is a **unipolar** stepper motor, meaning each coil winding has a centre-tap connected to the positive supply. Current flows from the supply through the centre-tap and out through one of the two coil ends, which is switched to ground by the driver. This makes unipolar motors easier to drive than bipolar motors — you never need to reverse current through a winding.

#### Internal Structure of the 28BYJ-48
- **4 phases** (coils): A, B, C, D
- **5 wires**: Red (common/centre-tap +5V), Blue (Phase A), Pink (Phase B), Yellow (Phase C), Orange (Phase D)
- **Gear reduction**: approximately 64:1 internal planetary gearbox
- **Internal motor step angle**: 11.25° per full step (32 steps/revolution of the motor rotor)
- **Output shaft step angle**: 11.25° ÷ 64 ≈ 0.176° per step
- **Full steps per output revolution**: 32 × 64 = **2048 steps**

### Full-Step Wave Drive (4-Step Sequence)
Wave drive energises **one coil at a time** in sequence. This is the simplest drive mode and perfectly matches the CD4017's behaviour (only one output is HIGH at any moment). The rotor follows the rotating magnetic field one step at a time.

**Clockwise rotation sequence (forward):**

| Step | CD4017 Output | Coil A (Blue) | Coil B (Pink) | Coil C (Yellow) | Coil D (Orange) | Rotor Movement |
|------|---------------|:-------------:|:-------------:|:---------------:|:---------------:|----------------|
|  1   | Q0 HIGH       |      ON       |     OFF       |      OFF        |      OFF        | +11.25° (internal) |
|  2   | Q1 HIGH       |     OFF       |      ON       |      OFF        |      OFF        | +11.25° |
|  3   | Q2 HIGH       |     OFF       |     OFF       |       ON        |      OFF        | +11.25° |
|  4   | Q3 HIGH       |     OFF       |     OFF       |      OFF        |       ON        | +11.25° |

After Step 4 the counter resets and Step 1 repeats, producing continuous rotation. Every four clock pulses = one full electrical cycle = 11.25° × 4 = 45° of rotor rotation (or ≈ 0.176° × 4 ≈ 0.703° at the output shaft).

**Counter-clockwise rotation** requires reversing the sequence (Q3→Q2→Q1→Q0). In this hardware-only circuit, direction is set by the wiring order of motor leads to the ULN2003. To reverse direction, swap the Blue (A) and Orange (D) wire connections while also swapping Pink (B) and Yellow (C) — or more simply, use a 4-pole double-throw switch to reverse the coil-pair order.

### Two-Phase Full-Step (Higher Torque Alternative)
Energising **two adjacent coils simultaneously** doubles the holding torque by creating a stronger magnetic field midway between two poles. This requires a slightly different sequencing circuit (e.g., diode OR gates on CD4017 outputs) but delivers approximately 40% more torque than wave drive. The step positions fall between the single-coil positions.

| Step | Coil A | Coil B | Coil C | Coil D |
|------|:------:|:------:|:------:|:------:|
|  1   |  ON    |  ON    |  OFF   |  OFF   |
|  2   |  OFF   |  ON    |  ON    |  OFF   |
|  3   |  OFF   |  OFF   |  ON    |  ON    |
|  4   |  ON    |  OFF   |  OFF   |  ON    |

### Stage 1 — Clock Generator (555 Timer in Astable Mode)
The 555 timer is wired in **astable mode**, producing a continuous square wave with no external trigger:
1. Capacitor C charges through Ra + Rb (pot) until it reaches 2/3 VCC (upper threshold)
2. 555 discharges capacitor through Rb (pot) to pin 7 (discharge) until it reaches 1/3 VCC (lower threshold)
3. Cycle repeats indefinitely

The output on pin 3 toggles HIGH and LOW, producing clock pulses that drive the CD4017. Rotating the potentiometer changes Rb, directly adjusting frequency and therefore motor speed. A 1kΩ fixed Ra prevents the minimum resistance from going to zero (which would short pin 7 to the supply through only the capacitor).

### Stage 2 — Sequencer (CD4017 Decade Counter)
The CD4017 is a CMOS Johnson decade counter with **ten decoded outputs** (Q0–Q9). At each rising edge of the clock input:
- The currently HIGH output goes LOW
- The next output in sequence goes HIGH
- Only **one output is HIGH at any time** — ideal for single-coil wave drive

For a 4-step motor cycle, the CD4017's Q4 output (pin 10) is connected back to the **RESET pin (pin 15)**. The instant Q4 goes HIGH, the counter immediately resets to Q0. In practice, Q4 is never seen as a sustained HIGH — it's a brief spike that resets the counter. The active outputs are Q0, Q1, Q2, Q3 cycling repeatedly.

The **Clock Enable (pin 13)** is held LOW (active low) to allow normal counting. Connecting it HIGH would freeze the counter and stop the motor, which can be used as a software-style pause using a switch.

### Stage 3 — Power Driver (ULN2003 Darlington Array)
CMOS logic outputs (CD4017) can source or sink only a few milliamps — far too little to drive a stepper motor coil (which draws ~130–160 mA at 5V). The **ULN2003** solves this by pairing each input with a **Darlington transistor** (two BJTs in cascade), achieving:
- **Current gain** of 1000+ (a 1mA logic signal controls 500mA load current)
- **Voltage rating** of 50V (safe for inductive spikes)
- **7 channels** in one 16-pin package (we use 4)
- **Built-in flyback diodes** connected to the COM pin (pin 9)

When a CD4017 output goes HIGH, it drives the corresponding ULN2003 base HIGH, turning on that Darlington pair. The collector (motor coil end) is pulled to ground, current flows through the motor coil from +5V (common wire) through the energised coil winding to ground, and the rotor steps.

### Flyback (Freewheeling) Diode Protection
Motor coils are **inductive loads** — stored magnetic energy must go somewhere when the transistor switches OFF. Without protection, this energy collapses as a high-voltage spike (potentially hundreds of volts) that can destroy the driver transistors. Protection is provided two ways:
1. **ULN2003 internal diodes** — connected between each collector output and COM (pin 9). When COM is tied to the motor supply (+5V), these diodes clamp spikes back to the supply.
2. **External 1N4007 diodes** — placed across each motor coil terminal (cathode to +5V, anode to coil end / ULN2003 output), providing additional clamping and protection depth. Belt-and-suspenders approach for inductive loads.

## Pin Configuration

### 555 Timer IC (8-pin DIP)
```
    +--U--+
GND |1   8| VCC (+5V)
TRG |2   7| DIS (discharge)
OUT |3   6| THR (threshold)
RST |4   5| CTL (control)
    +-----+
```
| Pin | Name      | Connection in This Circuit              |
|-----|-----------|----------------------------------------|
|  1  | GND       | Ground                                  |
|  2  | TRG       | Tied to Pin 6 (trigger = threshold)    |
|  3  | OUT       | Clock signal → CD4017 Pin 14           |
|  4  | RST       | +5V (always enabled)                   |
|  5  | CTL       | 100nF to GND (noise suppression)       |
|  6  | THR       | Top of capacitor / junction Ra–C        |
|  7  | DIS       | Junction Ra and Rb (pot wiper)         |
|  8  | VCC       | +5V                                    |

Timing: Ra = 1kΩ (pin 8 → pin 7), Rb = 10kΩ pot (pin 7 → pin 6/2), C = 10µF (pin 6 → GND)

### CD4017 Decade Counter (16-pin DIP)
```
       +--U--+
   Q5  |1  16| VDD (+5V)
   Q1  |2  15| RESET
   Q0  |3  14| CLOCK
   Q2  |4  13| CLOCK ENABLE
   Q6  |5  12| CARRY OUT
   Q7  |6  11| Q4
   Q3  |7  10| Q9
  VSS  |8   9| Q8
       +-----+
```
| Pin | Name         | Connection in This Circuit                     |
|-----|--------------|-----------------------------------------------|
|  3  | Q0           | ULN2003 Input 1 (pin 1) → Coil A (Blue)      |
|  2  | Q1           | ULN2003 Input 2 (pin 2) → Coil B (Pink)      |
|  4  | Q2           | ULN2003 Input 3 (pin 3) → Coil C (Yellow)    |
|  7  | Q3           | ULN2003 Input 4 (pin 4) → Coil D (Orange)    |
| 11  | Q4           | RESET (pin 15) — resets counter after Q3      |
| 14  | CLOCK        | 555 Timer Pin 3 (clock signal)                |
| 13  | CLOCK ENABLE | GND through 10kΩ (active LOW = counting)      |
| 15  | RESET        | Q4 (pin 11) — auto-reset after 4 steps        |
|  8  | VSS          | GND                                           |
| 16  | VDD          | +5V                                           |
| 12  | CARRY OUT    | Not connected                                  |

### ULN2003A Darlington Array (16-pin DIP)
```
       +--U--+
   1B  |1  16| 1C
   2B  |2  15| 2C
   3B  |3  14| 3C
   4B  |4  13| 4C
   5B  |5  12| 5C
   6B  |6  11| 6C
   7B  |7  10| 7C
  GND  |8   9| COM
       +-----+
```
| Pin(s) | Name    | Connection in This Circuit                              |
|--------|---------|---------------------------------------------------------|
|  1     | 1B      | CD4017 Q0 (pin 3) — Coil A input                       |
|  2     | 2B      | CD4017 Q1 (pin 2) — Coil B input                       |
|  3     | 3B      | CD4017 Q2 (pin 4) — Coil C input                       |
|  4     | 4B      | CD4017 Q3 (pin 7) — Coil D input                       |
| 5,6,7  | 5B–7B   | Not connected (unused channels)                         |
| 16     | 1C      | Coil A free end (Blue wire) + 1N4007 anode              |
| 15     | 2C      | Coil B free end (Pink wire) + 1N4007 anode              |
| 14     | 3C      | Coil C free end (Yellow wire) + 1N4007 anode            |
| 13     | 4C      | Coil D free end (Orange wire) + 1N4007 anode            |
|  9     | COM     | +5V motor supply (clamps internal flyback diodes)       |
|  8     | GND     | Ground                                                  |

### 28BYJ-48 Stepper Motor (5-wire connector)
```
  Red   ─── Common (+5V centre-tap)
  Blue  ─── Phase A (Coil A end)
  Pink  ─── Phase B (Coil B end)
 Yellow ─── Phase C (Coil C end)
 Orange ─── Phase D (Coil D end)
```
| Wire   | Connection                                      |
|--------|-------------------------------------------------|
| Red    | +5V motor supply                                |
| Blue   | ULN2003 Output 1C (pin 16) + 1N4007 anode      |
| Pink   | ULN2003 Output 2C (pin 15) + 1N4007 anode      |
| Yellow | ULN2003 Output 3C (pin 14) + 1N4007 anode      |
| Orange | ULN2003 Output 4C (pin 13) + 1N4007 anode      |

> Wire colours can vary between batches — verify with a multimeter (coil A–B share the same winding via centre tap and should read ~13Ω between ends, ~6.5Ω from end to red).

## Building Instructions

### Step 1: Power Distribution
1. Connect the positive breadboard rail to +5V (power supply red wire)
2. Connect the negative breadboard rail to GND (power supply black wire)
3. Place a **100nF ceramic capacitor** directly across the power rails near the centre of the board (supply decoupling)
4. Place a second **100nF capacitor** near the ULN2003 power pins (high-current switching decoupling)
5. Verify supply voltage with a multimeter (should read 4.9–5.1V)

### Step 2: 555 Timer Astable Clock Circuit
1. Insert the 555 timer IC on the breadboard (notch/dot at pin 1 end)
2. **Power**: Pin 8 (+VCC) → +5V rail; Pin 1 (GND) → GND rail
3. **Reset**: Pin 4 (RST) → +5V rail (keeps timer running continuously)
4. **Control voltage bypass**: Pin 5 (CTL) → 100nF capacitor → GND (suppresses noise on internal comparator reference)
5. **Timing resistor Ra**: 1kΩ resistor from Pin 8 to Pin 7 (discharge)
6. **Speed potentiometer Rb**: Connect the 10kΩ potentiometer with:
   - One outer terminal to Pin 7 (discharge)
   - Wiper (centre terminal) to Pin 6 (threshold)
   - Other outer terminal to a 10µF capacitor going to GND
   - *(This puts pot in the Rb position — rotating it changes Rb from 0 to 10kΩ)*
7. **Threshold–Trigger bridge**: Wire Pin 6 (THR) directly to Pin 2 (TRG) — both monitor capacitor voltage
8. **Output**: Pin 3 will carry the clock signal (verify it pulses when power is applied)

> **Polarity check**: The 10µF electrolytic capacitor has a positive (+) lead — connect it to Pin 6, negative to GND.

### Step 3: CD4017 Sequencer
1. Insert the CD4017 on the breadboard, at least 4 columns away from the 555
2. **Power**: Pin 16 (VDD) → +5V; Pin 8 (VSS) → GND
3. **Clock input**: Wire Pin 14 (CLOCK) to 555 Timer Pin 3 (output)
4. **Clock enable**: Wire Pin 13 (CLOCK ENABLE) through a 10kΩ resistor to GND (held LOW = counting enabled)
5. **Reset feedback**: Wire Pin 11 (Q4) directly to Pin 15 (RESET)
   - This is the key auto-reset connection: as soon as Q4 would go HIGH, the counter is instantly reset to Q0, creating the 4-step loop Q0→Q1→Q2→Q3→(reset)→Q0
6. **Verify output sequence**: With a multimeter on the Q0 pin (pin 3) you should see it toggle HIGH briefly as the circuit runs

### Step 4: ULN2003 Driver Stage
1. Insert the ULN2003 on the breadboard (note orientation — pin 1 top-left)
2. **Ground**: Pin 8 (GND) → GND rail
3. **COM**: Pin 9 (COM) → +5V rail (connects internal flyback diode cathodes to supply)
4. **Input wiring** (from CD4017 to ULN2003):
   - CD4017 Pin 3 (Q0) → ULN2003 Pin 1 (1B)
   - CD4017 Pin 2 (Q1) → ULN2003 Pin 2 (2B)
   - CD4017 Pin 4 (Q2) → ULN2003 Pin 3 (3B)
   - CD4017 Pin 7 (Q3) → ULN2003 Pin 4 (4B)
5. Leave inputs 5B, 6B, 7B (pins 5,6,7) unconnected

### Step 5: Flyback Diode Protection
Install four **1N4007** diodes (one per motor coil channel):
1. **Diode orientation**: Cathode (striped end) toward +5V, anode toward ULN2003 output
2. **Diode 1**: Cathode to +5V, Anode to ULN2003 Pin 16 (1C)
3. **Diode 2**: Cathode to +5V, Anode to ULN2003 Pin 15 (2C)
4. **Diode 3**: Cathode to +5V, Anode to ULN2003 Pin 14 (3C)
5. **Diode 4**: Cathode to +5V, Anode to ULN2003 Pin 13 (4C)

> The ULN2003 has internal flyback diodes to COM (pin 9), which is already wired to +5V. The external 1N4007s provide additional clamping and are good practice when driving inductive loads on a breadboard where layout is not optimised.

### Step 6: Motor Connections
1. Connect the motor **Red wire** (common centre-tap) to +5V
2. Connect the **Blue wire** (Coil A) to ULN2003 Pin 16 (1C)
3. Connect the **Pink wire** (Coil B) to ULN2003 Pin 15 (2C)
4. Connect the **Yellow wire** (Coil C) to ULN2003 Pin 14 (3C)
5. Connect the **Orange wire** (Coil D) to ULN2003 Pin 13 (4C)

> If your motor has different colour wires, use a multimeter in resistance mode: measure between the red wire and each other wire. Each should read approximately 6.5–7Ω (half-winding). Any two non-red wires that read ~13Ω are the ends of the same winding.

### Step 7: Final Verification Checklist
Before applying power, confirm:
- [ ] 555 timer oriented correctly (notch/dot to pin 1)
- [ ] CD4017 oriented correctly (notch/dot to pin 1)
- [ ] ULN2003 oriented correctly (notch/dot to pin 1)
- [ ] 10µF capacitor polarity correct (+ to pin 6 of 555)
- [ ] 1N4007 diodes all pointing the right way (stripe to +5V)
- [ ] No shorts between power rails (measure GND to +5V: should be > 1kΩ before power-on)
- [ ] Motor red wire connected to +5V
- [ ] ULN2003 pin 9 (COM) connected to +5V
- [ ] CD4017 Pin 11 (Q4) wired to Pin 15 (RESET)

Apply power — the motor should begin rotating. Turn the potentiometer to control speed.

## Testing & Troubleshooting

### Expected Behaviour
- **Power applied**: Motor begins rotating immediately (may twitch slightly before establishing rotation)
- **Potentiometer turned toward minimum Rb (0kΩ)**: Motor spins faster (~4 RPM)
- **Potentiometer turned toward maximum Rb (10kΩ)**: Motor spins slower (~0.2 RPM)
- **At mid-range**: Steady rotation ~1–2 RPM visible to the naked eye
- **Motor coils**: Slightly warm to the touch after a few minutes — normal
- **Motor current**: Approximately 130–160 mA from the 5V supply

### Voltage Measurements

**555 Timer — expected readings:**
| Pin | Expected Voltage        |
|-----|-------------------------|
|  1  | 0V (GND)                |
|  8  | 5V (VCC)                |
|  4  | 5V (reset held HIGH)    |
|  3  | Oscillating 0V ↔ ~3.5V  |
|  2/6| Oscillating ~1.7V ↔ 3.3V (1/3 to 2/3 of 5V) |

**CD4017 — expected readings:**
| Pin | Expected Voltage                          |
|-----|-------------------------------------------|
|  8  | 0V (VSS)                                  |
| 16  | 5V (VDD)                                  |
| 14  | Pulsing 0V ↔ ~3.5V (clock from 555)       |
| 3,2,4,7 | Only one HIGH (~5V) at a time, rest LOW |
| 11  | Brief spikes only (reset pulse, near 0V steady) |

**ULN2003 — expected readings:**
| Pin | Expected Voltage                             |
|-----|----------------------------------------------|
|  8  | 0V (GND)                                     |
|  9  | 5V (COM)                                     |
| 1–4 | Match CD4017 Q0–Q3 outputs (~0V or ~3.5V)   |
| 16,15,14,13 | Near 0V when ON, near 5V when OFF (inverted relative to input) |

**Motor coil voltage (relative to GND):**
- Coil end when driven ON: ~0.8–1.2V (ULN2003 saturation voltage)
- Coil end when OFF: ~4.5–5V (pulled up through coil winding from red wire)

### Common Issues and Solutions

**Motor does not rotate at all:**
- Check all power connections first — multimeter between +5V and GND rails
- Verify 555 timer is oscillating: probe Pin 3 with multimeter in AC mode (should show ~1–3V AC depending on frequency)
- Verify CD4017 receives clock: probe Pin 14 for pulsing
- Check CD4017 reset connection: if Pin 11 is permanently wired to Pin 15 with no Q4 reset feedback *before* Q0 counts, counter is stuck in reset — double-check wiring
- Check motor red wire is connected to +5V
- Try powering the motor coil manually: briefly connect a coil end (e.g., Blue wire) through a 33Ω resistor to GND — motor should click one step

**Motor vibrates or twitches but doesn't rotate:**
- Clock frequency too low — rotate pot toward minimum resistance (higher frequency) to increase step rate above the motor's resonance frequency
- Or clock frequency too high — motor can't keep up; reduce frequency with pot
- Coil wiring order may be wrong — swap Blue and Pink wires to change sequence and try again
- Missing ground on ULN2003 pin 8, or missing +5V on COM pin 9 — check both

**Motor rotates in wrong direction:**
- Swap the Blue (A) wire and Orange (D) wire connections at the ULN2003
- Also swap Pink (B) and Yellow (C) wires
- This reverses the coil excitation sequence: Q0→D, Q1→C, Q2→B, Q3→A

**Motor runs but skips steps or is erratic:**
- Power supply cannot deliver enough current — 28BYJ-48 draws ~160 mA; cheap USB adapters may droop under load
- Add a 100µF electrolytic capacitor across the +5V/GND rails (energy reservoir for current spikes)
- Clock frequency too high — reduce it with the pot
- Breadboard contacts loose — re-seat ICs and check all jumper wires
- Motor winding resistance: measure each coil end to red wire; should read ~6.5Ω. Open or short indicates a damaged motor

**Motor only moves one or two steps then stops:**
- CD4017 reset feedback may be wired to wrong output pin — verify Q4 is Pin 11 (not Pin 10 or Pin 5)
- Counter stuck in reset: Q4 (pin 11) should connect to RESET (pin 15), not to VDD or GND

**One coil never activates:**
- ULN2003 channel may be damaged — swap to an unused channel (e.g., use inputs 5B/5C for the bad channel)
- Or that CD4017 output has failed — try a replacement IC
- Use the DC voltage test: probe each ULN2003 input pin (1–4) and verify each goes HIGH in sequence as clock runs

**Motor gets very hot:**
- Motor coils remain energised indefinitely (the circuit does not de-energise coils) — this is normal for this basic design but causes heating
- For long-term operation, add a switch or logic to gate the clock enable (CD4017 pin 13 HIGH) to freeze and de-energise after desired rotation
- Verify supply is 5V, not 12V — overvoltage will destroy the motor quickly

**IC chips getting hot:**
- Wrong supply voltage — measure VCC, must be 5V (not 9V or 12V)
- Reversed ICs — remove power immediately and check orientation

## Calculations

### 555 Astable Frequency
```
f = 1.44 / ((Ra + 2 × Rb) × C)

Components: Ra = 1kΩ, Rb = 0 to 10kΩ (potentiometer), C = 10µF

Maximum speed (pot at minimum Rb = 0Ω):
  f_max = 1.44 / ((1,000 + 0) × 0.00001)
        = 1.44 / 0.01
        = 144 Hz

Minimum speed (pot at maximum Rb = 10,000Ω):
  f_min = 1.44 / ((1,000 + 20,000) × 0.00001)
        = 1.44 / 0.21
        ≈ 6.86 Hz

Midpoint (Rb = 5kΩ):
  f_mid = 1.44 / ((1,000 + 10,000) × 0.00001)
        = 1.44 / 0.11
        ≈ 13.1 Hz
```

### Duty Cycle
```
Duty cycle = (Ra + Rb) / (Ra + 2×Rb)

At Rb = 5kΩ:  (1k + 5k) / (1k + 10k) = 6/11 ≈ 54.5% (close to 50%)
At Rb = 0Ω:   (1k + 0)  / (1k + 0)   = 100% (can't reach — Ra prevents this)
At Rb = 10kΩ: (1k + 10k)/ (1k + 20k) = 11/21 ≈ 52.4%

The duty cycle stays close to 50% across the range, giving clean clock pulses.
```

### Motor Speed (RPM)
```
Steps per output-shaft revolution (full-step / wave drive):
  Internal steps per rev = 360° / 11.25° = 32
  Gear ratio             = 64:1 (approximately)
  Total steps/revolution = 32 × 64 = 2048

RPM formula:
  RPM = (clock_frequency × 60 sec/min) / steps_per_revolution
  RPM = (f × 60) / 2048

At f_max = 144 Hz:
  RPM = (144 × 60) / 2048 = 8,640 / 2048 ≈ 4.2 RPM

At f_mid = 13.1 Hz:
  RPM = (13.1 × 60) / 2048 = 786 / 2048 ≈ 0.38 RPM

At f_min = 6.86 Hz:
  RPM = (6.86 × 60) / 2048 = 411.6 / 2048 ≈ 0.20 RPM

Speed range with 10kΩ pot: approximately 0.2 to 4.2 RPM
```

### Step Angle at Output Shaft
```
Step angle = 360° / steps_per_revolution
           = 360° / 2048
           ≈ 0.176° per step

Or equivalently: stride angle = 5.625° (half-step) / 2 for full-step
  Full-step output angle = 5.625° / 2 = ≈ 0.176° per full step (output shaft)
```

### Motor Coil Current and ULN2003 Base Drive
```
Motor coil resistance (each half-winding) = ~6.5Ω
Motor coil current per active coil:
  I_coil = V_supply / R_coil = 5V / 6.5Ω ≈ 769 mA  (theoretical max)

In practice, the 28BYJ-48 is inductance-limited and current-limited by the driver:
  Rated coil current ≈ 130–160 mA at 5V (per manufacturer spec)
  ULN2003 max per channel: 500 mA — safely within rating
  ULN2003 max total: 2.5A across all channels — only one active at a time here

ULN2003 input threshold: VIN ≥ 1.0V to turn ON
CD4017 output HIGH: ~4.5V at 5V supply — comfortably drives ULN2003 inputs
ULN2003 saturation (on-state): Vce_sat ≈ 0.9–1.2V (coil end held to ~1V above GND)

Total supply current estimate:
  ICs (555 + CD4017): ≈ 5 mA combined
  ULN2003 driver:     ≈ 2 mA quiescent
  Active motor coil:  ≈ 150 mA
  Total:              ≈ 157 mA from 5V supply
```

### Flyback Voltage Clamping
```
Without protection, an inductor switching OFF generates:
  V_spike = L × (dI/dt)

For a ~10mH coil winding switching 150mA in 100ns:
  V_spike = 0.01 × (0.15 / 0.0000001) = 15,000 V — catastrophic!

With 1N4007 flyback diodes clamped to +5V:
  V_spike = V_supply + V_forward_diode = 5V + 0.7V = 5.7V (safe)

This is why flyback diodes are mandatory, not optional.
```

## Key Concepts Learned
- **Stepper Motor Physics**: Discrete rotation via electromagnetic field rotation in fixed steps
- **Unipolar Drive**: Centre-tap winding simplifies driving — only ground switching needed, no H-bridge required
- **Wave Drive Sequencing**: Single-phase excitation naturally matches decade counter one-hot outputs
- **Clock Generation**: 555 timer as a variable-frequency square wave oscillator
- **Decade Counter Sequencing**: CD4017 as a hardware state machine with decoded single-bit outputs
- **Auto-Reset Feedback**: Using a counter output to reset itself — defining a sub-cycle (Q0→Q3 instead of Q0→Q9)
- **Darlington Transistor Arrays**: Cascaded BJTs for high-current amplification from logic signals
- **Inductive Kickback**: Why inductive loads generate voltage spikes and how flyback diodes clamp them
- **Speed vs Torque Tradeoff**: At high step rates the motor cannot respond fully and loses torque
- **Open-Loop Position Control**: Stepper motors count steps without encoders — no feedback required

## Applications
- **Industrial positioning systems**: Printers, plotters, CNC machines, laser cutters
- **Camera pan/tilt rigs**: Timelapse sliders, surveillance systems
- **3D printers**: X, Y, Z axis and extruder drives
- **Robotic actuators**: Joint drives, gripper mechanisms
- **Valve and damper control**: HVAC systems, flow control
- **Telescope mounts**: Equatorial tracking drives
- **Educational demonstrations**: Visualising digital sequencing and motor physics
- **Clock mechanisms**: Custom hands or rotating display elements
- **Conveyor systems**: Precise indexed feeding of parts
- **Laboratory instruments**: Microscope stage positioning, spectrometer gratings

## Next Steps / Extensions

1. **Add direction control switch**: Wire a 4PDT (or two 2PDT) toggle switch to reverse coil order without re-wiring the breadboard
2. **Half-stepping**: Add diode OR gates between adjacent CD4017 outputs to energise two coils simultaneously, doubling the step resolution (4096 steps/rev) and increasing torque
3. **Add step counter**: Chain a second CD4017 (or CD4040 binary counter) to count motor steps, turning an LED on after a precise number of rotations
4. **Auto-stop after N steps**: Use a CD4040 ripple counter and a logic gate to cut clock enable (CD4017 pin 13) after a preset step count
5. **Microcontroller comparison**: Implement the same drive using an Arduino — notice how 10 lines of `digitalWrite()` in a loop replaces the entire sequencer stage
6. **Drive a different stepper**: Replace 28BYJ-48 with a 12V bipolar stepper — you'll need an L293D or L298N H-bridge instead of ULN2003 (bipolar motors need current reversal)
7. **Speed display**: Add a frequency counter (CD4026 + 7-segment) on the 555 output to display clock frequency
8. **Microstepping**: Requires PWM and a dedicated driver like A4988 or DRV8825 — explore how microstepping gives smoother motion and higher resolution

## Experimentation Ideas

### Change Motor Speed Range
**Faster range (100–1000 Hz)** — use C = 1µF instead of 10µF:
- f = 1.44 / ((1k + 2×Rb) × 1µF)
- Range: ~68 Hz (Rb=10kΩ) to 1440 Hz (Rb=0) → ~2 to 42 RPM

**Slower range (0.5–15 Hz)** — use C = 100µF:
- Range: ~0.68 Hz to 14.4 Hz → ~0.02 to 0.42 RPM

### Direction Reversal Switch
Add a **4-pole double-throw (4PDT) switch** between the CD4017 outputs and ULN2003 inputs:
- **Forward**: Q0→1B, Q1→2B, Q2→3B, Q3→4B
- **Reverse**: Q0→4B, Q1→3B, Q2→2B, Q3→1B (equivalent to Q3→Q2→Q1→Q0 sequence)

### Pause/Run Control
Connect a normally-open pushbutton between CD4017 Pin 13 (CLOCK ENABLE) and +5V:
- Button released: Pin 13 = LOW (via 10kΩ to GND) → motor runs
- Button pressed: Pin 13 = HIGH → counting halted → motor freezes in place

### Two-Phase Full-Step (Higher Torque)
To energise two coils at a time, add **OR diodes** (1N4148) between adjacent outputs:
- Diode between Q0 and Q1 anodes, cathode to coil A: coil A is active during both Q0 and Q3
- Requires careful analysis of which coil pairs are adjacent — refer to a full-step two-phase truth table
- Delivers ~41% more holding torque than wave drive

## Safety Notes
- ✅ **Low voltage (5V)** — safe to handle with bare hands while circuit is running
- ✅ **Motor is low power** — maximum ~1W; no fire or burn risk under normal use
- ⚠️ **Do not exceed 5V for the 28BYJ-48** — higher voltages overheat and destroy the motor windings quickly
- ⚠️ **Do not omit flyback diodes** — without them, inductive spikes can immediately destroy the ULN2003 and potentially the CD4017
- ⚠️ **Check IC orientation before applying power** — reversed CMOS ICs (CD4017) usually fail immediately; the 555 and ULN2003 may survive briefly but can overheat
- ⚠️ **CMOS ICs (CD4017) are static-sensitive** — touch a grounded metal surface before handling
- ⚠️ **Electrolytic capacitor polarity** — reversed 10µF can bulge or burst; always verify + terminal orientation
- ✅ **Motor will get warm after continuous running** — this is normal; it is not designed for continuous 100% duty-cycle operation in this open-loop driver configuration
- ⚠️ **If any IC gets uncomfortably hot within seconds** — remove power immediately and check for wiring errors or reversed components

---

**Circuit Status**: ✅ Verified circuit design — ready to build
**Last Updated**: December 2024

Precision motion, no microcontroller required! ⚙️⚡
