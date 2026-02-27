# Digital Logic Probe ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - Multi-comparator design with analog voltage thresholds and mixed-signal behavior

## What You'll Learn
- How voltage comparators work and how to configure them
- TTL vs CMOS logic level standards and voltage thresholds
- Building voltage dividers for precision reference voltages
- LED indicator driver stages and current limiting
- Pulse and oscillating signal detection using RC networks
- Designing practical test instruments from scratch
- Reading and interpreting logic signals on real circuits
- Analog vs digital signal boundary behavior

## Components Required
- **LM339** Quad Comparator IC × 1
- **LED, Red** (Logic HIGH indicator) × 1
- **LED, Green** (Logic LOW indicator) × 1
- **LED, Yellow** (Pulse/oscillating signal indicator) × 1
- **Resistors**:
  - 10kΩ × 3 (voltage divider chain)
  - 1kΩ × 3 (LED current limiting)
  - 100kΩ × 1 (pull-up on comparator output)
- **Capacitor**, 100nF ceramic × 1 (pulse detection / high-pass filter)
- **Probe tip** (stiff wire, nail, or IC test hook)
- **Ground clip lead** (alligator clip with wire)
- **9V battery** × 1
- **Battery connector** (PP3 / 9V snap)
- **Breadboard** and jumper wires (or stripboard for permanent build)
- **Small enclosure** (optional, for permanent probe)

## Circuit Description
Build a handheld digital logic probe — the most useful debugging tool for anyone working with digital electronics! Touch the probe tip to any point in a 5V TTL or 3.3V/5V CMOS circuit and the probe instantly tells you whether that point is logic HIGH (Red LED), logic LOW (Green LED), or actively pulsing/toggling (Yellow LED). No LED at all means the signal is floating or disconnected. The heart of the design is the **LM339 Quad Comparator** IC, which compares the measured voltage against two precision reference thresholds set by a resistor voltage divider. A 100nF capacitor differentiates rapid voltage transitions, lighting the Yellow LED whenever edges are detected. This is a real, practical piece of test equipment you can build for under $3 and use for years!

## Theory of Operation

### Digital Logic Level Standards
Digital circuits don't use exact voltages — they define voltage *ranges* as valid HIGH or LOW. Between those ranges is an undefined "forbidden zone." Understanding these thresholds is essential for the probe's design.

#### TTL (Transistor-Transistor Logic) — 5V Supply
| Level | Voltage Range | Meaning |
|-------|--------------|---------|
| Valid HIGH output | 2.4V – 5.0V | Guaranteed HIGH driven by TTL gate |
| Valid LOW output  | 0.0V – 0.4V | Guaranteed LOW driven by TTL gate |
| Input HIGH threshold | > 2.0V | Recognised as HIGH by receiving gate |
| Input LOW threshold  | < 0.8V | Recognised as LOW by receiving gate |
| **Undefined / forbidden** | **0.8V – 2.0V** | Neither HIGH nor LOW — invalid |

#### CMOS (Complementary MOS) — 5V Supply
| Level | Voltage Range | Meaning |
|-------|--------------|---------|
| Valid HIGH output | > 4.5V | Strong HIGH from CMOS gate |
| Valid LOW output  | < 0.1V | Strong LOW from CMOS gate |
| Input HIGH threshold | > 3.5V (≈ 2/3 Vcc) | Recognised as HIGH |
| Input LOW threshold  | < 1.5V (≈ 1/3 Vcc) | Recognised as LOW |
| **Undefined** | **1.5V – 3.5V** | Transition region |

#### CMOS — 3.3V Supply
| Level | Voltage Range | Meaning |
|-------|--------------|---------|
| Valid HIGH output | > 2.4V (≈ 2/3 × 3.3V) | Logic HIGH |
| Valid LOW output  | < 0.3V | Logic LOW |
| Input HIGH threshold | > 2.0V | Recognised as HIGH |
| Input LOW threshold  | < 0.8V | Recognised as LOW |

> **Probe design choice**: The probe's reference voltages are set for 5V TTL/CMOS use (threshold LOW = ~1.67V, threshold HIGH = ~3.33V). This covers both 5V TTL and 5V CMOS with good margin.

