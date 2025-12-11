# LED Chaser/Sequencer (4017 IC) ⭐⭐⭐☆☆

## Difficulty Level
**Beginner** - Learn about decade counters

## What You'll Learn
- Decade counters and how they work
- Clock signals and timing
- Sequential logic circuits
- Combining multiple ICs

## Components Required
- CD4017 Decade Counter IC × 1
- 555 Timer IC × 1
- LEDs (various colors) × 10
- Resistors:
  - 220Ω × 10 (for LEDs)
  - 1kΩ × 1
  - 10kΩ × 1
- Capacitors:
  - 10µF × 1
  - 100nF × 2
- 9V battery × 1
- Breadboard and jumper wires

## Circuit Description
A running light display where LEDs light up one after another in sequence, creating a "chaser" or "Knight Rider" effect. The 555 timer generates clock pulses, and the CD4017 decade counter lights each LED in turn (Q0 through Q9).

## Theory of Operation

### CD4017 Decade Counter
- **10 decoded outputs** (Q0-Q9)
- **Counts on rising edge** of clock input
- **One output HIGH at a time** (sequential)
- **Resets to Q0** after Q9 or via RESET pin

### How It Works
1. 555 timer generates clock pulses (astable mode)
2. Each clock pulse advances the counter
3. Counter activates outputs sequentially: Q0→Q1→Q2...→Q9→Q0
4. Each output drives one LED
5. Adjust 555 frequency to control chase speed

## Pin Configuration

### CD4017 (16-pin DIP)
```
     +--U--+
 Q5  |1  16| VCC
 Q1  |2  15| RESET
 Q0  |3  14| CLK
 Q2  |4  13| EN
 Q6  |5  12| OUT
 Q7  |6  11| Q4
 Q3  |7  10| Q8
 GND |8   9| Q9
     +------+
```

## Building Instructions

### Step 1: Build 555 Timer Clock
Follow the 555 astable configuration (Project #1):
- R1 = 1kΩ, R2 = 10kΩ, C = 10µF
- This gives ~7Hz clock rate (visible chasing)

### Step 2: Setup CD4017
1. Power: Pin 16 to +9V, Pin 8 to GND
2. Clock input: Pin 14 to 555 output (pin 3)
3. Enable: Pin 13 to GND (always enabled)
4. Reset: Pin 15 to GND (or use for early reset)

### Step 3: Connect LEDs
Connect 10 LEDs to outputs Q0-Q9:
- Each LED anode → Output pin (Q0-Q9)
- Each LED cathode → 220Ω resistor → GND

### Step 4: Test
Power on and watch the LEDs chase!

## Experimentation Ideas

### Change Speed
- **Faster**: Decrease 555 R2 or C value
- **Slower**: Increase 555 R2 or C value
- Add potentiometer for variable speed!

### Reduce LED Count
Connect RESET (pin 15) to desired output to reset early:
- Q3 → Reset: Creates 3-LED chaser
- Q5 → Reset: Creates 5-LED chaser

### Bidirectional Chase
Use two 4017s counting in opposite directions!

### Add Sound
Connect piezo buzzer to each output through different value resistors for musical tones

## Key Concepts
- Digital counters
- Clock signals
- Sequential circuits
- Decoded outputs

## Applications
- Running lights
- Turn signal indicators
- Visual displays
- LED animations
- Position indicators

## Next Steps
- Traffic Light Controller (Project #18) - Sequential state machine
- Digital Dice (Project #16) - More counter applications
- Stepper Motor Driver (Project #22) - Use 4017 for motor sequencing

---

**Circuit Status**: 📋 Awaiting schematic and photos
**Last Updated**: December 2024

Chase those lights! 💫⚡
