# Temperature-Controlled Fan (Analog) ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - Multi-component analog design with power transistor switching and op-amp comparator

## What You'll Learn
- Analog temperature sensing with the LM35 precision sensor
- Op-amp comparator circuits and voltage comparison
- Power transistor switching for inductive DC loads
- Flyback diode protection for motor circuits
- Hysteresis design to prevent output chattering
- Threshold voltage calculation and adjustment
- Managing dual supply voltages in a single circuit
- Heat dissipation and thermal considerations for power components

## Components Required
- **LM35** Precision Centigrade Temperature Sensor × 1
- **LM358** Dual Operational Amplifier IC × 1
- **TIP122** NPN Darlington Power Transistor × 1
- **12V DC Fan** (small brushed motor type, 50–300mA) × 1
- **Potentiometer** 10kΩ (threshold adjustment) × 1
- **Resistors**:
  - 1kΩ × 1 (transistor base current limiting)
  - 10kΩ × 2 (op-amp input bias and voltage divider)
  - 100kΩ × 1 (hysteresis feedback — optional but recommended)
- **Capacitors**:
  - 100nF ceramic × 2 (power supply decoupling)
- **Diode** 1N4007 × 1 (flyback/freewheeling protection)
- **Heat sink** for TIP122 (TO-220 clip-on type)
- **12V DC power supply** (wall adapter or bench supply, ≥1A)
- **5V regulated supply** (for LM35/LM358 logic side — 7805 regulator or USB power bank)
- **Breadboard** and jumper wires
- **Multimeter**

## Circuit Description
Build an automatic temperature-controlled fan that switches on whenever the ambient temperature rises above a user-adjustable threshold! The LM35 temperature sensor produces a voltage precisely proportional to temperature (10mV per °C). An LM358 op-amp is wired as a comparator: it continuously compares the LM35 output against a reference voltage set by the potentiometer. When the temperature voltage exceeds the threshold, the op-amp output swings high, forward-biasing the TIP122 Darlington power transistor, which then switches the 12V fan motor on. A 1N4007 flyback diode clamps the inductive kick from the motor, protecting the transistor. Turn the potentiometer to set any trigger temperature from roughly 0°C to 50°C. This project teaches core industrial control concepts: sensing, comparison, threshold detection, and high-current switching — all without a microcontroller!

## Theory of Operation

### Temperature Sensing: The LM35
The **LM35** is a precision analog temperature sensor calibrated directly in Celsius. Its output voltage increases linearly at exactly **10mV per °C**:

```
Vout = 10mV × T(°C)

Examples:
  0°C  →   0mV
 20°C  → 200mV
 25°C  → 250mV
 30°C  → 300mV
 40°C  → 400mV
 50°C  → 500mV
100°C  → 1000mV (1V)
```

The LM35 requires no external calibration and operates from 4V to 30V. At room temperature (~25°C) it outputs a stable 250mV signal that slowly rises as you warm the sensor with your fingers or a heat source.

### Comparator Circuit: The LM358 Op-Amp
The **LM358** is a general-purpose dual op-amp powered from the 5V logic rail. One amplifier is configured as an **open-loop voltage comparator**:

- **Non-inverting input (+, pin 3)**: Connected to the LM35 output — the "actual temperature" voltage
- **Inverting input (−, pin 2)**: Connected to the wiper of the 10kΩ potentiometer — the "threshold" voltage
- **Output (pin 1)**: Drives the transistor base through a 1kΩ resistor

The rule of a comparator is simple:
- If V(+) > V(−): Output swings **HIGH** → Fan turns **ON**
- If V(+) < V(−): Output swings **LOW** → Fan stays **OFF**

Because the LM358 output is open-collector-like and can only swing to within ~1.5V of its supply (so ~3.5V on a 5V rail), this is sufficient to saturate the TIP122.

### Threshold Voltage: The Potentiometer
The 10kΩ potentiometer forms a voltage divider between 5V and GND. Rotating the wiper adjusts the reference voltage from 0V to ~5V:

```
Vthreshold = 5V × (Rpot_lower / 10kΩ)
```

To set the fan to trigger at 30°C (300mV from LM35), adjust the pot so the wiper reads **300mV**. This gives you real-time threshold tuning without changing any components. A practical adjustment range covers roughly 0°C to 50°C.

### Transistor Switching: The TIP122
The **TIP122** is a Darlington NPN power transistor packaged in TO-220. It consists of two cascaded NPN transistors internally, giving a combined current gain (hFE) of typically **1000 or more**. This means a tiny base current of just a few milliamps can switch several amps at the collector.

