# Adjustable Power Supply (LM317) ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Voltage regulation and power supply design

## What You'll Learn
- Linear voltage regulation principles
- LM317 adjustable regulator operation
- Power supply design fundamentals  
- Heat dissipation and thermal management
- Filtering and ripple reduction
- Load regulation and line regulation
- Safe voltage/current output design

## Components Required
- **LM317T** Variable Voltage Regulator IC × 1 (TO-220 package)
- **Transformer** 12V-0-12V center-tapped, 500mA or 1A × 1
- **Bridge Rectifier** (1A, 50V) × 1 (or 4× 1N4007 diodes)
- **Capacitors**:
  - 1000µF, 25V electrolytic × 2 (input filtering)
  - 100µF, 25V electrolytic × 1 (output filtering)
  - 10µF, 25V electrolytic × 1
  - 100nF (0.1µF) ceramic × 2 (high-frequency filtering)
  - 10nF (0.01µF) ceramic × 1 (stability)
- **Resistors**:
  - 240Ω, 1/4W × 1 (R1 - fixed resistor)
  - 5kΩ potentiometer × 1 (R2 - voltage adjustment)
- **Heat Sink** for LM317 (required for currents >200mA)
- **Thermal paste** (for heat sink attachment)
- **Optional Components**:
  - Digital voltmeter module (DC 0-30V)
  - LED indicator (power on) with 1kΩ resistor
  - Binding posts or banana jacks (red/black)
  - Case/enclosure
  - Fuse and fuse holder (1A)

## Circuit Description
Build your own variable DC bench power supply! This essential tool outputs adjustable voltage from 1.25V to ~12V (or higher depending on transformer) with up to 1A current capacity. The LM317 is a 3-terminal adjustable linear regulator - one of the most popular voltage regulator ICs ever made. Perfect for testing circuits, charging batteries, or providing variable voltage to projects. Every electronics enthusiast needs a good power supply!

## Theory of Operation

### How the LM317 Works

The LM317 maintains a constant **1.25V reference voltage** between its OUTPUT and ADJUST pins. It uses feedback to regulate the output voltage based on the external resistor network.

**Key Principle**: The LM317 forces 1.25V across R1, creating a constant current through R1 and R2.

### Voltage Formula
```
V_out = 1.25V × (1 + R2/R1) + (I_adj × R2)

Simplified (I_adj ≈ 50µA, negligible):
V_out ≈ 1.25V × (1 + R2/R1)
```

### Example Calculations

**With R1 = 240Ω, R2 = 5kΩ potentiometer:**

**Minimum Output** (R2 = 0Ω):
```
V_out = 1.25V × (1 + 0/240)
V_out = 1.25V
```

**Maximum Output** (R2 = 5kΩ):
```
V_out = 1.25V × (1 + 5000/240)
V_out = 1.25V × (1 + 20.83)
V_out = 1.25V × 21.83
V_out ≈ 27.3V
```

**Actual max** is limited by input voltage. With 12V transformer rectified (≈17V DC), max output ≈15V.

### Current Through Resistors
```
I_R1 = 1.25V / R1 = 1.25V / 240Ω ≈ 5.2mA
```
This current flows through both R1 and R2, creating voltage drops.

### Power Supply Stages

**Stage 1: AC to Pulsing DC (Rectification)**
- Transformer steps down mains AC to 12V AC
- Bridge rectifier converts AC to pulsing DC
- Output: Unfiltered DC with large ripple (~120Hz pulsations)

**Stage 2: Smoothing (Filtering)**
- Large capacitors (1000µF) smooth the pulsing DC
- Stores energy during AC peaks, releases during valleys
- Reduces ripple to <5% typically
- Output: Relatively smooth DC (~17V from 12V AC transformer)

**Stage 3: Regulation (LM317)**
- LM317 maintains constant output voltage regardless of:
  - Input voltage variations (line regulation)
  - Load current variations (load regulation)
- Adjustable from 1.25V to (V_in - 3V dropout)
- Output: Clean, stable, adjustable DC voltage

**Stage 4: Output Filtering**
- Smaller capacitors on output for transient response
- 100µF handles load changes
- 100nF filters high-frequency noise
- Output: Very clean DC with minimal ripple (<10mV)

### Dropout Voltage
LM317 requires **minimum 3V difference** between input and output:
```
V_out(max) = V_in - 3V

Example: 17V input → 14V max output
```

## Pin Configuration

### LM317T (TO-220 Package)
```
Looking at front (metal tab facing away):

   ___________
  |           |
  |   LM317   |  ← Metal tab with mounting hole
  |___________|
   |   |   |
   1   2   3

Pin 1: ADJUST (ADJ)  - Feedback voltage sense
Pin 2: OUTPUT (OUT)  - Regulated DC output  
Pin 3: INPUT (IN)    - Unregulated DC input
```

