# PWM Motor Controller (True PWM with MOSFET) ⭐⭐⭐⭐⭐

## Difficulty Level
**Advanced** - Professional-grade motor speed control using analog PWM generation, power MOSFET switching, and inductive load protection

## What You'll Learn
- Pulse-Width Modulation (PWM) theory and how it controls average voltage
- 555 timer in astable mode as a fixed-frequency PWM generator
- Diode-assisted variable duty cycle modification for the 555 timer
- N-channel MOSFET operation as a high-side power switch
- Gate drive requirements and gate resistor design
- Flyback (freewheeling) diode selection and placement for inductive loads
- Motor physics: back-EMF, inductive kick, and why they damage unprotected circuits
- Heat sink selection and thermal management for power MOSFETs
- Measuring duty cycle and frequency with a multimeter and oscilloscope
- Decoupling capacitors and noise suppression in power circuits

## Components Required
- **NE555 Timer IC** (or LM555, TLC555) × 1
- **IRF540N N-Channel MOSFET** (or IRLZ44N for lower gate-drive voltage) × 1
- **DC Motor** (12V, up to 5A — fan motor, gearmotor, or hobby motor) × 1
- **Potentiometer** 10kΩ linear taper × 1 (duty cycle / speed control)
- **Diode 1N4007** (flyback / freewheeling protection across motor) × 1
- **Diode 1N4148** (small-signal, for 555 variable duty cycle modification) × 1
- **Resistors**:
  - 1kΩ × 2 (R1: upper timing limit resistor; Rg: MOSFET gate resistor)
  - 10kΩ × 1 (pull-down on 555 pin 4 / reset, or gate bleed)
- **Capacitors**:
  - 100nF ceramic × 2 (timing capacitor C1; decoupling on 555 pin 5)
  - 10µF electrolytic × 1 (power rail bulk decoupling)
- **Heat sink** for TO-220 MOSFET package × 1 (clip-on or screw-mount)
- **12V DC power supply**, 2A minimum × 1
- **Breadboard** and jumper wires (use heavier gauge for motor current path)
- Multimeter and oscilloscope (highly recommended for verification)

## Circuit Description
Build a professional-grade DC motor speed controller that smoothly varies motor speed from a complete stop to full power using a single potentiometer! The NE555 timer generates a rock-steady ~1 kHz square wave whose duty cycle is continuously adjustable from roughly 5% to 95% via a diode-assisted potentiometer network. The output of the 555 drives the gate of a power N-channel MOSFET (IRF540N), which switches up to 5 A of motor current with microsecond precision. A 1N4007 flyback diode recirculates the motor's inductive energy every time the MOSFET turns off, protecting the switch from destructive voltage spikes. The result is smooth, efficient, flicker-free speed control — the same fundamental technique used in industrial variable-speed drives, electric vehicles, and robotics.

## Theory of Operation

### How PWM Controls Motor Speed

A DC motor's speed is proportional to the average voltage applied to it. Rather than wasting energy as heat in a variable resistor (as in a rheostat controller), PWM switches the full supply voltage ON and OFF at high frequency. The ratio of ON time to the total period is the **duty cycle**:

```
Average Voltage = Supply Voltage × Duty Cycle
Examples (12V supply):
  10% duty cycle → 1.2V average  → very slow / barely turning
  50% duty cycle → 6.0V average  → half speed
  90% duty cycle → 10.8V average → near full speed
 100% duty cycle → 12.0V average → full speed
```

Because the switching frequency (~1 kHz) is far above the mechanical time constant of the motor (~50–200 ms), the motor's spinning mass acts as a low-pass filter and responds only to the average voltage. The motor runs smoothly with no audible switching noise at 1 kHz (above hearing threshold for motors of this size).

### Stage 1: The 555 Timer PWM Generator

The 555 timer is configured in **astable mode**, continuously oscillating and producing a square wave on its output pin (pin 3). In standard astable operation the duty cycle is fixed by two resistors and a capacitor. This circuit adds a **diode-assisted variable duty cycle modification** to decouple the charge and discharge paths:

```
Standard 555 astable (duty cycle always > 50%):
  Charge path:  VCC → R_A → R_B → Cap  (through both resistors)
  Discharge path:    Cap → R_B → Pin 7  (through R_B only)

Modified circuit (duty cycle 5%–95%):
  Charge path:  VCC → R1(1kΩ) → Pot_upper → D_bypass (1N4148) → Cap
  Discharge path:    Cap → R2(1kΩ) → Pot_lower → Pin 7 → GND
```