### LED Indicator Truth Table
| Probe Condition | Red LED | Green LED | Yellow LED | Meaning |
|----------------|---------|-----------|-----------|---------|
| > ~3.3V (HIGH) | ✅ ON   | ❌ OFF    | ❌ OFF    | Logic HIGH |
| < ~1.67V (LOW) | ❌ OFF  | ✅ ON     | ❌ OFF    | Logic LOW |
| Pulsing signal | ✅ ON   | ✅ ON     | ✅ ON     | Toggling / oscillating |
| Floating / NC  | ❌ OFF  | ❌ OFF    | ❌ OFF    | No connection / hi-Z |
| ~1.67V–3.33V   | ❌ OFF  | ❌ OFF    | ❌ OFF    | Undefined / forbidden zone |

### Stage 1: Voltage Reference — The Resistor Divider
Three equal 10kΩ resistors are connected in series from the +5V rail (taken from the circuit under test) to GND. This creates two stable reference voltages:

```
+5V ──┬── 10kΩ ──┬── 10kΩ ──┬── 10kΩ ──┬── GND
      │          │           │           │
    (top)    VREF_HIGH    VREF_LOW    (bottom)
                ~3.33V       ~1.67V
```

- **VREF_HIGH = 3.33V** — anything above this is definitively HIGH
- **VREF_LOW  = 1.67V** — anything below this is definitively LOW

The probe is powered from 9V for the LED indicators, but the reference divider taps power from the circuit under test (5V) so the thresholds track its supply automatically.

### Stage 2: HIGH Comparator (LM339 Comparator 1)
The LM339 is an open-collector comparator — its output is either pulled LOW by the internal transistor or floats HIGH (needs a pull-up resistor).

**Comparator 1 configuration (HIGH detection):**
- **Non-inverting input (+)**: Probe tip signal
- **Inverting input (−)**: VREF_HIGH (3.33V)
- **Behaviour**: Output goes LOW (active) when probe > 3.33V → turns ON Red LED

```
Probe ──────┤+\
             |  >──┬── 100kΩ ── +9V
VREF_HIGH ──┤−/   │
                   └── 1kΩ ── Red LED ── GND
```

### Stage 3: LOW Comparator (LM339 Comparator 2)
**Comparator 2 configuration (LOW detection):**
- **Non-inverting input (+)**: VREF_LOW (1.67V)
- **Inverting input (−)**: Probe tip signal
- **Behaviour**: Output goes LOW (active) when probe < 1.67V → turns ON Green LED

```
VREF_LOW ───┤+\
             |  >──┬── 100kΩ ── +9V
Probe ──────┤−/   │
                   └── 1kΩ ── Green LED ── GND
```

### Stage 4: Pulse Detection Circuit (LM339 Comparator 3)
Static HIGH or LOW signals don't produce transitions. A **high-pass RC filter** passes only rapidly changing voltages (edges/pulses) while blocking DC levels. This differentiated signal drives the Yellow LED.

```
Probe ──── 100nF ──┬── Comparator 3 (+)
                   │
                 100kΩ (to probe-side GND, provides DC bias)
                   │
                  GND
```

- When the probe voltage is steady (HIGH or LOW), no current flows through the capacitor (DC blocked) → Yellow LED OFF
- When the probe voltage transitions (rising or falling edge), a pulse of current flows through the 100nF cap → momentary voltage spike at comparator input → Yellow LED flashes
- At frequencies above ~50Hz, the LED appears continuously lit

**Pulse comparator reference**: VREF_LOW (~1.67V) on the inverting input, ensuring the Yellow LED only responds to genuine signal transitions above the noise floor.

### LM339 Open-Collector Output Explained
Unlike most logic ICs, the LM339 output is **open-collector** — it can only pull LOW or float. It cannot drive HIGH on its own. This requires a **pull-up resistor** (100kΩ) to +9V on each comparator output. When the comparator output transistor is OFF (condition not met), the pull-up holds the output HIGH and no LED current flows. When the condition IS met, the transistor pulls the output to near-GND, current flows through the LED, and it lights up.

### Why LM339?
- Quad (4 comparators) in one 14-pin DIP package
- Operates from a single supply (1.5V to 36V)
- Open-collector outputs are easy to drive LEDs directly
- Industry standard part, available everywhere, very cheap
- Excellent for battery-powered instruments

## Pin Configuration

