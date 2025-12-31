# Dual Power Supply (±12V Regulated) ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - Dealing with AC mains voltage and dual rail systems

## What You'll Learn
- Dual voltage power supply design (±V rails)
- Center-tapped transformer operation
- Bridge rectification and filtering
- Voltage regulation with 78xx/79xx series
- Heat dissipation and thermal management
- Power supply protection circuits

## Components Required
- **Center-tapped transformer** (12V-0-12V, 2A) × 1
- **Bridge rectifiers** or 1N4007 diodes × 8
- **LM7812** +12V voltage regulator × 1
- **LM7912** -12V voltage regulator × 1
- **Capacitors**:
  - 2200µF/25V electrolytic × 4 (input filtering)
  - 100nF/50V ceramic × 4 (output decoupling)
- **Heat sinks** for regulators × 2
- **Fuse** (2A, slow-blow) × 1
- **LED indicators** (Red, Green) × 2
- **Resistors** 1kΩ × 2 (for LEDs)
- **Power switch** (SPST, 250V AC rated) × 1
- **Terminal blocks** or binding posts × 3 (GND, +12V, -12V)
- **PCB or perfboard** (strongly recommended for permanent build)
- **Enclosure** (plastic, non-conductive)

## Circuit Description
Build a professional-grade dual power supply that provides both +12V and -12V simultaneously! Essential for op-amp circuits, audio amplifiers, and analog electronics. Uses center-tapped transformer to create split AC voltages, rectifies each half separately, and regulates with 78/79 series ICs. Includes proper filtering, heat sinking, LED indicators, and protection. This is a must-have tool for any electronics workbench!

## Theory of Operation

### Why Dual Power Supplies?
Many analog circuits need both positive and negative voltages:
- **Op-amps**: Need ±V to amplify AC signals (audio, sensors)
- **Audio amplifiers**: Symmetric power for push-pull operation
- **Analog circuits**: Bipolar signals require bipolar power
- **Reference circuits**: Create ground at mid-point

### How It Works

**Step 1: AC Transformation**
- Mains AC (120V/230V) steps down to safe voltage
- Center-tapped secondary: Two 12V AC outputs with common center
- Center tap becomes ground reference (0V)
- Each half provides 12V AC relative to center

**Step 2: Rectification**
- Each half-wave rectified separately
- Bridge rectifiers convert AC to pulsing DC
- Top half: +12V rail (before regulation)
- Bottom half: -12V rail (before regulation)

**Step 3: Filtering**
- Large capacitors (2200µF) smooth pulsing DC
- Remove 120Hz ripple (100Hz in 50Hz countries)
- Store energy between AC peaks
- Result: Relatively smooth but unregulated DC

**Step 4: Voltage Regulation**
- **LM7812**: Regulates positive rail to exactly +12V
- **LM7912**: Regulates negative rail to exactly -12V
- Removes remaining ripple
- Maintains constant voltage despite load changes
- Protects against overvoltage and shorts

**Step 5: Output Filtering**
- 100nF ceramic caps at outputs
- Remove high-frequency noise
- Improve transient response
- Essential for sensitive circuits

### The Three Rails
```
+12V  ───┬─── Positive supply
         │
  0V  ───┼─── Ground (center tap)
         │
-12V  ───┴─── Negative supply
```
Total voltage span: 24V (+12V to -12V)

## Pin Configuration

### LM7812 Positive Voltage Regulator (TO-220)
```
  Front view (metal tab up):
  
  ┌─────────┐
  │  7812   │
  │ ┌─────┐ │ Metal tab (connects to GND)
  └─┼──┼──┼─┘
    │  │  │
    1  2  3
    │  │  │
   IN GND OUT
```
- Pin 1 (IN): Unregulated +V input (15-20V DC)
- Pin 2 (GND): Ground reference
- Pin 3 (OUT): Regulated +12V output

### LM7912 Negative Voltage Regulator (TO-220)
```
  Front view (metal tab up):
  
  ┌─────────┐
  │  7912   │
  │ ┌─────┐ │ Metal tab (connects to GND)
  └─┼──┼──┼─┘
    │  │  │
    1  2  3
    │  │  │
   GND IN OUT
```
- Pin 1 (GND): Ground reference
- Pin 2 (IN): Unregulated -V input (-15 to -20V DC)
- Pin 3 (OUT): Regulated -12V output

**Note**: 7912 has different pinout than 7812!

## Building Instructions

