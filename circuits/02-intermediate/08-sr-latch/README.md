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

## How It Works
- Press **SET** button → Output goes HIGH (LED on) and **stays on**
- Press **RESET** button → Output goes LOW (LED off) and **stays off**  
- The circuit maintains its state even after you release the buttons
- This is **digital memory** in its simplest form!

## Theory of Operation

### SR Latch Truth Table
| S | R | Q | Q̄ | State |
|---|---|---|---|-------|
| 0 | 0 | Q | Q̄ | Hold (no change) |
| 1 | 0 | 1 | 0 | Set |
| 0 | 1 | 0 | 1 | Reset |
| 1 | 1 | X | X | Invalid (avoid!) |

### Using NAND Gates
Two cross-coupled NAND gates create the bistable latch:
- Output of gate 1 feeds input of gate 2
- Output of gate 2 feeds input of gate 1
- This positive feedback creates two stable states

## Building Instructions

### Using CD4011 (NAND Gates)
1. Power: Pin 14 to +5V, Pin 7 to GND
2. **Gate 1** (pins 1, 2, 3):
   - Pin 1: SET button (active LOW with pull-up)
   - Pin 2: Connect to Gate 2 output (pin 4)
   - Pin 3: Q output → LED 1
3. **Gate 2** (pins 5, 6, 4):
   - Pin 5: RESET button (active LOW with pull-up)
   - Pin 6: Connect to Gate 1 output (pin 3)
   - Pin 4: Q̄ output → LED 2

### Button Configuration
- One side of button to input pin
- Other side to GND
- 10kΩ pull-up resistor from input pin to +5V

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

## Next Steps
- Digital Dice (Project #16) - Uses latches
- Bistable Relay (Project #15) - Mechanical memory
- Build a 4-bit register using 4 SR latches!

---

**Circuit Status**: ✅ Fritzing file exists (SR-Latch.fzz)
**Last Updated**: December 2024

Remember everything! 🧠💾