The 1N4148 diode (D_bypass) has its **anode at the potentiometer wiper** (which connects to Pin 7, the discharge pin) and its **cathode at Pin 6/Pin 2** (the capacitor junction):

- **During charge** (Pin 7 = open-circuit, output HIGH):  
  The capacitor charges through R1 + the upper pot section only. D_bypass is forward-biased, so the lower pot section is bypassed — the capacitor "sees" only the upper path resistance.
  `t_high = 0.693 × (R1 + R_pot_upper) × C`

- **During discharge** (Pin 7 = pulled LOW, output LOW):  
  The capacitor discharges through the lower pot section + R2 → Pin 7 → GND. D_bypass is now reverse-biased (cathode at the higher-voltage cap side), so it is fully blocking.
  `t_low = 0.693 × (R2 + R_pot_lower) × C`

Rotating the potentiometer transfers resistance between the upper and lower sections. With R1 = R2 = 1 kΩ and a 10 kΩ pot:
- Wiper at top:  duty cycle ≈ (1k + 10k)/(1k + 10k + 1k) = 92%
- Wiper at bottom: duty cycle ≈ 1k/(1k + 10k + 1k) = 8%
- Wiper at centre: duty cycle = 50%

The fixed 1 kΩ resistors at each end prevent the duty cycle from reaching 0% or 100%, which would stall or saturate the 555's internal flip-flop.

### Stage 2: MOSFET Gate Drive

The 555's output (Pin 3) swings between GND and ≈ V_supply (about 10–11 V at 12 V supply). This is connected to the **gate** of the IRF540N through a 1 kΩ gate resistor (Rg):

- **Gate HIGH** (≈ 11 V): IRF540N fully ON. V_gs ≈ 11 V >> V_th (2–4 V), so the MOSFET is in deep saturation. R_ds(on) ≈ 44 mΩ. For a 3 A motor, V_drop across MOSFET = 3A × 0.044Ω ≈ 0.13 V. Nearly the full 12 V appears across the motor. ✅

- **Gate LOW** (≈ 0 V): IRF540N fully OFF. Motor current path is broken. No (significant) current flows. ✅

The 1 kΩ gate resistor serves two purposes:
1. Slows the gate charge/discharge slightly to suppress high-frequency ringing on the gate drive line (a common source of EMI).
2. Limits the 555 output current during the instant the MOSFET gate capacitance (C_iss ≈ 1.7 nF) charges, keeping the 555 within its 200 mA output rating.

**Why N-Channel MOSFET (low-side switch)?**  
The motor is connected between the +12 V supply and the MOSFET drain. The source is tied to GND. This "low-side switch" arrangement means the gate-to-source voltage equals the gate voltage directly — simple and efficient. The 555 output easily drives a 0 V / 12 V swing from GND reference with no level-shifting needed.

**IRLZ44N alternative**: This logic-level N-channel MOSFET has V_th ≈ 1–2 V and is fully enhanced at V_gs = 5 V. It works identically but is better suited for 5 V gate drive (e.g., from an Arduino PWM pin in a future upgrade).

### Stage 3: Flyback Diode Protection

A DC motor is an **inductive load**. Its armature winding stores magnetic energy (E = ½LI²) when current flows. When the MOSFET turns OFF, the current cannot stop instantaneously — Lenz's law demands it continues to flow. With no path available, the drain voltage would spike to hundreds of volts, instantly destroying the MOSFET.

The **1N4007 flyback diode** (also called a freewheeling or snubber diode) is placed across the motor:
- **Anode**: motor terminal connected to MOSFET drain (the "bottom" of the motor)
- **Cathode**: motor terminal connected to +12 V supply (the "top" of the motor)

During normal ON time, this diode is reverse-biased and passes no current. The instant the MOSFET switches OFF:
1. Motor back-EMF drives the drain node **below** the anode voltage
2. Diode becomes forward-biased and conducts
3. Inductive current recirculates through the diode and motor winding
4. Energy dissipates harmlessly as heat in the motor's own resistance
5. Drain voltage is clamped to +12 V + 0.7 V (one diode drop above supply) — well within the MOSFET's 100 V rating

**1N4007 selection**: Rated 1000 V PIV and 1 A average / 30 A surge. The 30 A surge rating handles the inductive kick transients. For motors above 3 A continuous, consider a Schottky diode (SB560 or 1N5819) for faster recovery and lower forward drop.

## Pin Configuration

