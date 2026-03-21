# SR Latch (Set-Reset Flip-Flop) ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Introduction to digital memory

## What You'll Learn
- Digital memory fundamentals
- Flip-flops and latches
- Basic logic gates
- Bistable circuits

## Components Required
- CD4011 Quad NAND Gate IC (or CD4001 Quad NOR Gate IC) × 1
- Push buttons × 2
- LEDs × 2
- Resistors:
  - 220Ω × 2 (for LEDs)
  - 10kΩ × 2 (pull-down/up)
- 5V power supply (or 9V with regulator)
- Breadboard and jumper wires

## Circuit Description
Build a fundamental memory element - the SR Latch! This circuit "remembers" which button was pressed last and holds that state. It's the building block of all computer memory and a perfect introduction to digital logic and memory circuits.

## How It Works:

The SR latch is the simplest form of digital memory.
It remembers the last button pressed:

* Press **SET** → Output turns ON and stays ON
* Press **RESET** → Output turns OFF and stays OFF

Even after you release the button, the state remains.

The circuit has **two NAND gates connected to each other**.

Each gate feeds back into the other.

This feedback creates two stable states:

* Q = HIGH, Q̄ = LOW
* Q = LOW, Q̄ = HIGH

Once it enters one of those states, it stays there until forced to change.

---

## Theory of Operation

### SR Latch Truth Table
| S | R | Q | Q̄ | Meaning          |
| - | - | - | -- | ---------------- |
| 1 | 1 | Q | Q̄ | Hold (no change) |
| 0 | 1 | 1 | 0  | Set              |
| 1 | 0 | 0 | 1  | Reset            |
| 0 | 0 | X | X  | Invalid (avoid)  |

### Using NAND Gates
Two cross-coupled NAND gates create the bistable latch:
- Output of gate 1 feeds input of gate 2
- Output of gate 2 feeds input of gate 1
- This positive feedback creates two stable states

## CD4011 Pinout Diagram
```
        +-----U-----+
  1A  1 |•        14| VDD (+5V)
  1B  2 |         13| 4B
  1Y  3 |         12| 4A
  2Y  4 |  CD4011 11| 4Y
  2A  5 |         10| 3Y
  2B  6 |          9| 3A
  GND 7 |__________8| 3B
```
---

## Building Instructions

### Using CD4011 (NAND Gates)

## 1️⃣ Power Connections

* Pin 14 → +5V
* Pin 7 → GND

---

## 2️⃣ Gate Wiring

### Gate 1 (SET side)

* Pin 1 → SET input
* Pin 2 → Connect to Pin 4 (output of Gate 2)
* Pin 3 → Q output → LED + 220Ω → GND

---

### Gate 2 (RESET side)

* Pin 5 → RESET input
* Pin 6 → Connect to Pin 3 (output of Gate 1)
* Pin 4 → Q̄ output → LED + 220Ω → GND

---

## 3️⃣ Button Wiring (Important)

Each button needs a pull-up resistor.

For BOTH SET and RESET:

```
+5V
  |
 [10k]
  |
  +-----> To input pin (Pin 1 or Pin 5)
  |
 (button)
  |
 GND
```
* Connect one pin of the button to the 10k resistor
* Connect the other end of the resistor to 5V
* Connect the same pin to pin 1 (for set) or pin 2 (for reset)

Explanation:

* Not pressed → input = HIGH
* Pressed → input = LOW

That LOW triggers the NAND latch.

Do NOT use pull-down resistors here.

## Testing & Troubleshooting

### Expected Behavior
- Press SET: Q LED lights, Q̄ LED turns off, **state holds**
- Press RESET: Q LED turns off, Q̄ LED lights, **state holds**
- Releasing buttons: **State remains unchanged**

### Common Issues
**Both LEDs on or off:**
- Check cross-coupling connections
- Verify power connections
- Try pressing buttons to initialize

**State doesn't hold:**
- Check feedback connections between gates
- Verify IC is working (test individual gates)

**No response to buttons:**
- Check button connections and pull-up resistors
- Verify buttons are momentary (not latching)

## Key Concepts
- Digital memory
- Bistable circuits
- Logic gates
- Flip-flops (building blocks of registers and RAM!)

## Applications
- Memory elements in computers
- Debounced switches
- State storage
- Toggle switches
- Foundation of all digital memory!

## Project Ideas:

* Build a toggle switch using an SR latch
* Create a 4-bit register using 4 latches
* Combine with a clock to explore flip-flops


## Next Steps
- Digital Dice (Project #16) - Uses latches
- Bistable Relay (Project #15) - Mechanical memory
- Build a 4-bit register using 4 SR latches!

---

**Circuit Status**: ✅ Completed

**Last Updated**: February 2026