**IMPORTANT**: Tab is connected to OUTPUT (pin 2). Isolate from ground if heat sink is grounded!

### Bridge Rectifier (4-pin package)
```
    AC ~
    |  |
   +-  -+
   
+-----------+
|  ~     ~  |  AC inputs (from transformer)
|           |
|  +     -  |  DC outputs
+-----------+

~  ~  = AC input terminals (interchangeable)
+     = Positive DC output
-     = Negative DC output (GND)
```

## Building Instructions

### Safety First!
⚠️ **WARNING**: This project involves mains AC voltage (120V/240V) which can be **LETHAL**!
- Only work on transformer primary side if you have electrical training
- Better option: Use pre-made 12V AC wall adapter (safer)
- Never touch exposed AC connections when powered
- Use proper insulation and enclosure
- Add a fuse on the AC side

### Step 1: Rectification Stage
1. Connect transformer **secondary** (12V AC) terminals to bridge rectifier **AC inputs** (~ terminals)
2. If using individual diodes instead of bridge:
   - Create bridge: D1 anode → AC, cathode → +DC
   - D2 cathode → AC, anode → GND
   - D3 anode → AC, cathode → +DC  
   - D4 cathode → AC, anode → GND
3. Bridge **positive output** goes to next stage
4. Bridge **negative output** to GND (common reference)

### Step 2: Input Filtering
1. Connect **1000µF capacitor** from bridge (+) to GND
   - Watch polarity! Negative stripe to GND
   - This smooths rectified pulsing DC
2. Add **second 1000µF capacitor** in parallel (optional, better filtering)
3. Add **100nF ceramic capacitor** in parallel for HF noise
4. Measure voltage: Should be √2 × 12V ≈ 17V DC

### Step 3: LM317 Voltage Regulator
1. **Mount heat sink** to LM317 with thermal paste
   - Essential for currents >200mA!
   - Use insulating washer if heat sink is grounded
2. Insert LM317 on breadboard or terminal strip
3. Connect **INPUT pin (3)** to filtered DC from bridge rectifier
4. Connect **100nF ceramic** between INPUT and GND (close to IC)
5. This is unregulated input (~17V)

### Step 4: Voltage Setting Network
1. Connect **240Ω resistor (R1)** from OUTPUT pin (2) to ADJUST pin (1)
2. Connect **5kΩ potentiometer** from ADJUST pin (1) to GND:
   - Pot terminal 1: to ADJUST pin
   - Pot terminal 2 (wiper): to GND
   - Pot terminal 3: leave open (or also to GND)
3. This creates adjustable voltage divider
4. Add **10µF capacitor** from ADJUST to GND (improves ripple rejection)

### Step 5: Output Filtering
1. Connect **100µF capacitor** from OUTPUT pin to GND (watch polarity!)
2. Connect **100nF ceramic** in parallel for HF filtering
3. Add **10nF ceramic** from OUTPUT to ADJUST (improves transient response)
4. These capacitors provide clean output and prevent oscillations

### Step 6: Output Connections
1. Connect **positive output** from OUTPUT pin to red binding post/jack
2. Connect **negative output** (GND) to black binding post/jack
3. Optional: Add **LED indicator** from output through 1kΩ resistor to GND
4. Optional: Add **voltmeter module** across output terminals

### Step 7: Protection (Recommended)
1. Add **1A fuse** on transformer primary (AC mains side)
2. Add **diode** across LM317 (cathode to INPUT, anode to OUTPUT)
   - Protects if output gets shorted to input
3. Add **diode** from GND to OUTPUT
   - Protects if output becomes more negative than ground

### Step 8: Testing Before Enclosure
1. **Visual inspection**: Check all connections, polarity
2. **Continuity test**: Verify no shorts between output and input
3. **Power up without load**:
   - Turn pot to minimum
   - Measure output: Should be ~1.25V
   - Turn pot to maximum
   - Measure output: Should increase to ~12-15V
4. **Load test**: Connect 100Ω resistor as load (draws ~100mA at 10V)
5. Check voltage regulation - should stay constant
6. Feel heat sink - should be warm but not too hot to touch

### Step 9: Enclosure and Final Assembly
1. Mount all components in proper enclosure
2. Ensure all AC connections fully insulated
3. Drill holes for adjustment pot, voltmeter, output terminals
4. Label: "ADJUSTABLE DC POWER SUPPLY 1.25-15V, 1A MAX"
5. Add voltage markings on pot knob (calibrate first!)