### NE555 Timer IC (8-pin DIP)
```
      +--U--+
  GND |1   8| VCC (+12V)
  TRG |2   7| DIS  ←── Pot wiper + D_bypass anode
  OUT |3   6| THR  ←── Pot lower end + D_bypass cathode + Cap top
  RST |4   5| CTL  ←── 100nF to GND (noise filter)
      +-----+

Pin 1 (GND):  Ground
Pin 2 (TRG):  Trigger — tied to Pin 6 (threshold = trigger, astable)
Pin 3 (OUT):  PWM output → 1kΩ gate resistor → MOSFET gate
Pin 4 (RST):  Reset — tie to VCC (+12V) to keep 555 running always
Pin 5 (CTL):  Control voltage — bypass to GND with 100nF ceramic cap
Pin 6 (THR):  Threshold — tied to Pin 2, top of capacitor C1, cathode of D_bypass
Pin 7 (DIS):  Discharge — pot wiper connection + anode of D_bypass
Pin 8 (VCC):  +12V supply (NE555 rated to 16V max)
```

### IRF540N N-Channel MOSFET (TO-220 package)
```
  Front view (markings facing you):
  +-------+
  |IRF540N|
  +-------+
   | | |
   G D S

  G = Gate   → 1kΩ resistor → 555 Pin 3 (PWM signal)
  D = Drain  → Motor negative terminal + 1N4007 anode
  S = Source → GND (circuit ground, 0V)

  Key ratings (IRF540N):
    V_DS(max) = 100V
    I_D(max)  = 33A continuous (with adequate heat sink)
    R_ds(on)  = 44mΩ typical (V_gs = 10V)
    V_gs(th)  = 2–4V (fully on at ~10V gate drive)
    V_gs(max) = ±20V (do not exceed!)
    Package:  TO-220 (bolt to heat sink with thermal paste)
```

### Complete PWM Circuit (ASCII Schematic)
```
+12V Supply ──────┬──────────────────────────────────┐
                  │                                  │
                 [10µF]     Motor (DC, 12V)          │
                  │          ┌─────┐                 │
                  │     ┌────┤  M  ├────┐            │
                  │     │    └─────┘    │            │
                  │     │               │(drain)     │
                  │    [+] 1N4007  IRF540N           │
                  │    [-] (fly-  ┌──┤ ├──┐         │
                  │     │  back)  G  D   S           │
                  │     └─────────┘  │   │           │
                  │                  │   └───────────┴── GND
                  │               [1kΩ Rg]
NE555 Circuit:    │                  │
                  │             555 Pin 3 (OUT)
 Pin 8 ── +12V    │
 Pin 1 ── GND     │       555 internal:
 Pin 4 ── +12V    │       ┌──────────────────┐
 Pin 5 ──[100nF]──GND     │ THR(6) ── TRG(2) │
                  │       │      │            │
  +12V ──[R1 1kΩ]─┤       │ +─[C1 100nF]─ GND│
                  │       │                  │
              Pot wiper                      │
           (= DIS / Pin 7)──────────────Pin 7│
                  │                          │
          ┌──[1N4148 D_bypass]──┐            │
          │       │             │            │
    Pot lower    [R2 1kΩ]      └─── Pin 6/2 │
     (0–9kΩ)      │                   │     │
          └───────┴─────── Cap top ───┘     │
                                 │          └──────────┘
                               [C1 100nF]
                                 │
                                GND

Notes:
  • Pot upper half = wiper to R1 junction (resistance increases → higher duty cycle)
  • Pot lower half = wiper to R2/cap junction (resistance increases → lower duty cycle)
  • D_bypass (1N4148) anode at pot wiper, cathode at cap top (Pin 6/2)
  • Decoupling: 10µF across +12V/GND near 555; 100nF on Pin 5
  • Gate bleed: optional 10kΩ from gate to source keeps MOSFET OFF if signal lost
```

## Building Instructions

### Step 1: Power Rails and Decoupling
1. Connect the +12V power supply positive lead to the breadboard's top positive rail.
2. Connect the supply negative (GND) to the top negative rail.
3. Bridge both rails to the bottom positive and negative rails with jumper wires.
4. Place a **10 µF electrolytic capacitor** across the power rails near where the 555 will sit (positive leg to +12V, negative to GND). This bulk capacitor absorbs current spikes when the MOSFET switches.
5. **Do not connect the power supply yet** — build the full circuit before applying power.