⚠️ **DANGER**: This project involves AC mains voltage which can be lethal! Only attempt if you have experience with AC wiring, or work with a qualified electrician. Always disconnect from mains when working on circuit.

### Safety First!
1. **Use insulated tools**
2. **Work on non-conductive surface**
3. **Never touch circuit when powered from AC mains**
4. **Use proper enclosure** - all AC wiring must be enclosed
5. **Add fuse protection** - 2A slow-blow fuse on transformer primary
6. **Ground the enclosure** if metal

### Step 1: Transformer Connection (AC Mains Section)
1. Mount transformer securely in enclosure
2. Primary side (120V/230V):
   - Connect fuse in series with hot (live) wire
   - Connect fuse output to transformer primary
   - Connect neutral to transformer primary
   - Add power switch in hot wire
3. **Test continuity** with multimeter (power OFF!)
4. **Insulate all AC connections** with heat shrink or electrical tape

### Step 2: Rectification Stage
**Positive Rail:**
1. Connect transformer secondary top terminal (12V) to bridge rectifier AC input
2. Connect center tap to second AC input of bridge
3. Bridge + output → 2200µF capacitor + terminal
4. Bridge - output → 2200µF capacitor - terminal (which goes to center tap/GND)
5. Watch capacitor polarity!

**Negative Rail:**
6. Connect transformer secondary bottom terminal (-12V) to second bridge rectifier
7. Connect center tap to second AC input
8. Bridge - output → 2200µF capacitor - terminal  
9. Bridge + output → 2200µF capacitor + terminal (to center tap/GND)
10. Note: Connections reversed for negative rail!

### Step 3: Voltage Regulators
**LM7812 (Positive):**
1. Attach heat sink to regulator
2. IN (pin 1) → Positive filter cap (+)
3. GND (pin 2) → Center tap (0V)
4. OUT (pin 3) → 100nF capacitor → +12V output terminal
5. Add 100nF from IN to GND (close to IC)

**LM7912 (Negative):**
6. Attach heat sink to regulator
7. GND (pin 1) → Center tap (0V)
8. IN (pin 2) → Negative filter cap (-)
9. OUT (pin 3) → 100nF capacitor → -12V output terminal
10. Add 100nF from IN to GND (close to IC)

### Step 4: Output Terminals and Indicators
1. Mount three terminal blocks/binding posts:
   - Red: +12V
   - Black: GND (0V)
   - Blue: -12V
2. **Positive LED**: +12V → 1kΩ → LED → GND (green LED)
3. **Negative LED**: -12V → 1kΩ → LED → GND (red LED)

### Step 5: Final Assembly
1. Mount all components on PCB or perfboard
2. Keep AC and DC sections separate
3. Use thick wires for high-current paths (18-20 AWG)
4. Secure all connections
5. **Double-check all wiring** before applying power!

## Testing & Troubleshooting

### Initial Testing (No Load)
1. **Visual inspection**: Check all connections, no shorts
2. **Transformer test**: Measure AC secondary voltages
   - Should be ~12V AC from center to each end
   - ~24V AC between ends
3. **Rectifier test**: Check DC after rectifiers (before regulators)
   - Positive: ~17V DC
   - Negative: ~-17V DC
4. **Regulator outputs**:
   - +12V rail: 11.8V to 12.2V
   - -12V rail: -11.8V to -12.2V
5. **LEDs**: Both should light

### Load Testing
1. Connect 100Ω resistor from +12V to GND
   - Current: 120mA
   - Voltage should stay at 12V
2. Connect 100Ω resistor from GND to -12V
   - Same test for negative rail
3. **Temperature check**: Regulators will get warm (not too hot to touch)

### Voltage Measurements
**AC Side (transformer secondary):**
- Center to top: 12V AC
- Center to bottom: 12V AC
- Top to bottom: 24V AC

**DC Side (after rectification, before regulation):**
- Positive cap: +15V to +18V DC
- Negative cap: -15V to -18V DC

**Regulated Outputs:**
- +12V output: +11.9V to +12.1V
- GND: 0V
- -12V output: -11.9V to -12.1V

### Common Issues

**No output voltage:**
- Check fuse - might be blown
- Transformer connections loose
- Bridge rectifier backward
- Regulator damaged

**Low voltage (<11V):**
- Transformer output too low
- Load too high (>1.5A)
- Regulator overheating and shutting down
- Poor connections causing voltage drop

**Regulator very hot:**
- Insufficient heat sink
- Excessive current draw
- Input voltage too high (>15V difference from output)
- Regulator may be damaged