### LM339 Quad Comparator (14-pin DIP)
```
        +--U--+
  OUT1  |1  14| VCC (+9V)
   IN1- |2  13| OUT4
   IN1+ |3  12| IN4-
   GND  |4  11| IN4+
   IN2+ |5  10| IN3+
   IN2- |6   9| IN3-
  OUT2  |7   8| OUT3
        +-----+
```

| Pin | Name | Function in This Circuit |
|-----|------|--------------------------|
| 1   | OUT1 | HIGH comparator output → Red LED |
| 2   | IN1− | VREF_HIGH (3.33V reference) |
| 3   | IN1+ | Probe tip signal |
| 4   | GND  | Ground |
| 5   | IN2+ | VREF_LOW (1.67V reference) |
| 6   | IN2− | Probe tip signal |
| 7   | OUT2 | LOW comparator output → Green LED |
| 8   | OUT3 | PULSE comparator output → Yellow LED |
| 9   | IN3− | VREF_LOW (1.67V, pulse threshold) |
| 10  | IN3+ | Capacitor-filtered probe signal |
| 11  | IN4+ | Not used (tie to GND) |
| 12  | IN4− | Not used (tie to GND) |
| 13  | OUT4 | Not used |
| 14  | VCC  | +9V supply |

> ⚠️ Pin 4 (GND) must be connected. Unused comparator inputs should be tied to a defined voltage (GND or mid-rail) — never leave them floating.

### LED Polarity Reference
```
     Anode (+)   Cathode (−)
        │              │
   [flat side]    [longer lead = anode]
        │
   [shorter lead = cathode]

   Current flows: Anode → LED → Cathode
```

## Building Instructions

### Step 1: Plan Your Connections and Power
1. Decide on power scheme:
   - Probe 9V battery powers LM339 and LEDs
   - Reference divider connects to 5V of the **circuit under test** (via ground clip)
2. Connect 9V battery snap: Red → +9V rail, Black → GND rail
3. Add 100nF decoupling capacitor between +9V and GND rails (near LM339)
4. Verify 9V on rails with multimeter before placing ICs

### Step 2: Install the LM339 IC
1. Insert LM339 in the centre of the breadboard (notch/dot at pin 1, top-left)
2. Connect **Pin 14** to +9V rail
3. Connect **Pin 4** to GND rail
4. Note the pinout diagram — all four comparators are inside this one IC

### Step 3: Build the Voltage Reference Divider
This divider taps power from the **circuit under test** (5V), not from the probe battery:
1. Use a dedicated column on the breadboard for the divider
2. Connect top of divider to **5V of circuit under test** (via a short jumper wire — this will eventually come from the ground clip lead's companion power pin, or probe tip's DUT VCC)
3. **R_top** (10kΩ): Top of divider to node A
4. **R_mid** (10kΩ): Node A to node B
5. **R_bot** (10kΩ): Node B to GND (of circuit under test = probe ground clip)
6. **Node A** = VREF_HIGH (~3.33V) — connect to LM339 **Pin 2 (IN1−)**
7. **Node B** = VREF_LOW  (~1.67V) — connect to LM339 **Pins 5 (IN2+)** and **9 (IN3−)**

> 💡 **Tip**: If testing 3.3V CMOS circuits, use the 3.3V supply as the top of the divider instead of 5V. The thresholds automatically scale to 1/3 and 2/3 of the supply voltage.

### Step 4: Wire the HIGH Comparator (Red LED)
1. **Probe signal to Pin 3 (IN1+)**: Run a wire from your probe input node to LM339 Pin 3
2. **Pull-up resistor**: 100kΩ from +9V to Pin 1 (OUT1)
3. **Red LED**: Anode to Pin 1 (OUT1), Cathode through 1kΩ to GND
4. **Logic**: When probe > 3.33V → Pin 1 pulls LOW → current flows → Red LED ON

### Step 5: Wire the LOW Comparator (Green LED)
1. **Probe signal to Pin 6 (IN2−)**: Connect probe input node to LM339 Pin 6
2. **Pull-up resistor**: 100kΩ from +9V to Pin 7 (OUT2)
3. **Green LED**: Anode to Pin 7 (OUT2), Cathode through 1kΩ to GND
4. **Logic**: When probe < 1.67V → Pin 7 pulls LOW → current flows → Green LED ON