## Testing & Troubleshooting

### Expected Behavior
- Smooth voltage adjustment from 1.25V to ~15V (with 12V transformer)
- Output voltage stable regardless of load (up to 1A)
- Minimal ripple (<50mV peak-to-peak)
- Quick response to pot adjustment
- Heat sink warm during use (normal)

### Voltage Measurements

**No Load (open circuit):**
- Transformer secondary: 12V AC (use AC setting on multimeter)
- Rectified (after bridge, before filter): Pulsing ~17V DC
- Filtered (after 1000µF caps): Steady ~17V DC
- LM317 OUTPUT: 1.25V to 15V (adjustable with pot)
- Ripple: <20mV (use oscilloscope or AC millivolt range)

**With 1A Load:**
- Input voltage may drop slightly (transformer loading)
- Output should remain stable within ±50mV
- Heat sink should be warm (~50-60°C acceptable)

### Common Issues and Solutions

**No output voltage:**
- Check input voltage to LM317 INPUT pin (should be 17V)
- Verify LM317 orientation (pin 1 = ADJ, pin 2 = OUT, pin 3 = IN)
- Check continuity of 240Ω resistor
- Test LM317: Remove and check with multimeter (may be damaged)
- Ensure all capacitors have correct polarity

**Output voltage doesn't adjust:**
- Potentiometer not connected correctly
- Wiper must go to GND
- Check 240Ω resistor - may be open
- Verify ADJUST pin connection

**Output voltage too low (stuck at 1.25V):**
- Potentiometer wiper not moving - rotate shaft
- R2 path open - check connections
- Short between ADJUST and GND

**Output voltage too high or uncontrolled:**
- R1 (240Ω) may be open - test with multimeter
- ADJUST pin floating - check connection
- Internal LM317 damage - replace

**High ripple on output (>100mV):**
- Input filtering inadequate - add more capacitance (2200µF)
- Output capacitors missing or wrong polarity
- Electrolytic capacitor dried out (old) - replace
- Load current too high - reduce load or upgrade components
- Add 10µF from ADJUST to GND

**Voltage drops under load:**
- Input voltage too low (dropout)
- Need V_in ≥ V_out + 3V at all times
- Transformer undersized - upgrade to higher current rating
- Poor connections - check all joints
- LM317 may be fake/substandard - source from reputable supplier

**LM317 gets extremely hot:**
- **Power dissipation too high**: P = (V_in - V_out) × I_load
- Example: (17V - 5V) × 1A = 12W (TOO HOT!)
- Solutions:
  1. Use lower input voltage transformer for low output voltages
  2. Larger heat sink with thermal paste
  3. Add cooling fan
  4. Reduce load current
  5. Switch to switching regulator for efficiency

**Oscillations or instability:**
- Output oscillating or noisy
- Add 100nF ceramic cap close to LM317 INPUT pin
- Add 10nF ceramic from OUTPUT to ADJUST
- Use short, direct wiring to LM317
- Ensure good grounding

**Transformer hums loudly:**
- Normal: Slight hum from magnetostriction
- Excessive hum: Loose laminations or overload
- Check load current - may be too high

**Blown fuse:**
- Short circuit on output - find and fix
- Transformer failure - test with multimeter
- Bridge rectifier shorted - replace
- Inrush current - use slow-blow fuse

### Load Regulation Test
1. Set output to 10V with no load
2. Connect 10Ω resistor (1A load)
3. Output should drop <100mV (good regulation)
4. If drops more, increase input filtering

### Line Regulation Test
1. Vary input voltage ±10% (e.g., 15V-19V)
2. Output should stay within ±10mV
3. LM317 has excellent line regulation

## Calculations

### Power Dissipation in LM317
```
P_dissipated = (V_in - V_out) × I_load

Example: 5V output, 1A load, 17V input
P = (17V - 5V) × 1A = 12W

This requires substantial heat sink!
```

### Heat Sink Requirements
```
Thermal resistance needed:
θ_total = (T_max - T_ambient) / P_dissipated

For LM317: T_max = 125°C (but keep <100°C)
Ambient = 25°C, P = 12W, want T_junction = 80°C

θ_total = (80°C - 25°C) / 12W = 4.6°C/W

LM317 θ_JC = 5°C/W (junction to case)
Need heat sink: θ_SA < 4.6 - 5 = -0.4°C/W... not possible!

Solution: Reduce power dissipation (lower Vin, reduce Iload)
```