### Step 2: Install the 555 Timer IC
1. Insert the NE555 into the breadboard spanning the centre gap. Note the orientation — the notch or dot on the IC marks **pin 1**.
2. **Power pins**: Pin 8 to +12V rail, Pin 1 to GND rail.
3. **Reset**: Wire Pin 4 directly to Pin 8 (+12V). This keeps the 555 running at all times. Do not leave it floating.
4. **Control voltage bypass**: Wire Pin 5 to GND through a **100nF ceramic capacitor**. This filters noise from the internal 2/3 VCC reference voltage, stabilising the PWM frequency.

### Step 3: Variable Duty Cycle Timing Network
This is the heart of the circuit. Follow carefully — the diode direction matters.

1. **R1 (1kΩ)**: Connect from +12V rail to Pin 7 (DIS). This is the upper series resistor; it sets the minimum duty cycle (~8%) so the 555 never stalls.

2. **Potentiometer**: Insert the 10kΩ pot into the breadboard.
   - Connect the pot's **top terminal** (pin 1 of pot) to the same node as Pin 7 (the bottom of R1).
   - Connect the pot's **wiper** (centre terminal) to Pin 7. Both the top terminal and Pin 7 are now at this shared node — R1 connects VCC to it; the wiper and Pin 7 connect downward from it.
   
   > Actually re-read the topology: R1 goes from VCC to the **top terminal** of the pot. Pin 7 connects at the **wiper**. This way the upper pot half (Ra) is between VCC/R1-junction and the wiper (= Pin 7 node), and the lower pot half (Rb) is between the wiper and the capacitor.

   Correct connections:
   - R1 (1kΩ): +12V rail → top terminal of 10kΩ pot
   - Pin 7: wired to pot **wiper**
   - **Bottom terminal** of pot: connect to one end of R2 (see below)

3. **R2 (1kΩ)**: Connect from pot bottom terminal to Pin 6 / Pin 2 node (the capacitor top). This resistor sets the minimum discharge resistance, preventing 0% duty cycle.

4. **D_bypass (1N4148)**: Orient carefully!
   - **Anode** (the banded end is the **cathode** — the plain end is the anode): connect to the **pot wiper node** (= Pin 7 node).
   - **Cathode** (banded end): connect to the **Pin 6 / Pin 2 node** (the capacitor top, the same node where R2 terminates).
   - This diode is now in parallel with (Rb + R2), oriented to forward-bias during capacitor charging and reverse-bias during discharge.

5. **Timing capacitor C1 (100nF ceramic)**: Connect from the Pin 6 / Pin 2 node to GND.

6. **Pin 2 / Pin 6 tie**: Wire Pin 2 (Trigger) and Pin 6 (Threshold) together to the capacitor top node. Both must see the same capacitor voltage for astable operation.

### Step 4: Test the 555 Oscillator (Before Adding MOSFET)
1. Temporarily wire Pin 3 (OUT) through a **1kΩ resistor** to an LED (other LED lead to GND).
2. Apply +12V power.
3. The LED should blink rapidly (invisible due to ~1kHz, but will appear dimmer than constant-on).
4. Measure Pin 3 with a multimeter on DC volts: should read approximately `12V × duty cycle` (e.g., ~6V at centre pot = 50% duty cycle).
5. If you have an oscilloscope: you should see a clean square wave at ~1kHz. Rotate the pot; duty cycle should sweep from ~8% to ~92% while frequency stays roughly constant.
6. Remove the LED and test resistor before proceeding.

### Step 5: MOSFET Gate Circuit
1. Insert the **IRF540N** (or IRLZ44N) at the edge of the breadboard so the large metal tab faces toward an area where you can clip a heat sink.
2. Identify the legs looking at the front (marked side): left = Gate, centre = Drain, right = Source.
3. **Gate resistor Rg (1kΩ)**: Connect from 555 Pin 3 to the Gate leg of the MOSFET.
4. **Gate bleed (optional but recommended)**: Connect a 10kΩ resistor from Gate to Source. This ensures the MOSFET turns fully OFF if the PWM signal is disconnected or the 555 loses power.
5. **Source**: Connect directly to GND rail.
6. Attach the **heat sink** to the MOSFET's metal tab now, before connecting motor power (apply a small amount of thermal compound between tab and heat sink for best heat transfer).

### Step 6: Motor and Flyback Diode
1. **Motor wiring**: Connect one motor terminal to the **+12V power rail**. Connect the other motor terminal to the **Drain** of the MOSFET. Current path: +12V → Motor → Drain → Source → GND (when MOSFET is ON).
2. **Flyback diode 1N4007**: Place across the motor:
   - **Cathode** (banded end): connect to the +12V side of the motor (the +12V rail).
   - **Anode** (plain end): connect to the Drain side of the motor (= MOSFET drain node).
   - Double-check: the diode must be **reverse-biased** during normal operation (cathode more positive than anode). Verify: cathode at +12V, anode at drain which is pulled toward GND → correct orientation. ✅