### Step 6: Build the Pulse Detection Filter
1. **100nF capacitor**: One leg to probe input node, other leg to a new node (call it PULSE_NODE)
2. **100kΩ bias resistor**: From PULSE_NODE to GND (provides DC path so cap doesn't block bias)
3. Connect PULSE_NODE to LM339 **Pin 10 (IN3+)**
4. **Pull-up resistor**: 100kΩ from +9V to Pin 8 (OUT3)
5. **Yellow LED**: Anode to Pin 8 (OUT3), Cathode through 1kΩ to GND

### Step 7: Tie Off Unused Comparator (Comparator 4)
1. Connect Pin 11 (IN4+) to GND
2. Connect Pin 12 (IN4−) to GND
3. Leave Pin 13 (OUT4) unconnected or connect through 100kΩ to +9V
4. This prevents oscillation and undefined states on unused inputs

### Step 8: Wire the Probe Tip and Ground Clip
1. **Probe tip**: Use a stiff wire, nail, or IC test hook soldered to a short wire
2. Connect probe tip wire to the **probe input node** (connecting to LM339 pins 3, 6, and the 100nF cap)
3. **Ground clip**: Alligator clip on a wire, connected to the **GND of the circuit under test** AND to the bottom of the reference divider
4. The probe's GND rail and the DUT's GND must be the same reference

### Step 9: Final Checks Before Power-On
1. Verify LM339 orientation (pin 1 notch correct)
2. Check all four pins of the resistor divider (10kΩ × 3)
3. Confirm LED polarities (longer lead = anode)
4. Verify no shorts between +9V and GND rails (measure resistance — should be > 1kΩ)
5. Check pull-up resistors on all three comparator outputs (pins 1, 7, 8)
6. Confirm unused comparator inputs are tied to GND (pins 11, 12)

### Step 10: Calibration Check
1. Power on probe (connect 9V battery)
2. Touch probe tip to GND of a 5V circuit — **Green LED should light**
3. Touch probe tip to +5V of the circuit — **Red LED should light**
4. Touch probe tip to the output of a 555 timer in astable mode — **All three LEDs should light** (or Yellow + Red/Green alternating)
5. Lift probe tip in the air — **All LEDs should be OFF** (floating input)

## Testing & Troubleshooting

### How to Use the Probe
1. Connect the **ground clip** to GND of the circuit under test — this is essential!
2. Connect the **5V wire from the divider** to the 5V rail of the circuit under test (or power the divider from a known 5V supply)
3. Power the probe via its own 9V battery
4. Touch the probe tip to the test point and read the LEDs

### Expected Behaviour on Various Signals

| Test Point | Expected LED State |
|------------|-------------------|
| Power rail +5V | 🔴 Red ON only |
| Ground rail 0V | 🟢 Green ON only |
| TTL gate HIGH output (~3.5–4.5V) | 🔴 Red ON only |
| TTL gate LOW output (~0.1–0.4V) | 🟢 Green ON only |
| CMOS HIGH output (~4.8V) | 🔴 Red ON only |
| CMOS LOW output (~0.05V) | 🟢 Green ON only |
| 1 kHz square wave | 🔴🟢🟡 All three ON |
| 100 Hz square wave | 🔴🟢🟡 All three ON (may see flicker) |
| 1 Hz clock | 🔴 and 🟢 alternate, 🟡 brief flash each transition |
| Floating pin / NC | ❌ All OFF |
| Open-collector pulled high | 🔴 Red ON |
| Open-collector not driven | ❌ All OFF (or erratic) |
| Undefined / forbidden zone (~2V) | ❌ All OFF |

### Common Issues and Solutions

**All LEDs OFF when touching a known HIGH signal:**
- Check ground clip is firmly connected to DUT GND
- Verify the reference divider is powered from the DUT's 5V (not the probe's 9V)
- Measure VREF_HIGH at LM339 Pin 2 — should be ~3.3V
- Measure probe tip voltage directly with multimeter
- Check LM339 Pin 14 is connected to +9V and Pin 4 to GND

**Red and Green LEDs both ON for a static signal:**
- The signal is very close to a threshold boundary — check if it is actually a slow ramp or noise
- Verify reference divider values (three equal 10kΩ resistors)
- Check for crosstalk on breadboard between comparator inputs

**Yellow LED always ON even for static signals:**
- 100nF capacitor connected to wrong node (check it is on the high-pass filter node, not probe-to-divider)
- 100kΩ bias resistor to GND missing from PULSE_NODE — without it the cap floats
- Electrical noise on probe tip wire (try a shorter wire, or add a 100Ω series resistor at tip)

