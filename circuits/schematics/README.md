# 📐 Schematics and Reference Designs

## Purpose
This directory contains general schematics, reference designs, and circuit building blocks that can be used across multiple projects.

## Contents

### Reference Circuits
- Common circuit building blocks
- Standard configurations for popular ICs
- Tested and verified designs

### Component Reference
- IC pinout diagrams
- Common component values
- Substitution guides

### Design Templates
- Power supply circuits
- Amplifier stages
- Filter circuits
- Oscillator designs
- Logic gate configurations

## Useful Reference Designs

### Power Supplies
- **5V Regulator (7805)**: Standard configuration with input/output caps
- **Variable Regulator (LM317)**: Adjustable voltage with calculation formulas
- **Dual Supply (±12V)**: Center-tapped transformer design

### Amplifiers
- **Common Emitter Amplifier**: Basic transistor amplifier stage
- **Op-Amp Configurations**: Inverting, non-inverting, differential
- **Audio Amp (LM386)**: Standard audio amplifier circuit

### Oscillators and Timers
- **555 Astable**: Basic oscillator with frequency calculations
- **555 Monostable**: One-shot timer configuration
- **Crystal Oscillator**: Precision timing circuits

### Logic and Digital
- **Debounce Circuit**: Clean switch inputs
- **SR Latch**: Basic flip-flop using gates
- **Clock Divider**: Using counter ICs

### Motor Control
- **H-Bridge**: Bidirectional motor control
- **Stepper Driver**: Unipolar and bipolar configurations
- **PWM Controller**: Variable speed control

### Sensors and Interfaces
- **LDR Light Sensor**: Light detection with threshold
- **Temperature Sensor**: LM35 interfacing
- **IR Transmitter/Receiver**: Proximity detection

## Adding Schematics

### File Formats
- **Fritzing (.fzz)**: For breadboard and schematic views
- **PDF**: For easy viewing and printing
- **PNG/JPG**: For quick reference
- **KiCad files**: For PCB design

### Naming Convention
```
[function]-[ic/component]-[variant].ext

Examples:
- 555-astable-1khz.fzz
- lm386-amplifier-min-gain.pdf
- 7805-regulator-standard.png
```

### Documentation
Each schematic should include:
1. **Title and description**
2. **Component values**
3. **Key formulas** (if applicable)
4. **Input/output specifications**
5. **Notes and warnings**

## Common IC Pinouts

### 555 Timer (8-pin DIP)
```
    +--U--+
GND |1   8| VCC
TRG |2   7| DIS
OUT |3   6| THR
RST |4   5| CTL
    +-----+
```

### LM358/LM324 Op-Amp
```
LM358 (Dual)        LM324 (Quad)
    +--U--+             +--U--+
1O  |1   8| VCC     1O  |1  14| 4O
1-  |2   7| 2O      1-  |2  13| 4-
1+  |3   6| 2-      1+  |3  12| 4+
GND |4   5| 2+      VCC |4  11| GND
    +-----+         2O  |5  10| 3+
                    2-  |6   9| 3-
                    2+  |7   8| 3O
                        +------+
```

### CD4017 Decade Counter
```
     +--U--+
 Q5  |1  16| VCC
 Q1  |2  15| RST
 Q0  |3  14| CLK
 Q2  |4  13| EN
 Q6  |5  12| OUT
 Q7  |6  11| Q4
 Q3  |7  10| Q8
 GND |8   9| Q9
     +------+
```

## Standard Component Values

### Resistors (E12 Series)
10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82 (×10ⁿ)

Common values: 220Ω, 1kΩ, 10kΩ, 100kΩ, 1MΩ

### Capacitors
**Ceramic (nF)**: 1, 10, 100, 470
**Electrolytic (µF)**: 1, 10, 100, 470, 1000

### LEDs
- Forward Voltage: 1.8-3.6V (depending on color)
- Forward Current: 10-20mA (typical)
- Current Limiting Resistor: 220Ω - 1kΩ

## Circuit Design Tips

### Power Supply Guidelines
1. Always add 100nF bypass capacitor near each IC
2. Use electrolytic caps (10µF-100µF) for bulk filtering
3. Add reverse polarity protection (diode)
4. Calculate power dissipation for regulators
5. Use adequate heatsinks for power components

### Signal Conditioning
1. Use voltage dividers for scaling
2. Add RC filters for noise reduction
3. Buffer high-impedance signals with op-amps
4. Decouple analog and digital grounds

### Protection Circuits
1. Flyback diodes for inductive loads (motors, relays)
2. TVS diodes for ESD protection
3. Fuses for overcurrent protection
4. Zener diodes for voltage clamping

## Testing and Troubleshooting

### Essential Measurements
1. **Power rails**: Verify correct voltages
2. **IC pins**: Check VCC and GND first
3. **Signal paths**: Trace signals with multimeter/oscilloscope
4. **Component values**: Verify with multimeter

### Common Problems
- **No power**: Check connections and battery
- **Intermittent operation**: Loose connections
- **Incorrect output**: Wrong component values
- **Overheating**: Insufficient current limiting or heatsinking

## Resources

### Online Simulators
- **Falstad Circuit Simulator**: https://falstad.com/circuit/
- **CircuitLab**: https://www.circuitlab.com/
- **LTspice**: https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html - Free SPICE simulator by Analog Devices

### Reference Websites
- **All About Circuits**: https://www.allaboutcircuits.com/
- **Electronics Tutorials**: https://www.electronics-tutorials.ws/
- **TI Precision Labs**: Training videos from Texas Instruments

### Datasheet Sources
- Manufacturer websites (TI, Analog, ON Semi, etc.)
- **AllDataSheet**: https://www.alldatasheet.com/
- **DataSheetCatalog**: https://www.datasheetcatalog.com/

## Contributing

### Adding New Schematics
1. Create clear, well-documented schematic
2. Test circuit on breadboard
3. Export in multiple formats (Fritzing, PDF, PNG)
4. Add README with full documentation
5. Include component list and values
6. Add notes about variations and alternatives

### Quality Guidelines
- ✅ Clean, readable schematic layout
- ✅ All components labeled with values
- ✅ Clear input/output labels
- ✅ Include test points
- ✅ Add notes for critical values
- ✅ Specify power requirements

## Safety Reminders

### Low Voltage Circuits (< 30V DC)
- ✅ Generally safe to touch
- ⚠️ Still respect polarity and component ratings

### High Voltage / AC Circuits
- ⚠️ **DANGEROUS** - Can be lethal!
- ⚠️ Never work on live AC circuits
- ⚠️ Use isolated transformers for testing
- ⚠️ Follow electrical codes for permanent installations
- ⚠️ Consider consulting qualified electrician

### Component Safety
- ⚠️ Watch capacitor polarity (can explode!)
- ⚠️ Use heatsinks for power components
- ⚠️ Don't exceed maximum IC voltages
- ⚠️ Add flyback diodes for relays/motors
- ⚠️ Fuse all power supplies

---

**Status**: 📋 Awaiting reference schematics and designs
**Last Updated**: December 2024

Build with confidence! 📐⚡