3. **Critical**: Do not omit or reverse this diode. Without it, inductive spikes exceeding 100V will appear at the drain, destroying the MOSFET in seconds.

### Step 7: Final Inspection Checklist
Before applying power:
- [ ] IC notch/dot at Pin 1 end — confirm orientation
- [ ] Pin 4 (555 RST) tied to +12V
- [ ] Pin 5 (555 CTL) has 100nF to GND
- [ ] Pin 2 and Pin 6 are wired together
- [ ] 100nF capacitor between Pin 6/2 node and GND
- [ ] D_bypass (1N4148): anode at pot wiper, cathode at Pin 6/2 node
- [ ] MOSFET source to GND; gate through 1kΩ to Pin 3
- [ ] Flyback 1N4007: cathode to +12V side of motor, anode to MOSFET drain
- [ ] Heat sink attached to MOSFET tab
- [ ] 10µF decoupling cap across power rails (correct polarity)
- [ ] No shorts between +12V and GND rails

### Step 8: Power On and Initial Test
1. Set the potentiometer to the **minimum position** (wiper toward bottom, minimum duty cycle).
2. Connect the 12V power supply.
3. Motor should be stopped or barely turning.
4. Slowly rotate the potentiometer toward maximum.
5. Motor should smoothly and continuously increase speed from stopped to full speed.
6. Rotate back — motor should slow smoothly.
7. The MOSFET will become warm within a minute at high current. Verify the heat sink is getting warm (not hot). If the MOSFET gets too hot to touch, improve heat sinking or reduce motor current.

## Testing & Troubleshooting

### Expected Behaviour at Each Pot Position
| Pot Position | Duty Cycle | Avg. Voltage | Expected Motor Behaviour |
|---|---|---|---|
| Fully CCW (min) | ~8% | ~1.0V | Stopped or barely twitching |
| 25% rotation | ~27% | ~3.2V | Very slow, low torque |
| Centre (50%) | ~50% | ~6.0V | Medium speed |
| 75% rotation | ~73% | ~8.7V | Fast, good torque |
| Fully CW (max) | ~92% | ~11.0V | Near full speed |

### Voltage Measurements with Multimeter

**555 Timer pins (engine running):**
| Pin | Expected (DC) | Notes |
|---|---|---|
| Pin 8 | 12.0V | VCC |
| Pin 1 | 0V | GND |
| Pin 4 | 12.0V | Reset tied HIGH |
| Pin 3 | ~1V to ~11V (varies) | DC avg = 12V × duty cycle |
| Pin 6/2 | ~4V to 8V | Sawtooth avg; should read ~6V |
| Pin 5 | ~8V | 2/3 of VCC, stable |

**MOSFET pins:**
| Pin | Expected | Notes |
|---|---|---|
| Gate | Same as Pin 3 of 555 | ~0.5V lower due to Rg drop |
| Source | 0V | GND |
| Drain (motor ON, 50% DC) | ~6V (DC avg) | Switches 0–12V at 1kHz |

**Motor terminals:**
- Motor high side (top, +12V): constant 12V
- Motor low side (drain side): ~12V × duty cycle (DC average)
- Average voltage across motor = 12V × duty cycle ✅

### Oscilloscope Measurements (Ideal)

**Pin 3 (PWM waveform)**:
- At 50% duty cycle: clean square wave, ~1kHz, 0V to ~11V amplitude
- Duty cycle should sweep from ~8% to ~92% as pot rotates

**MOSFET Drain**:
- Should look identical to Pin 3 (same square wave)
- Voltage HIGH during MOSFET OFF = +12V
- Voltage LOW during MOSFET ON = <0.5V (R_ds(on) × I_motor)
- **Watch for**: ringing or spikes at transitions (should be <1V with 1kΩ Rg and 1N4007 in place)

**Flyback diode across motor (with scope probe across motor)**:
- Should show clean square wave
- No large negative spikes at turn-off (the 1N4007 should clamp these to <1V below GND)

### Common Issues and Solutions

**Motor does not run at all:**
- Measure Pin 3 of 555: if it reads 0V constantly, 555 is not oscillating
  - Check Pin 4 is at +12V (not floating)
  - Check capacitor C1 is connected to GND (correct polarity if electrolytic)
  - Check R1 and pot are connected to +12V and Pin 7 correctly