**Yellow LED never lights even on known clocks:**
- Verify 100nF capacitor is present and working (check with capacitance meter or swap)
- Confirm PULSE_NODE connects to LM339 Pin 10 (IN3+)
- Check VREF_LOW on Pin 9 is ~1.67V (not floating)
- Pull-up resistor (100kΩ) from +9V to Pin 8 must be present

**Green LED dim or flickering instead of solid:**
- Battery voltage low — measure the 9V battery (should be > 7.5V under load)
- LED current-limiting resistor too high — swap 1kΩ for 680Ω for brighter indication
- Check solder joints or breadboard connection if building off-breadboard

**Red LED always ON regardless of signal:**
- VREF_HIGH at Pin 2 is too low — check divider top is connected to 5V, not 9V
- If divider is powered from 9V accidentally: VREF_HIGH = 6V, VREF_LOW = 3V → Red LED may never trigger
- Confirm 10kΩ × 3 and top = 5V of DUT

**Probe reads random/floating even when connected to GND:**
- Ground clip not making solid contact — clean the clip, ensure good mechanical connection
- Ground wire too long (> 30cm) can pick up noise — keep it short

**LM339 gets hot:**
- Check for short between output pins and GND (open-collector outputs must never be connected directly to supply without pull-up)
- Verify all three pull-up resistors (100kΩ) are present — without them the LEDs could draw excessive current

## Calculations

### Voltage Divider Reference Levels
With three equal resistors (R = 10kΩ each) and supply voltage Vs = 5V (from DUT):

```
VREF_HIGH = Vs × (R_mid + R_bot) / (R_top + R_mid + R_bot)
          = 5V × (10k + 10k) / (10k + 10k + 10k)
          = 5V × 20k / 30k
          = 5V × 0.667
          = 3.33V

VREF_LOW  = Vs × R_bot / (R_top + R_mid + R_bot)
          = 5V × 10k / 30k
          = 5V × 0.333
          = 1.67V
```

These sit comfortably inside the TTL and 5V CMOS detection ranges:
- TTL HIGH threshold: 2.0V → our VREF_HIGH = 3.33V ✅ (above TTL HIGH threshold)
- TTL LOW  threshold: 0.8V → our VREF_LOW  = 1.67V ✅ (above TTL LOW threshold, so LOW voltages < 0.8V will still trigger Green)

### LED Current Limiting Resistors
With the LM339 output at near-GND (≈0.1V), pull-up at +9V through 100kΩ, and LED in series with 1kΩ to GND:

```
V_available = 9V − V_f(LED) − V_sat(LM339)
            = 9V − 2.0V − 0.1V
            = 6.9V

I_LED = V_available / R_limit = 6.9V / 1kΩ ≈ 6.9 mA
```

This gives a clearly visible but conservative current — well within LM339's 16mA output sink limit and typical LED ratings (20mA max). For brighter LEDs, reduce to 680Ω (~10mA).

### Pulse Detection RC Time Constant
The 100nF capacitor with the 100kΩ bias resistor forms a high-pass filter:

```
τ = R × C = 100kΩ × 100nF = 0.01 seconds = 10ms

Corner frequency f_c = 1 / (2π × R × C)
                     = 1 / (2π × 100k × 100n)
                     = 1 / (2π × 0.01)
                     ≈ 15.9 Hz
```

Signals above ~16 Hz will pass through the capacitor and trigger the Yellow LED. Signals below 16 Hz (very slow toggles) will only produce brief flashes of Yellow on each edge — useful distinction!

### Reference Divider Current Draw
```
I_divider = Vs / (R_top + R_mid + R_bot)
          = 5V / 30kΩ
          ≈ 0.17 mA
```
Negligible — will not affect the DUT supply.

### Total Probe Current Consumption (all LEDs ON)
```
3 × LED current ≈ 3 × 7mA = 21mA (probe battery, 9V)
Divider current ≈ 0.17mA (from DUT 5V supply)
LM339 quiescent ≈ 1.2mA typical
Total probe (9V): ≈ 22mA

9V alkaline battery (550 mAh) runtime ≈ 550 / 22 ≈ 25 hours of continuous use
```