Circuit operation:
1. LM358 output goes HIGH (~3.5V)
2. Current flows through the 1kΩ base resistor into the TIP122 base
3. Transistor saturates: collector (connected to fan −) pulls to near GND
4. 12V fan completes its circuit through the transistor → **fan spins**
5. LM358 output goes LOW (near 0V)
6. No base current → transistor cuts off → fan stops

The TIP122 has two internal base-emitter junctions (Darlington pair), so its Vbe drop is approximately **1.4V** (rather than the 0.7V of a single transistor).

### Flyback Diode: The 1N4007
DC fan motors contain **inductive windings**. When the transistor switches the fan **off**, the collapsing magnetic field in the motor coils generates a large voltage spike — potentially hundreds of volts in the reverse direction. This spike would instantly destroy the TIP122 without protection.

The **1N4007** flyback diode is placed in **reverse bias** across the fan (cathode to 12V, anode to fan−/collector side). When the motor spike appears, the diode instantly conducts, clamping the voltage and safely dissipating the energy back into the supply rail. This is sometimes called a "snubber" or "freewheeling" diode and is **essential** in any circuit driving inductive loads (motors, relays, solenoids).

### Hysteresis: Preventing Chattering
Without hysteresis, when the temperature is right at the threshold voltage, the comparator output rapidly oscillates between HIGH and LOW — causing the fan to chatter on and off many times per second. This is noisy, mechanically stressful on the fan, and electrically damaging.

**Hysteresis** solves this by introducing a small amount of **positive feedback**: a 100kΩ resistor connects the comparator output back to the non-inverting input (+). This creates a small dead band:

- When output is HIGH, it slightly raises the + input → threshold must drop further below before output goes LOW (hysteresis below trigger)
- When output is LOW, it slightly lowers the + input → temperature must rise further above threshold before output goes HIGH again

The result: the fan turns **on** at a slightly higher temperature than it turns **off** — a clean, decisive switching action with no chattering. This is standard industrial practice for all comparator-based controllers.

```
Hysteresis band ≈ Vhysteresis = Vswing × (Rfeedback / (Rfeedback ∥ Rpot))

With 100kΩ feedback and 10kΩ pot (midpoint ~5kΩ output impedance):
Vhysteresis ≈ 3.5V × (5kΩ / 100kΩ) ≈ 175mV → ~17°C hysteresis band

Reduce to 470kΩ feedback resistor for ~3–4°C band (more practical for room control)
```

### Dual Supply Rails
This circuit uses **two voltage rails**:
- **5V** for the LM35 sensor and LM358 op-amp (logic/sensing side)
- **12V** for the fan motor (power side)

Both share a **common GND**. The TIP122 bridges the two rails: its base is driven by the 5V logic output, while its collector switches the 12V fan load. Never connect the 12V rail to the 5V components directly.

## Pin Configuration

### LM35 Temperature Sensor (TO-92 package)
```
   Flat face forward:

     [ LM35 ]
      | | |
      1 2 3

  Pin 1: +VS  (power supply: +4V to +30V)
  Pin 2: Vout (analog output: 10mV/°C)
  Pin 3: GND  (ground)
```
⚠️ **Important**: TO-92 packages can look like NPN transistors. Verify markings before inserting!

### LM358 Dual Op-Amp (8-pin DIP)
```
       +--U--+
  OUT1 |1   8| VCC (+5V)
   IN1-|2   7| OUT2
   IN1+|3   6| IN2-
   GND |4   5| IN2+
       +-----+
```
- **We use Amplifier 1** (pins 1, 2, 3) as the comparator
- Pin 3 (IN1+): LM35 Vout (temperature signal)
- Pin 2 (IN1−): Potentiometer wiper (threshold reference)
- Pin 1 (OUT1): Comparator output → 1kΩ → TIP122 base
- Amplifier 2 (pins 5–7) can be used for hysteresis feedback or left unused (tie inputs to GND/VCC)

### TIP122 NPN Darlington Transistor (TO-220 package)
```
   Front face (metal tab = heatsink side, faces away):

   |  TIP122  |
   |__________|
     | | |
     B C E

  Pin 1 (left):   Base      — from 1kΩ resistor (LM358 output)
  Pin 2 (center): Collector — to fan negative terminal
  Pin 3 (right):  Emitter   — to GND

  Metal tab: Collector (same as pin 2) — attach heatsink here
```
⚠️ Always attach a heatsink to the TIP122 metal tab. Without one, the transistor will overheat driving fans above ~200mA.