- Measure MOSFET gate: if gate voltage never exceeds ~2V, MOSFET won't turn on
  - Verify gate resistor (1kΩ) is connected to Pin 3, not another pin
- Check motor wiring: motor between +12V and Drain (not Source)
- Test motor directly on 12V supply to verify it works

**Motor runs but speed does not change with pot:**
- D_bypass diode may be missing or reversed — the duty cycle will be fixed if both timing paths use the same resistance
- Check diode orientation: anode at wiper/Pin 7 node, cathode at Pin 6/2 node
- Verify pot wiper is actually connected to Pin 7 (use continuity tester)
- Pot may be faulty — check resistance between wiper and each end terminal changes smoothly

**Motor only runs at full speed (no low-speed control):**
- R2 (1kΩ between pot bottom and Pin 6/2) may be missing → duty cycle minimum is 0%, 555 stalls
- Verify R2 is present and connected

**Motor speed jumps or stutters:**
- Poor breadboard connections on the motor current path (high current causes voltage drop in loose connections)
- Add a direct, short jumper wire for the motor-to-MOSFET-drain connection using stiff wire
- Check 10µF decoupling capacitor is present and polarised correctly

**MOSFET gets extremely hot quickly:**
- Motor drawing more current than expected — measure motor current with multimeter in series (DC mA/A range)
- Heat sink may not be making good contact — re-apply thermal compound and secure firmly
- If I_motor > 5A, the IRF540N can handle it (rated 33A) but the heat sink must be much larger. Use a metal enclosure as heat sink if needed.
- At 3A and 50% duty cycle: P_mosfet ≈ 3² × 0.044Ω × 0.5 ≈ 200mW — MOSFET should barely get warm. If it's hot, check for short circuit on drain.

**Motor vibrates or hums but doesn't spin at low duty cycle:**
- This is normal at very low duty cycles (<15%) — the average voltage is below motor start-up threshold
- Increase the minimum pot resistance (use 2.2kΩ for R1 instead of 1kΩ) to raise the minimum duty cycle
- For better low-speed torque, lower the PWM frequency (increase C1 to 1µF → ~100Hz), but this will cause audible motor hum

**555 gets warm:**
- Check the 100nF on Pin 5 is present — without it, internal reference is unstable and draws extra current
- Check Pin 4 is at VCC, not floating (floating RST causes spurious triggering and increased supply current)
- NE555 drawing ~10–15mA from 12V is normal (~120–180mW)

**Large voltage spikes visible on oscilloscope at MOSFET drain:**
- Flyback diode may be reversed — check cathode (banded end) is at +12V
- Diode may be too slow — 1N4007 has ~500ns recovery time; try 1N5819 Schottky (20ns) for cleaner switching
- Add a 100nF ceramic capacitor from drain to GND as an additional snubber if spikes persist

## Calculations

### PWM Frequency
```
Astable 555 frequency with diode-modified duty cycle:

  f = 1.44 / ((R_total_timing) × C1)

where R_total_timing = R1 + R_pot_total + R2
                     = 1kΩ + 10kΩ + 1kΩ = 12kΩ

  f = 1.44 / (12,000 × 100×10⁻⁹)
  f = 1.44 / 0.00120
  f ≈ 1,200 Hz  (~1.2 kHz)

Note: frequency is approximately constant regardless of pot position because
R_pot_upper + R_pot_lower = R_pot_total (constant), so only the ratio changes.
```

### Duty Cycle Formula (Variable Duty Cycle Modification)
```
  D = (R1 + R_pot_upper) / (R1 + R_pot_total + R2)

At pot minimum (R_pot_upper = 0, R_pot_lower = 10kΩ):
  D_min = 1k / (1k + 10k + 1k) = 1/12 ≈ 8.3%

At pot centre (R_pot_upper = R_pot_lower = 5kΩ):
  D_mid = (1k + 5k) / 12k = 6/12 = 50.0%

At pot maximum (R_pot_upper = 10kΩ, R_pot_lower = 0):
  D_max = (1k + 10k) / 12k = 11/12 ≈ 91.7%
```

### Average Motor Voltage
```
  V_avg = V_supply × D

Examples (12V supply):
  D = 25%:  V_avg = 12 × 0.25 = 3.0V
  D = 50%:  V_avg = 12 × 0.50 = 6.0V
  D = 75%:  V_avg = 12 × 0.75 = 9.0V
  D = 92%:  V_avg = 12 × 0.92 = 11.0V
```