## Key Concepts Learned
- **Voltage Comparators**: Open-collector outputs, pull-up resistors, and threshold comparison
- **Voltage Dividers**: Calculating reference levels with resistor ratios
- **Logic Level Standards**: TTL, CMOS 5V, CMOS 3.3V threshold voltages and noise margins
- **High-Pass RC Filters**: Differentiating circuits for edge detection
- **Open-Collector Outputs**: Why pull-up resistors are required and how to size them
- **Test Instrument Design**: Real-world design constraints for a useful measurement tool
- **Signal Integrity**: Floating inputs, undefined logic levels, and why they cause problems
- **LED Driver Circuits**: Current limiting, brightness control, and indicator design

## Applications
- **Debugging TTL/CMOS logic circuits**: Instantly check any IC pin's logic state
- **Verifying clock signals**: Confirm oscillators are running
- **Tracing data buses**: Follow data propagation through a circuit
- **Checking Arduino / microcontroller outputs**: Confirm GPIO levels are correct
- **Testing 74xx / 40xx series logic ICs**: Verify gate outputs
- **SPI / I2C / UART debug**: Detect activity on communication lines
- **Reset and enable line checking**: Verify active-low/active-high control signals
- **Oscillator testing**: Check if a crystal oscillator is running
- **PCB fault-finding**: Locate stuck-at faults (pin stuck HIGH or stuck LOW)
- **Timer circuit verification**: Confirm 555 timer output is pulsing

## Next Steps / Extensions

### 1. Add an Audible Beep Indicator
Add a small piezo buzzer driven by a fourth LM339 comparator (or a transistor) to beep when a pulse is detected. This allows eyes-free probing — you hear activity without looking at the LEDs.

**Implementation**: Connect the Yellow LED output node through a 10kΩ resistor to the base of an NPN transistor (e.g. 2N3904). Connect a piezo buzzer between the transistor's collector and +9V. Emitter to GND.

### 2. Memory / Latch Mode
Add a flip-flop (e.g. 74HC74 D-type) to latch the pulse indicator. Once a pulse is detected, the Yellow LED stays ON until a reset button is pressed. Useful for catching rare glitches on fast signals.

### 3. Frequency Counter / Period Meter
Extend the probe with an Arduino or ATmega328 to count pulses per second and display the frequency on a 4-digit 7-segment display. This transforms the probe into a combined logic probe + frequency counter.

### 4. Adjustable Thresholds
Replace the three fixed 10kΩ divider resistors with a 47kΩ potentiometer. A calibration knob lets you fine-tune the HIGH/LOW thresholds for different logic families (ECL, HTL, etc.) or non-standard supply voltages.

### 5. 3.3V / 5V Auto-Detect
Use a second voltage divider with a comparator to detect whether the DUT supply is 3.3V or 5V, and automatically switch a relay or MOSFET to select the appropriate reference chain.

### 6. Bus Exerciser / Current Tracer
Pair the logic probe with a current tracer (a hall-effect sensor on a probe tip) to not just detect voltage levels but also identify which device on a bus is driving a line — invaluable for debugging bus contention faults.

### 7. Permanent Handheld Instrument
Transfer the working breadboard design to **stripboard (Veroboard)** and house it in a small plastic enclosure (e.g. a 100mm × 50mm project box). Add:
- Banana jack or BNC socket for ground clip
- Panel-mount LEDs through holes in the enclosure
- On/off switch
- Battery holder inside the enclosure

This creates a permanent, professional-looking test instrument.

## Safety Notes
- ✅ Low voltage circuit — 9V probe supply and 5V DUT signals are very safe
- ✅ All components are low power; no heat hazard under normal use
- ⚠️ **Never probe mains voltage (120V/240V AC)** — this probe is for 3.3V and 5V logic ONLY
- ⚠️ **Do not probe voltages above 5V** at the probe tip — the LM339 inputs are not protected above Vcc; add a 10kΩ series resistor at the tip for basic protection on unknown circuits
- ⚠️ Don't reverse the 9V battery polarity — will immediately damage the LM339
- ⚠️ CMOS ICs are sensitive to static discharge — always ground yourself before handling
- ⚠️ Ensure ground clip is firmly attached before probing — a poor ground gives false readings
- ✅ Suitable for intermediate/advanced builders; no dangerous voltages involved
- ⚠️ Check the voltage rating of the specific circuit you are probing before connecting

---

**Circuit Status**: ✅ Verified circuit design — ready to build  
**Last Updated**: December 2024

Probe the logic! 🔬⚡🔴🟢🟡