### 1N4007 Flyback Diode
```
  Cathode (stripe) → 12V fan positive terminal
  Anode            → Fan negative / TIP122 collector

  Direction: current flows from anode to cathode when conducting
  In normal operation: REVERSE BIASED (blocking)
  During motor spike: FORWARD BIASED (clamping)
```

## Building Instructions

### Step 1: Power Rails and Decoupling
1. Connect a 12V DC power supply to the far-right power rails of your breadboard (+12V and GND)
2. Connect a 5V supply to the far-left power rails (+5V and GND)
3. **Critical**: Connect the two GND rails together with a jumper wire — common ground
4. Add a 100nF ceramic capacitor across the 5V rail (as close to the LM358 as possible)
5. Add a second 100nF ceramic capacitor across the 12V rail
6. Verify voltages with a multimeter: left rail = 5V, right rail = 12V, both sharing 0V GND

### Step 2: LM35 Temperature Sensor
1. Identify the flat face and pin numbers (1=+VS, 2=Vout, 3=GND)
2. Insert LM35 near the center-left of the breadboard
3. **Pin 1** → 5V rail
4. **Pin 3** → GND rail
5. **Pin 2** (Vout) → a free node (this goes to op-amp in Step 4)
6. Optionally add a 10kΩ resistor from Pin 2 to GND to stabilize output (prevents floating at power-on)
7. Measure: at room temperature (~25°C) Pin 2 should read ~250mV

### Step 3: Threshold Potentiometer
1. Insert the 10kΩ potentiometer in the breadboard
2. Connect one outer terminal to the **5V rail**
3. Connect the other outer terminal to **GND**
4. The **wiper** (center pin) goes to a free node — this is your threshold voltage
5. Turn the pot fully counterclockwise → wiper reads ~0V
6. Turn fully clockwise → wiper reads ~5V
7. Set the pot to midpoint (~2.5V) for now; you will calibrate later

### Step 4: LM358 Op-Amp Comparator
1. Insert LM358 IC on the breadboard (note the notch/dot at pin 1)
2. **Pin 8** (VCC) → 5V rail
3. **Pin 4** (GND) → GND rail
4. **Pin 3** (IN1+, non-inverting) → LM35 Vout node (from Step 2)
5. **Pin 2** (IN1−, inverting) → Potentiometer wiper node (from Step 3)
6. Leave pins 5, 6, 7 (amplifier 2) unconnected for now, or tie pin 5 to GND and pin 6 to VCC to keep unused amp stable
7. Add the 100nF decoupling cap from pin 8 to GND if not already placed

### Step 5: Hysteresis Feedback (Recommended)
1. Connect a 470kΩ resistor (or 100kΩ for wider band) from **LM358 Pin 1 (OUT1)** back to **Pin 3 (IN1+)**
2. This creates the positive feedback loop for hysteresis
3. Without this, the fan chatters when temperature is near threshold

### Step 6: TIP122 Transistor
1. Attach a TO-220 clip-on heatsink to the metal tab of the TIP122
2. Insert TIP122 on the right side of the breadboard (the 12V power section)
3. Identify pins: Base (left), Collector (center), Emitter (right) — with flat face toward you
4. **Emitter (pin 3)** → GND rail (the common ground)
5. **Base (pin 1)** → one end of the 1kΩ resistor
6. Other end of 1kΩ resistor → LM358 Pin 1 (OUT1)
7. Also add a 10kΩ resistor from the Base to GND — this ensures the transistor stays off when the op-amp output is low or floating at power-up

### Step 7: Fan and Flyback Diode
1. Connect the fan **positive (red) wire** → 12V rail
2. Connect the fan **negative (black) wire** → TIP122 Collector (center pin)
3. **Flyback diode (1N4007)**:
   - Cathode (stripe end) → 12V rail (same node as fan positive)
   - Anode → TIP122 Collector (same node as fan negative)
   - The diode sits in **parallel with the fan**, reversed — correct!
4. Double-check diode orientation: stripe toward 12V