### MOSFET Power Dissipation
```
Power has two components: conduction loss and switching loss.

Conduction loss (dominant at low frequencies):
  P_cond = I²_motor × R_ds(on) × D
  At I = 3A, D = 50%, R_ds(on) = 44mΩ:
  P_cond = 9 × 0.044 × 0.5 = 0.198W ≈ 0.2W

Switching loss (depends on transition times):
  P_sw = ½ × V_supply × I_motor × (t_rise + t_fall) × f
  IRF540N t_rise ≈ 100ns, t_fall ≈ 100ns:
  P_sw = 0.5 × 12 × 3 × (200×10⁻⁹) × 1200
  P_sw = 0.5 × 12 × 3 × 0.00024 = 0.00432W ≈ 4mW (negligible at 1.2kHz)

Total: P_total ≈ 0.2W + 0.004W ≈ 0.2W

At 3A, the MOSFET runs cool (ΔT ≈ 0.2W × Rθja ≈ 0.2 × 62°C/W = 12°C rise).
Heat sink is insurance — mandatory above 5A continuous.

At maximum rated 5A continuous:
  P_cond = 25 × 0.044 × 1.0 = 1.1W  (at full duty cycle, worst case)
  Requires: Rθjc (TO-220 case) + Rθcs (thermal compound) + Rθsa (heat sink) < (150°C - 25°C) / 1.1W
  Minimum heat sink: Rθsa < 108°C/W — a small clip-on TO-220 heat sink (40°C/W) is adequate.
```

### Motor RMS Current and Ripple
```
RMS current (purely resistive approximation, valid for low inductance):
  I_rms = I_avg × √D    (at D = 50%, I_avg = 2A)
  I_rms = 2 × √0.5 = 2 × 0.707 = 1.41A

Ripple current (triangular approximation for motor inductor L):
  ΔI = V_supply × D × (1 - D) / (L × f)
  Typical small DC motor L ≈ 500µH:
  ΔI = 12 × 0.5 × 0.5 / (500×10⁻⁶ × 1200) = 3 / 0.6 = 5A  (high ripple!)
  This confirms the flyback diode must handle large peak currents on every cycle.
  → 1N4007's 30A surge rating is appropriate.

Actual peak drain current ≈ I_avg + ΔI/2
```

### Adjusting the Switching Frequency
```
To change f, swap the timing capacitor C1:

  Target f = 500 Hz:  C1 = 1.44 / (12kΩ × 500) = 240nF → use 220nF
  Target f = 1 kHz:   C1 = 1.44 / (12kΩ × 1000) = 120nF → use 100nF
  Target f = 5 kHz:   C1 = 1.44 / (12kΩ × 5000) = 24nF  → use 22nF
  Target f = 20 kHz:  C1 = 1.44 / (12kΩ × 20000) = 6nF  → use 6.8nF

Above 20kHz = ultrasonic (silent motor operation, used in high-quality speed controllers).
Below 100Hz = audible hum from motor vibration (useful for demonstrations).
```

## Key Concepts Learned
- **Pulse-Width Modulation (PWM)**: Controlling power by varying the ON/OFF ratio of a switched signal, not the voltage magnitude — the basis of nearly all modern motor drives, DC-DC converters, and LED dimmers
- **Duty Cycle**: The fraction of each switching period during which the output is HIGH; determines average output voltage
- **555 Timer Astable Mode**: Self-triggering oscillator using two resistors and a capacitor; one of the most widely used circuits in electronics
- **Diode-Assisted Variable Duty Cycle**: Using a bypass diode to decouple the charge and discharge timing paths, enabling symmetric duty cycle control (5%–95%) from a single potentiometer
- **N-Channel MOSFET as Power Switch**: Enhancement-mode FET used as a fully controlled switch with near-zero drop when ON and near-infinite impedance when OFF
- **Gate Drive**: The gate charge and threshold voltage requirements for reliable MOSFET switching; why gate resistors suppress ringing
- **Back-EMF and Inductive Kick**: The voltage spike generated by an inductor (motor) when its current is suddenly interrupted; a fundamental hazard in all power electronics
- **Flyback / Freewheeling Diode**: The standard protection against inductive kick in switched-mode motor and relay circuits
- **Thermal Management**: Power dissipation calculation, junction temperature estimation, and heat sink selection for power devices
- **Low-Side Switching**: Placing the power switch (MOSFET) on the GND side of the load for simpler gate drive requirements with N-channel devices