### Efficiency Calculation
```
Efficiency = (V_out × I_load) / (V_in × I_in) × 100%

Example: 5V out, 1A load, 17V in
I_in ≈ I_out (plus small quiescent current ~5mA)
Efficiency = (5V × 1A) / (17V × 1.005A) × 100%
Efficiency ≈ 29.2%

Linear regulators are INEFFICIENT when V_in >> V_out!
```

### Ripple Voltage
```
V_ripple = I_load / (f × C)

Where:
f = ripple frequency (120Hz for full-wave)
C = filter capacitance
I_load = load current

Example: 1A load, 2000µF capacitor
V_ripple = 1A / (120Hz × 0.002F)
V_ripple = 4.17V (before LM317)

LM317 ripple rejection: 65dB typical
Output ripple = 4.17V / 1780 ≈ 2.3mV (excellent!)
```

## Experimentation Ideas

### Add Current Limiting
- Insert LM317 current limit circuit (see datasheet)
- Adjustable from 100mA to 1.5A
- Protects against shorts

### Digital Voltmeter/Ammeter
- Add panel meter module (cheap from eBay/Amazon)
- Displays voltage and current simultaneously
- Makes bench supply much more usable!

### Dual Output (±Voltage)
- Build two identical supplies
- One positive, one negative
- Great for op-amp circuits

### Switch-Mode Pre-Regulator
- Add buck converter before LM317
- Reduces power dissipation
- Maintains efficiency at low output voltages

### Add Constant Current Mode
- Additional LM317 as current regulator
- Useful for LED testing and battery charging

### Remote Sensing
- Add wires from output to load
- Compensates for voltage drop in output wires
- Provides accurate voltage at load

### Adjustable Current Limit
- Add transistor and sense resistor
- Foldback current limiting
- Protects supply and load

## Key Concepts Learned
- **Linear Voltage Regulation**: Using active components to maintain voltage
- **Transformer Theory**: AC voltage step-up/down
- **Rectification**: Converting AC to DC
- **Filtering**: Smoothing pulsing DC with capacitors
- **Feedback Control**: How regulators maintain output
- **Thermal Management**: Heat sinking and power dissipation
- **Ripple Rejection**: Removing AC noise from DC output
- **Load/Line Regulation**: Maintaining voltage despite changes
- **Power Supply Design**: Complete system integration

## Applications
- **Bench power supply**: Testing and development
- **Battery charging**: Variable voltage charging
- **Circuit testing**: Adjustable voltage for experiments
- **LED drivers**: Constant voltage for LED strips
- **Prototyping**: Power for breadboard projects
- **Motor control**: Variable voltage for DC motors
- **Audio equipment**: Low-noise power rails
- **Sensor calibration**: Precise voltage reference

## Next Steps
Once you've mastered this circuit, try:
1. **Dual Power Supply** (Project #20) - Build ±12V supply
2. **Constant Current Source** - LM317 as current regulator
3. **Adjustable Bench Supply** - Add current limiting and metering
4. **Switching Power Supply** - High efficiency alternative
5. **Battery Charger** - CC/CV charging circuit

## Resources
- [LM317 Datasheet](https://www.ti.com/lit/ds/symlink/lm317.pdf) - Essential reading!
- [LM317 Calculator](http://www.ti.com/lit/an/snva558/snva558.pdf)
- Power Supply Design Tutorial by TI
- Heat Sink Selection Guide

## Safety Notes
- ⚠️ **DANGER**: Mains AC voltage can KILL! Work carefully on AC side
- ⚠️ Transformer primary (AC mains) must be properly insulated
- ⚠️ Use fuse on AC input for protection
- ⚠️ Ensure case is grounded if metal
- ⚠️ Electrolytic capacitors store charge - discharge before touching
- ⚠️ LM317 heat sink can be hot (>60°C) - don't touch during operation
- ⚠️ Short circuit on output can cause overheating - use current limiting
- ⚠️ Bridge rectifier gets warm - ensure adequate ventilation
- ✅ Test output with multimeter before connecting projects
- ✅ Start with low current loads when testing
- ✅ Use safety-rated transformer with proper certification
- ✅ Consider using pre-made 12V AC adapter instead of building transformer circuit

## Advanced Considerations

### Alternatives to Consider
**Buck/Boost Converters**: Higher efficiency (>85%) but more noise
**LM338**: Higher current version (5A) of LM317
**LM350**: Medium current (3A) version
**LT1083**: Low dropout (1V) alternative

### Improving Performance
- **Lower noise**: Add LC filter on output
- **Better load regulation**: Add output voltage sensing
- **Faster transient response**: Increase output capacitance
- **Parallel LM317s**: For higher current (with current sharing resistors)

---

**Circuit Status**: ✅ Verified and works as expected
**Last Updated**: December 2024

Power your projects! ⚡🔧