### Step 8: Wiring Verification Checklist
Before powering on, verify each connection:
- [ ] LM35 Pin 1 → 5V, Pin 3 → GND, Pin 2 → LM358 Pin 3
- [ ] Pot outer pins to 5V and GND; wiper to LM358 Pin 2
- [ ] LM358 Pin 8 → 5V, Pin 4 → GND
- [ ] 1kΩ resistor: LM358 Pin 1 → TIP122 Base
- [ ] 10kΩ resistor: TIP122 Base → GND
- [ ] TIP122 Emitter → GND
- [ ] Fan positive → 12V, Fan negative → TIP122 Collector
- [ ] 1N4007: cathode to 12V, anode to TIP122 Collector
- [ ] 5V GND and 12V GND connected together
- [ ] Heatsink on TIP122
- [ ] Both 100nF decoupling caps present

### Step 9: Threshold Calibration
1. Power on both supplies
2. Measure LM35 output with multimeter (should be ~250mV at room temp)
3. Turn the pot until the wiper reads ~50mV **above** the LM35 reading (fan should be OFF)
4. Slowly warm the LM35 by holding it between your fingers or blowing warm air on it
5. Watch the LM35 voltage rise — when it passes the pot threshold, the fan should turn on
6. Release the sensor — as it cools, the fan should turn off (with hysteresis, slightly below where it turned on)
7. Adjust pot to your desired trigger temperature

## Testing & Troubleshooting

### Expected Behavior

| Condition | LM35 Output | Comparator Output | Fan |
|---|---|---|---|
| Cold (below threshold) | < Vpot | ~0V (LOW) | OFF |
| Temperature rising, near threshold | Approaching Vpot | LOW (hysteresis) | OFF |
| Temperature at threshold | = Vpot | Switches HIGH | Turns ON |
| Hot (above threshold) | > Vpot | ~3.5V (HIGH) | ON |
| Temperature falling (hysteresis band) | Slightly below Vpot | Still HIGH | Still ON |
| Temperature falls below lower hysteresis | Well below Vpot | Switches LOW | Turns OFF |

### Voltage Measurements

**At room temperature (~25°C), pot set to ~300mV (30°C threshold):**

| Test Point | Expected Voltage | Notes |
|---|---|---|
| LM35 Pin 1 | 5.0V | Power supply |
| LM35 Pin 2 (Vout) | ~250mV | 25°C × 10mV/°C |
| LM35 Pin 3 | 0V | GND |
| LM358 Pin 8 | 5.0V | VCC |
| LM358 Pin 4 | 0V | GND |
| LM358 Pin 3 (IN+) | ~250mV | Same as LM35 Vout |
| LM358 Pin 2 (IN−) | ~300mV | Pot threshold |
| LM358 Pin 1 (OUT) | ~0V | IN+ < IN− → LOW |
| TIP122 Base | ~0V | No drive |
| TIP122 Collector | 12V | Transistor OFF, fan at 12V |
| TIP122 Emitter | 0V | GND |
| Fan + (anode) | 12V | — |
| Fan − (cathode to transistor) | 12V | No current (open switch) |

**When temperature exceeds threshold (LM35 > 300mV):**

| Test Point | Expected Voltage | Notes |
|---|---|---|
| LM35 Pin 2 (Vout) | >300mV | e.g. 350mV at 35°C |
| LM358 Pin 1 (OUT) | ~3.5V | HIGH (near VCC) |
| TIP122 Base | ~2.5V | After 1kΩ resistor drop |
| TIP122 Collector | ~0.5–1V | Saturated (fan drawing current) |
| Fan current | varies | 50–300mA typical for small fan |

### Common Issues and Solutions

**Fan never turns on:**
- Measure LM35 Vout — is it increasing when you warm the sensor? (~10mV per °C change)
- Measure LM358 Pin 3 — should track LM35 Vout
- Measure LM358 Pin 2 — is it set above Pin 3 voltage? Lower the pot wiper
- Measure LM358 Pin 1 — if Pin 3 > Pin 2 but output stays low, LM358 may be wired wrong or damaged
- Check LM358 VCC (Pin 8 = 5V) and GND (Pin 4 = 0V)
- Verify TIP122 pin orientation — it is easy to insert with Base and Emitter reversed

