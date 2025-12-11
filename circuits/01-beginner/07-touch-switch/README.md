# Touch Switch ⭐⭐☆☆☆

## Difficulty Level
**Beginner** - Introduction to capacitive sensing

## What You'll Learn
- High-impedance sensing
- Capacitive touch detection
- Logic gates and Schmitt triggers
- Touch sensor circuits

## Components Required
- CD4011 NAND Gate IC (or 74HC14 Schmitt Trigger) × 1
- BC547 NPN Transistor × 1
- Resistors:
  - 1MΩ × 1
  - 10kΩ × 1
  - 220Ω × 1
- Capacitor 100nF × 1
- LED or relay × 1
- Touch plate (any conductive material: coin, aluminum foil, copper) × 1
- 9V battery × 1
- Breadboard and jumper wires

## Circuit Description
Turn on an LED or device by touching a metal plate! This circuit uses high-impedance input to detect the small capacitance change from your finger. The Schmitt trigger or NAND gate provides clean switching, and the transistor drives the load.

## Theory of Operation

### Capacitive Sensing
Your body has capacitance (~100pF). When you touch the sensor plate:
1. Body capacitance couples to the touch pad
2. This changes the input impedance
3. High-value resistor (1MΩ) creates high-impedance input
4. Logic gate detects voltage change
5. Transistor amplifies signal to drive LED/relay

### Why Schmitt Trigger?
Schmitt triggers have hysteresis - two different threshold voltages:
- **Upper threshold**: ~3.5V
- **Lower threshold**: ~1.5V
This prevents oscillation and provides clean on/off switching.

## Pin Configuration

### CD4011 Quad NAND Gate
```
     +--U--+
 1A  |1  14| VCC
 1B  |2  13| 4B
 1Y  |3  12| 4A
 2A  |4  11| 4Y
 2B  |5  10| 3B
 2Y  |6   9| 3A
 GND |7   8| 3Y
     +------+
```

## Building Instructions

### Using 74HC14 Schmitt Trigger (Recommended)
1. Power: VCC to +9V, GND to 0V
2. Touch pad → 1MΩ resistor → Schmitt trigger input (pin 1)
3. Connect 10kΩ pull-down resistor from input to GND
4. Connect 100nF capacitor from input to GND (filtering)
5. Schmitt output (pin 2) → transistor base through 10kΩ
6. Transistor drives LED/relay

### Using CD4011 NAND Gates
1. Wire two NAND gates as inverters (connect inputs together)
2. Use as Schmitt trigger substitute
3. Rest of circuit same as above

## Testing & Troubleshooting

### Expected Behavior
- Touch pad: LED turns ON
- Release pad: LED turns OFF (or stays on if latching)
- No physical pressing required!

### Common Issues

**Too sensitive (triggers without touch):**
- Reduce input resistor (try 470kΩ)
- Add more filtering capacitance
- Shield sensor from interference

**Not sensitive enough:**
- Increase input resistor (try 2.2MΩ or 10MΩ)
- Increase touch pad size
- Ensure good connection to touch pad

**Oscillates or flickers:**
- Add more filtering (increase capacitor to 1µF)
- Use Schmitt trigger IC (74HC14)
- Shorten wires to touch pad

## Advanced Features

### Latching Touch Switch
Add an SR latch (Project #8) to make it toggle on/off with each touch!

### Multiple Touch Pads
Use multiple Schmitt triggers for several touch points

### Touch Dimmer
Use touch to control PWM for LED dimming

## Key Concepts
- Capacitive sensing
- High-impedance inputs
- Schmitt triggers
- Touch interfaces

## Applications
- Touch lamps
- Touchless switches
- Capacitive buttons
- Proximity detection
- Musical instruments (Theremin-like)

## Safety Notes
- ✅ Safe low-voltage circuit
- ⚠️ If controlling AC loads with relay, follow electrical safety!
- ⚠️ Touch pad must be isolated from mains voltage

---

**Circuit Status**: 📋 Awaiting schematic and photos
**Last Updated**: December 2024

Touch the future! ✋⚡
