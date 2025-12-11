# Simple Audio Amplifier ⭐⭐⭐☆☆

## Difficulty Level
**Beginner** - Introduction to audio circuits

## What You'll Learn
- Amplification basics
- Audio circuit design
- Capacitive coupling
- How amplifier ICs work

## Components Required
- LM386 Audio Amplifier IC × 1
- Capacitors:
  - 10µF electrolytic × 2
  - 100µF electrolytic × 1
  - 220µF electrolytic × 1
  - 100nF ceramic × 1
- Resistor 10Ω × 1
- 8Ω Speaker × 1
- 3.5mm audio jack × 1
- 9V battery × 1
- Breadboard and jumper wires

## Circuit Description
A basic audio amplifier that can boost signals from your phone or MP3 player to drive a speaker. The LM386 is a simple, low-power audio amplifier IC perfect for beginners. It requires minimal external components and can deliver up to 1W of power.

## Theory of Operation
The LM386 amplifies audio signals with a default gain of 20 (26dB). By adding a capacitor between pins 1 and 8, you can increase the gain up to 200. Input coupling capacitors block DC while passing AC audio signals.

### Gain Configuration
- **No external components**: Gain = 20
- **10µF capacitor between pins 1-8**: Gain = 200

### Pin Configuration (8-pin DIP)
```
    +--U--+
G+  |1   8| VCC (+9V)
IN- |2   7| BYPASS
IN+ |3   6| OUT
GND |4   5| OUT
    +-----+
```

## Building Instructions
1. Power: Connect pin 4 to GND, pin 6 to +9V
2. Input: Connect audio input through 10µF cap to pin 3
3. Gain (optional): Add 10µF between pins 1 and 8 for higher gain
4. Bypass: Connect 100nF from pin 7 to GND
5. Output: Connect speaker through 220µF cap to pin 5
6. Zobel network: 10Ω resistor + 100nF in series from output to GND (reduces oscillation)

## Key Concepts
- Signal amplification
- Audio circuits
- Capacitive coupling
- Input/output impedance

## Applications
- Portable speakers
- Intercom systems
- Audio monitors
- Guitar amplifiers (practice)

---

**Circuit Status**: 📋 Awaiting schematic and photos
**Last Updated**: December 2024

Amplify your signals! 🔊⚡
