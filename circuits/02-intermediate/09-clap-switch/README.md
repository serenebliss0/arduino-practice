# Clap Switch (Sound-Activated) ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Sound detection and flip-flops

## What You'll Learn
- Sound detection circuits
- Operational amplifiers
- Flip-flops and toggle circuits
- Relay control

## Components Required
- Electret microphone × 1
- LM358 Dual Op-Amp IC × 1
- CD4017 Decade Counter IC × 1
- BC547 Transistor × 1
- 5V Relay × 1
- Diode 1N4007 × 1 (flyback protection)
- Resistors: 1kΩ, 10kΩ, 100kΩ (various)
- Capacitors: 10µF, 100nF (various)
- 9V battery
- Load (LED or AC device through relay)
- Breadboard and jumper wires

## Circuit Description
Turn devices on/off by clapping! The microphone detects sound, the op-amp amplifies it and filters noise, and the 4017 counter toggles the relay. A classic beginner-intermediate project that combines analog and digital circuits.

## How It Works
1. Clap → Microphone generates small AC voltage
2. Op-amp amplifies audio signal
3. Envelope detector converts to DC pulse
4. Pulse clocks 4017 counter
5. Counter output toggles relay
6. Relay switches connected device

## Circuit Stages

### Stage 1: Microphone Preamp
- Electret mic with bias resistor
- AC coupling capacitor
- Op-amp configured as non-inverting amplifier
- Gain: ~100x

### Stage 2: Envelope Detector
- Peak detector using diode and capacitor
- Converts audio bursts to DC pulses

### Stage 3: Toggle Circuit
- CD4017 counter configured as divide-by-2 (toggle)
- Q0 and Q1 alternately activate
- Creates flip-flop behavior

### Stage 4: Relay Driver
- Transistor amplifies counter output
- Drives relay coil
- Flyback diode protects transistor

## Key Concepts
- Audio detection
- Operational amplifiers
- Envelope detection
- Toggle flip-flops
- Relay switching

## Applications
- Room lights (clap on, clap off!)
- Appliance control
- Demonstrations
- Magic tricks
- Accessibility aids

---

**Circuit Status**: 📋 Awaiting schematic and photos
**Last Updated**: December 2024

Clap on! Clap off! 👏💡