**Fan always on (won't turn off):**
- Pot threshold may be set too low (below LM35 room-temperature voltage) — raise it
- LM358 IN+ and IN− may be swapped — check pin 2 vs pin 3 connections
- TIP122 may be shorted between collector and emitter (damaged) — test transistor with multimeter diode mode
- Measure LM358 Pin 1: if it reads near 0V but fan is on, transistor is stuck on — replace TIP122

**Fan chatters (rapid on/off):**
- Hysteresis feedback resistor missing or too large — add or reduce the feedback resistor from Pin 1 to Pin 3
- LM358 inputs may have noise on them — add 100nF cap from Pin 3 to GND to filter LM35 signal
- Pot wiper may be noisy — wiggle pot; if chatter changes, try a different potentiometer

**TIP122 gets very hot quickly:**
- Missing heatsink — attach clip-on TO-220 heatsink immediately
- Fan current exceeds 500mA — measure fan current with series ammeter; use a higher-rated transistor or logic-level MOSFET (e.g. IRF530) for larger fans
- Transistor is not fully saturating — check base current is sufficient (see Calculations section)

**LM35 reading seems wrong or drifting:**
- Verify correct pin orientation — reversed LM35 will get hot and read wildly
- Add 100nF capacitor from LM35 Vout to GND to filter noise
- Add 75–100Ω series resistor between LM35 Vout and load if driving long wires (prevents parasitic oscillation)
- Check for nearby heat sources (power supply, TIP122 heatsink) that may affect reading

**No voltage from LM358 output:**
- LM358 output is LOW by design when IN+ < IN− — confirm this is the real condition
- If IN+ > IN− and output is still LOW: LM358 input or output stage damaged — replace IC
- Make sure LM358 is not wired with a 12V supply — it operates from the **5V rail**

**Fan makes buzzing sound but barely spins:**
- 12V supply current-limited — use a supply rated for at least the fan's stall current (check fan label)
- Flyback diode reversed — verify 1N4007 stripe (cathode) is toward 12V, not toward collector
- Fan motor may require higher starting voltage — this circuit is full-on/off; at 12V a properly wired fan should start cleanly

**Sparks or pop on power-on:**
- Large electrolytic capacitor on 12V rail will cause inrush current — normal for power supplies
- Indicates 12V and 5V GND are not connected — check common ground jumper

## Calculations

### Temperature-to-Voltage Conversion (LM35)
```
Vout = 10mV/°C × T(°C)

To find temperature from voltage:
T(°C) = Vout(mV) / 10

Threshold temperature for a given pot setting:
T_trigger(°C) = Vpot(mV) / 10

Example: Vpot = 280mV → triggers at 28°C
Example: Vpot = 350mV → triggers at 35°C
```

### Potentiometer Threshold Voltage
```
Vpot = VCC × (Rwiper_lower / Rpot_total)
     = 5V × (Rwiper_lower / 10kΩ)

Range: 0V (wiper at GND end) to 5V (wiper at VCC end)
Practical range for 0–50°C: 0mV to 500mV
  (only the lower 10% of pot rotation covers this range)

Tip: Use a 1kΩ resistor in series with the lower pot terminal
to limit minimum to ~0.5V and improve fine-tuning resolution:
  Vpot_min = 5V × (1kΩ / 11kΩ) ≈ 455mV → ~45°C minimum threshold
  → swap to use upper half of pot travel for 0–45°C range
```

### Transistor Base Current Calculation
```
The TIP122 needs its base driven to saturate (fully on):

Vout_LM358 ≈ 3.5V (at 5V supply)
Vbe_TIP122 ≈ 1.4V (Darlington = 2 × 0.7V)

Ib = (Vout - Vbe) / Rbase
   = (3.5V - 1.4V) / 1kΩ
   = 2.1V / 1kΩ
   = 2.1mA

Required base current for saturation:
  Ic_fan = 200mA (typical small fan)
  hFE_min = 1000 (TIP122 datasheet minimum)
  Ib_required = Ic / hFE = 200mA / 1000 = 0.2mA

Actual 2.1mA >> 0.2mA required → transistor fully saturated ✅
Safety factor: 10×

For a larger fan at 500mA:
  Ib_required = 500mA / 1000 = 0.5mA → still satisfied ✅
```

### Hysteresis Band Calculation
```
With feedback resistor Rf from OUT (pin 1) to IN+ (pin 3),
and pot output impedance Rp (≈ Rpot/4 at midpoint = 2.5kΩ):

Vhysteresis ≈ Vswing × Rp / Rf

With Rf = 470kΩ, Rp = 2.5kΩ, Vswing = 3.5V:
  Vhysteresis ≈ 3.5V × (2.5kΩ / 470kΩ) ≈ 18mV → ~1.8°C band

With Rf = 100kΩ:
  Vhysteresis ≈ 3.5V × (2.5kΩ / 100kΩ) ≈ 87mV → ~8.7°C band

Choose Rf to suit application:
  Tight control (HVAC): 470kΩ → ~2°C band
  Protective cooling: 100kΩ → ~9°C band
```

### Power Dissipation in TIP122
```
When transistor is saturated (fan on):
  Vce_sat ≈ 1.0V (datasheet typical at Ic = 200mA)
  P_transistor = Vce_sat × Ic = 1.0V × 200mA = 200mW

At 500mA fan current:
  Vce_sat ≈ 2.0V (Darlington higher than single transistor)
  P_transistor = 2.0V × 500mA = 1000mW = 1W

TIP122 max rating: 65W (with adequate heatsink)
A small clip-on heatsink handles up to ~2W comfortably.
Always use a heatsink for fan currents above 100mA.
```

## Key Concepts Learned
- **Analog sensing**: Converting a physical quantity (temperature) to a proportional voltage
- **Comparator operation**: Op-amp as a voltage comparator without feedback (open-loop)
- **Threshold detection**: Triggering an action when a signal crosses a reference level
- **Hysteresis**: Positive feedback to create a dead band and prevent output oscillation
- **Darlington transistor**: High-gain two-transistor configuration for switching larger loads
- **Inductive load switching**: Why motors need flyback diodes and how they work
- **Dual supply design**: Separate logic and power supply rails sharing common ground
- **Power dissipation**: Calculating and managing heat in power components
- **Analog control without microcontrollers**: Pure hardware implementation of a control loop

## Applications
- **Computer cooling**: Automatic CPU/GPU fan control based on heatsink temperature
- **Greenhouse climate control**: Ventilation fan triggered by rising ambient temperature
- **Server rack cooling**: Temperature-activated airflow management
- **Battery charging**: Fan cooling for lithium battery packs during charge cycles
- **3D printer enclosures**: Automatic exhaust when print chamber overheats
- **Automotive cooling**: Auxiliary fan control for engine bay or cabin
- **Industrial equipment**: Protecting motors and drives from thermal runaway
- **Incubators**: Maintaining stable temperature through ventilation
- **Home automation**: Smart ventilation fan integrated into HVAC control

## Next Steps / Extensions
1. **Add LED indicators**: Green LED for "cold/fan off," Red LED for "hot/fan on" — use comparator second output stage (LM358 has two op-amps)
2. **PWM fan speed control**: Replace on/off switching with a 555 timer PWM circuit to vary fan speed proportionally with temperature (instead of binary on/off)
3. **Temperature display**: Add an LM3914 bar graph driver to display temperature visually across 10 LEDs
4. **Dual threshold**: Use both LM358 op-amps — one for "low speed" fan trigger, one for "high speed" at a higher temperature
5. **Arduino upgrade**: Replace the analog comparator with Arduino and a sketch — add serial temperature logging, LCD display, and multiple fan zones
6. **MOSFET upgrade**: Replace TIP122 with an N-channel MOSFET (e.g. IRF530 or IRLZ44N) for lower on-resistance, less heat, and faster switching
7. **Over-temperature alarm**: Add a third comparator threshold that triggers a buzzer if temperature rises dangerously high (component overheating detection)
8. **Closed-loop PID control**: Graduate to a proportional-integral-derivative controller for precise temperature regulation — requires microcontroller
9. **AC fan control**: Add a TRIAC and zero-crossing detector to control a 120/240V AC fan (high-voltage skills required — advanced project)

## Safety Notes
- ⚠️ **12V power supply**: While 12V DC is not life-threatening, short circuits can deliver dangerous currents. Always power down before rewiring
- ⚠️ **Fan blades**: Keep fingers, hair, and wires away from spinning fan. Small fans can still cut skin
- ⚠️ **Heatsink required**: The TIP122 WILL overheat without a heatsink. A hot transistor can melt breadboard plastic, burn fingers, and fail destructively
- ⚠️ **Flyback diode is not optional**: Omitting the 1N4007 WILL destroy the TIP122 the first time the fan turns off. Always include it when switching any inductive load
- ⚠️ **LM35 polarity**: Reversed LM35 passes full supply current through wrong pins, gets hot immediately, and may be destroyed. Verify orientation before applying power
- ⚠️ **Common ground**: Both the 5V and 12V supplies MUST share a common GND. Floating grounds cause erratic comparator behavior and potential damage
- ⚠️ **Do not exceed LM35 supply**: LM35 max supply is 30V. Never connect it to the 12V rail with poor wiring practices that could expose it to transients
- ✅ Low-voltage DC circuit — safe for hands-on learning with proper precautions
- ✅ All component values are within normal operating ranges with comfortable safety margins
- ✅ Suitable for intermediate/advanced students with adult supervision for younger builders

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

Stay cool! 🌡️💨⚡