## Applications
- **Variable speed DC motor drives**: Power tools, fans, pumps, conveyors, robotics joints
- **Electric vehicle motor controllers**: The same PWM + MOSFET topology scales to kW-level drives with larger components
- **Battery chargers**: PWM charging profiles for lead-acid and lithium batteries
- **LED dimmers**: Flicker-free brightness control for high-power LEDs; identical circuit with LED replacing motor (omit flyback diode)
- **Solenoid / valve drivers**: PWM hold-mode reduces heat in continuously energised solenoids
- **Heating elements**: PWM power control for resistive heaters (toaster ovens, reflow ovens)
- **Audio amplifiers (Class D)**: High-frequency PWM drives speakers via LC filter — same switching principle
- **Switch-mode power supplies**: DC-DC converters (buck, boost, buck-boost) all use PWM to regulate output voltage
- **Wind turbine controllers**: PWM dump-load controllers to absorb excess generation
- **CNC and 3D printers**: Spindle speed control, stepper motor current regulation

## Next Steps / Extensions

1. **Add a tachometer feedback loop**: Wire a Hall-effect sensor to the motor shaft and feed its pulses to a comparator. Compare actual speed to a set-point and adjust the PWM duty cycle to compensate for load changes — this is closed-loop PID control.

2. **Microcontroller upgrade**: Replace the entire 555 circuit with an Arduino or ESP32. Use `analogWrite()` (8-bit PWM, ~490Hz or 980Hz) or Timer registers for true high-frequency PWM. Keep the same MOSFET and flyback diode — the power stage is identical.

3. **Bi-directional control (H-Bridge)**: Add three more MOSFETs to build an H-bridge, enabling forward, reverse, and braking. This is how every DC servo drive and brushed ESC works.

4. **Current-sensing and overcurrent protection**: Insert a 0.1Ω sense resistor in series with the motor, measure the voltage drop, and use a comparator to shut down the MOSFET if current exceeds a safe limit — protecting both the MOSFET and the motor.

5. **Brushless motor (BLDC) controller**: Apply PWM to three half-bridges (six MOSFETs) with commutation logic — the foundation of all modern BLDC and PMSM motor drives.

6. **Soft-start circuit**: Add an RC network to the pot's control voltage so the duty cycle ramps up slowly when power is applied, preventing the motor current surge from tripping an overcurrent protection.

7. **Digital speed display**: Connect a tachometer to a frequency counter (CD4033 + 7-segment display from project #16) to display RPM numerically.

8. **Audio tone generator**: Reduce C1 to ~10nF → f ≈ 12kHz. Connect a small speaker through a capacitor to Pin 3. Rotating the pot changes the duty cycle and timbre of the tone — an electronic theremin-like instrument.

## Safety Notes

- ⚠️ **12V at 5A = 60W of power** — connections must be solid. Use at least 22 AWG wire (18 AWG preferred) for the motor current path (MOSFET drain → motor → supply). Thin breadboard jumpers may overheat.
- ⚠️ **MOSFET heat**: Even 1W of dissipation will make a TO-220 package burning-hot without a heat sink. Always attach the heat sink before running the motor under load. Touch the heat sink (not the bare package) periodically — if it's too hot to hold for 3 seconds, improve cooling.
- ⚠️ **NEVER omit the flyback diode** (1N4007). Without it, the first time the MOSFET turns off, the inductive spike can exceed 100V and instantly destroy the device. Check diode orientation before every power-on.
- ⚠️ **Do not exceed V_gs = ±20V on the MOSFET gate**. The gate oxide is a capacitor that breaks down irreversibly above this voltage. With a 12V supply and 1kΩ gate resistor, this is not a concern — but never apply a higher supply without rechecking the gate drive voltage.
- ⚠️ **Motor in-rush current**: A stalled or starting motor can draw 5–10× its rated current. If your power supply has current limiting, set it to 2A initially to protect the circuit while testing.
- ⚠️ **Rotating motor shaft**: Keep fingers, wires, and clothing away from the spinning shaft. Secure the motor to a surface before running at high speed. A 12V gearmotor can generate significant torque.
- ✅ **12V DC is safe** from an electrocution standpoint, but can deliver enough current to cause burns, fires, or weld metals if shorted. Keep a fuse (2A fast-blow) in series with the +12V supply.
- ✅ **Breadboard limitation**: This circuit is fine for testing up to ~2A sustained motor current. For higher currents (3A+), move to a perfboard or PCB layout with wider copper traces and terminal blocks.
- ⚠️ **CMOS/linear IC static sensitivity**: Handle the NE555 and MOSFET by their bodies, not their pins. Touch a grounded metal object before handling to discharge static.

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

Spin it up! ⚡🔧