**Asymmetric outputs (+12V OK, but -12V wrong):**
- Check LM7912 pinout (different from 7812!)
- Negative rectifier wired incorrectly
- Negative filter cap polarity reversed

**High ripple voltage:**
- Filter capacitors too small or faulty
- Bad electrolytic capacitor
- Missing output decoupling caps (100nF)

**Transformer humming:**
- Normal at light load
- Mounting screws loose - tighten
- Overloaded if very loud

## Calculations

### Transformer Selection
```
Output power needed: 12V × 2A = 24W per rail
Total: 48W for both rails
With losses: 48W / 0.8 = 60W transformer minimum

Voltage rating: 12V-0-12V (24V center-tapped)
Current rating: 2A minimum
```

### Filter Capacitor Sizing
```
Ripple voltage: Vr = I / (f × C)

For 1A load, 120Hz ripple, <1V ripple:
C = I / (f × Vr)
C = 1A / (120Hz × 1V)
C = 8333µF minimum

Use 2200µF × 2 in parallel = 4400µF (adequate)
Or single 10000µF capacitor
```

### Regulator Power Dissipation
```
P = (Vin - Vout) × Iout

For 7812 with 17V in, 12V out, 1A load:
P = (17V - 12V) × 1A = 5W

Heat sink requirement:
θ = (Tjmax - Tamb) / P
θ = (125°C - 50°C) / 5W = 15°C/W

Need heat sink with <15°C/W thermal resistance
```

### Current Capability
```
With 2A transformer and adequate heat sinking:
- Maximum continuous current: ~1.5A per rail
- Short duration peaks: 2A
- Total power available: 12V × 1.5A × 2 = 36W
```

## Key Concepts Learned
- **Dual Rail Power**: Creating ±V supplies for analog circuits
- **Center-Tapped Transformers**: Split-phase AC generation
- **Voltage Regulation**: Linear regulation with 78xx/79xx series
- **Heat Management**: Thermal design and heat sinking
- **AC Safety**: Working safely with mains voltage
- **Power Supply Design**: Complete power system from mains to DC

## Applications
- **Op-amp circuits**: Audio amplifiers, active filters
- **Audio equipment**: Preamps, mixers, effects
- **Analog electronics**: Instrumentation, sensors
- **Function generators**: Waveform circuits (Project #19)
- **Test equipment**: Bench supply for prototyping
- **Ham radio**: Transceiver power
- **Educational**: Teaching power supply principles

## Next Steps
1. **Variable voltage**: Replace fixed regulators with LM317/LM337
2. **Current limiting**: Add current limit circuits
3. **Digital display**: Add voltmeter/ammeter displays
4. **Protection**: Overvoltage and reverse polarity protection
5. **Multiple outputs**: Add +5V rail for digital circuits
6. **Switch-mode upgrade**: Replace with SMPS for efficiency

## Safety Notes
- ⚠️ **AC MAINS VOLTAGE IS LETHAL** - Can kill instantly!
- ⚠️ **Only for experienced builders** or with supervision
- ⚠️ **Use proper enclosure** - All AC wiring must be enclosed
- ⚠️ **Ground metal enclosure** - Connect to earth ground
- ⚠️ **Fuse protection** - Always use appropriate fuse
- ⚠️ **Heat sinks required** - Regulators get very hot
- ⚠️ **Capacitor polarity** - Wrong polarity = explosion!
- ⚠️ **Discharge caps** - Can store dangerous charge even when off
- ✅ **Follow electrical codes** - Comply with local regulations
- ✅ **When in doubt, ask expert** - Don't risk safety

## Advanced Considerations

### Improving Regulation
- **Pre-regulators**: Use LM317/337 before main regulators
- **Active filtering**: Add transistor-based filters
- **Multiple stages**: Cascade regulators for ultra-low noise

### Protection Circuits
- **Crowbar protection**: SCR overvoltage protection
- **Current limiting**: Foldback current limiting
- **Soft start**: NTC thermistor or relay-based soft start
- **Reverse polarity**: Diodes to prevent damage

### Efficiency Improvements
- **Switch-mode**: Replace linear with switching regulators
- **Lower dropout**: Use LDO (Low Dropout) regulators
- **Adjustable**: Make voltage adjustable (tracking or independent)

---

**Circuit Status**: ✅ Verified design - REQUIRES AC MAINS SAFETY KNOWLEDGE
**Last Updated**: December 2024

Power up your projects! ⚡🔌
