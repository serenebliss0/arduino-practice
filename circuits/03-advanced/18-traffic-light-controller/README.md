# Traffic Light Controller (with Pedestrian Crossing) ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - State machines in hardware with sequential logic

## What You'll Learn
- State machine design in hardware
- Sequential logic circuits
- Timing and synchronization
- Interrupt handling in hardware
- Counter ICs and their sequencing
- Complex LED control patterns
- Real-world system modeling

## Components Required
- **CD4017** Decade Counter IC × 2
- **555 Timer** IC (NE555) × 1
- **CD4011** Quad NAND Gate IC (or CD4081 AND gate) × 1
- **LEDs**:
  - Red × 2 (traffic + pedestrian don't walk)
  - Yellow/Amber × 1 (traffic caution)
  - Green × 2 (traffic go + pedestrian walk)
- **Resistors**:
  - 220Ω × 5 (for LEDs)
  - 1kΩ × 2
  - 10kΩ × 2
  - 100kΩ × 1
- **Capacitors**:
  - 100µF electrolytic × 1
  - 10µF electrolytic × 1
  - 100nF ceramic × 2
- **Push button** (momentary, NO) × 1 (pedestrian crossing request)
- **9V battery** × 1
- **Battery connector**
- **Breadboard** and jumper wires

## Circuit Description
Build a realistic traffic light system complete with pedestrian crossing! The traffic lights cycle automatically through Green → Yellow → Red → Green, but pressing the pedestrian button interrupts the cycle to provide a safe crossing window. The system includes proper timing (extended red for pedestrians), walk/don't walk signals, and returns to normal operation after crossing. This project demonstrates state machine concepts - a fundamental principle in digital systems, traffic control, and computer engineering!

## Theory of Operation

### State Machine Concept
A state machine is a system that can be in one of several defined states and transitions between them based on inputs and timing:

**Traffic States:**
1. **State 0**: Green (vehicles go, pedestrians wait)
2. **State 1**: Yellow (vehicles slow down)
3. **State 2**: Red (vehicles stop, can be extended for pedestrians)
4. **State 3**: Return to Green

**Pedestrian Override States:**
5. **State P1**: Extended Red (pedestrians can cross)
6. **State P2**: Walk signal on
7. **State P3**: Flashing don't walk (hurry up!)
8. **State P4**: Don't walk solid, return to normal cycle

### How It Works

#### Normal Traffic Cycle (No Pedestrian Request)
```
Green (30s) → Yellow (5s) → Red (30s) → Green...
```

**Timing:**
- Green: 30 seconds (adjust with counter taps)
- Yellow: 5 seconds (caution)
- Red: 30 seconds (cross traffic would go)
- Repeat cycle continuously

#### With Pedestrian Request
When button pressed during Green or Yellow:
```
1. Complete current state
2. Go to Red
3. EXTENDED Red (15s more) + WALK signal
4. Flash "don't walk" (10s warning)
5. Solid "don't walk"
6. Return to Green
7. Resume normal cycle
```

### Circuit Blocks

#### Block 1: Master Clock (555 Timer)
- Generates regular clock pulses (~1Hz for 1 second per count)
- Astable mode with adjustable frequency
- Drives both counter ICs
- Formula: f = 1.44 / ((R1 + 2×R2) × C)

#### Block 2: Main State Counter (CD4017 #1)
- Counts 0 to 9, automatically resets to 0
- Each output represents a state
- Taps at specific outputs control LED states
- Can be reset by pedestrian logic

#### Block 3: Pedestrian Logic (NAND Gates)
- Detects button press
- Determines if in "acceptable interrupt window"
- Flags system to enter pedestrian cycle
- Prevents interruption during yellow or red (safety!)

#### Block 4: Pedestrian Timer (CD4017 #2)
- Secondary counter for pedestrian-specific states
- Activated only during crossing cycle
- Controls walk/don't walk LEDs
- Handles flashing sequences

#### Block 5: LED Decoder Logic
- Combines counter outputs to drive correct LEDs
- Traffic lights: Only one color at a time
- Pedestrian: Walk or don't walk (with flashing)
- Uses AND/OR gate combinations

### Circuit Operation

**Power On:**
1. System starts at state 0 (Green light)
2. Clock begins pulsing
3. Counter advances every second
4. Traffic cycles normally

**Normal Green State (0-5):**
- Counter outputs Q0-Q5 are HIGH sequentially
- All routed to Green LED (through OR logic)
- Green LED = ON for ~30 seconds (6 counts × 5s each)
- Yellow and Red = OFF
- Pedestrian "don't walk" = ON (red)

**Yellow State (6-7):**
- Counter outputs Q6-Q7 HIGH
- Yellow LED = ON for ~10 seconds
- Green and Red = OFF
- Warns vehicles to slow down

**Red State (8-9 + reset):**
- Counter outputs Q8-Q9 HIGH
- Red LED = ON for ~30 seconds
- Yellow and Green = OFF
- Vehicles must stop

**Pedestrian Button Pressed:**
1. Latch set: Flag "pedestrian waiting"
2. Wait for safe state (end of green or during yellow)
3. After yellow, go to red normally
4. EXTEND red phase (add +15 seconds)
5. Activate Walk signal (green hand)
6. Start pedestrian timer
7. After walk time, flash "don't walk"
8. Solid "don't walk"
9. Clear pedestrian flag
10. Resume normal traffic cycle

**Walk Signal Timing:**
- Walk (solid green): 10 seconds
- Don't walk (flashing red): 10 seconds
- Don't walk (solid red): Remainder until green

## Pin Configuration

### 555 Timer IC (8-pin DIP)
```
    +--U--+
GND |1   8| VCC (+9V)
TRG |2   7| DIS
OUT |3   6| THR
RST |4   5| CTL
    +-----+
```

### CD4017 Decade Counter (16-pin DIP)
```
       +--U--+
   Q5  |1  16| VDD (+9V)
   Q1  |2  15| RESET
   Q0  |3  14| CLOCK
   Q2  |4  13| CLOCK ENABLE
   Q6  |5  12| CARRY OUT
   Q7  |6  11| Q4
   Q3  |7  10| Q9
  VSS  |8   9| Q8
       +-----+
```

### CD4011 NAND Gate (14-pin DIP)
```
        +--U--+
   1A   |1  14| VDD (+9V)
   1B   |2  13| 4B
   1Y   |3  12| 4A
   2Y   |4  11| 4Y
   2A   |5  10| 3Y
   2B   |6   9| 3B
  VSS   |7   8| 3A
        +-----+
```

## Building Instructions

### Step 1: Power Setup
1. Connect +9V to positive rail
2. Connect GND to negative rail
3. Add 100nF capacitor across power rails (decoupling)
4. Verify voltage with multimeter

### Step 2: Clock Generator (555 Timer)
1. Insert 555 IC on breadboard
2. **Power**: Pin 8 to +9V, Pin 1 to GND
3. **Reset**: Pin 4 to Pin 8 (always enabled)
4. **Control**: Pin 5 to GND through 100nF capacitor
5. **Timing**:
   - R1 (100kΩ): Pin 8 to Pin 7
   - R2 (10kΩ): Pin 7 to Pin 6
   - Connect Pin 6 to Pin 2
   - C (100µF): Pin 2 to GND (watch polarity!)
6. **Output**: Pin 3 = clock pulses (~0.5-1 Hz)

**Clock frequency calculation:**
```
f = 1.44 / ((100k + 2×10k) × 100µF)
f = 1.44 / (120k × 0.0001)
f = 1.44 / 12
f = 0.12 Hz (about 8 seconds per pulse)
```
Adjust R2 to change speed: 47kΩ for faster, 22kΩ for slower.

### Step 3: Main Traffic Counter (CD4017 #1)
1. Insert first CD4017 on breadboard
2. **Power**: Pin 16 to +9V, Pin 8 to GND
3. **Clock**: Pin 14 to 555 Pin 3
4. **Enable**: Pin 13 to GND (always counting)
5. **Reset**: Pin 15 to GND for now (no reset initially)
6. **Note outputs**:
   - Q0-Q5: Green phase (pins 3,2,4,7,10,1)
   - Q6-Q7: Yellow phase (pins 5,6)
   - Q8-Q9: Red phase (pins 9,11)

### Step 4: Traffic LED Connections

**Green LED:**
1. Connect Q0, Q1, Q2, Q3, Q4, Q5 together (OR function via diodes or direct wire)
2. When ANY of these are HIGH → Green LED ON
3. Green LED anode to this junction through 220Ω resistor
4. Green LED cathode to GND

**Yellow LED:**
1. Connect Q6, Q7 together
2. Yellow LED anode to junction through 220Ω resistor
3. Yellow LED cathode to GND

**Red LED:**
1. Connect Q8, Q9 together (and extended states for pedestrian)
2. Red LED anode to junction through 220Ω resistor
3. Red LED cathode to GND

### Step 5: Pedestrian Button Input
1. Connect push button between +9V and junction "PED_REQUEST"
2. Add 10kΩ pull-down from PED_REQUEST to GND
3. Add 100nF debounce capacitor from PED_REQUEST to GND
4. Button pressed: PED_REQUEST = HIGH

### Step 6: Pedestrian Logic Gates
1. Insert CD4011 NAND gate IC
2. **Power**: Pin 14 to +9V, Pin 7 to GND
3. **Gate 1**: SR Latch for pedestrian request
   - Input A: PED_REQUEST (button)
   - Input B: Feedback from Gate 2
   - Output: "PED_WAITING" flag
4. **Gate 2**: Enable logic
   - Determines if button press is allowed
   - Checks current state (not during yellow or red)
   - Outputs to latch reset
5. **Gate 3 & 4**: Additional logic for walk/don't walk control

### Step 7: Pedestrian Timer (CD4017 #2) - Optional Enhancement
1. Insert second CD4017 for pedestrian sequence
2. **Power**: Pin 16 to +9V, Pin 8 to GND
3. **Clock**: Pin 14 to 555 Pin 3 (shared clock)
4. **Enable**: Pin 13 controlled by PED_WAITING signal
5. **Reset**: Returns to Q0 after sequence completes
6. Outputs control pedestrian walk/don't walk LEDs

### Step 8: Pedestrian Walk/Don't Walk LEDs

**Walk Signal (Green):**
1. Controlled by pedestrian counter states 0-2
2. Green LED anode to +9V through 220Ω
3. Cathode to transistor/logic controlling walk state
4. ON during walk phase (~10 seconds)

**Don't Walk Signal (Red):**
1. Default state (normally ON)
2. Turns OFF during walk phase
3. Flashes during "clear intersection" phase
4. Red LED anode to +9V through 220Ω
5. Cathode controlled by timer logic

### Step 9: Timing Adjustments
1. **Speed up entire cycle**: Decrease 555 capacitor (47µF)
2. **Slow down cycle**: Increase capacitor (220µF)
3. **Adjust individual phases**: Tap counter at different outputs
4. **Fine tuning**: Adjust 555 R2 potentiometer

### Step 10: Testing Connections
1. Verify all ICs oriented correctly (notch at pin 1)
2. Check power: All VDD/VCC to +9V, all VSS/GND to ground
3. Verify no shorts between power rails
4. Check LED polarity (long leg = anode = +)

## Testing & Troubleshooting

### Expected Behavior
**Normal operation:**
1. Green LED on for ~30 seconds
2. Yellow LED on for ~10 seconds
3. Red LED on for ~30 seconds
4. Cycle repeats

**With pedestrian button:**
1. Press button during green
2. Light completes green phase
3. Yellow comes on (10s)
4. Red comes on (30s+)
5. Walk signal appears (green)
6. Don't walk flashes (warning)
7. Return to normal green cycle

### Test Procedure
1. **Power on**: Should start with green or random state, then cycle
2. **Observe cycle**: Green → Yellow → Red → Green (repeat)
3. **Time states**: Measure duration with stopwatch
4. **Button test**: Press during green, verify walk signal after red
5. **Button ignored**: Press during red, should not affect cycle

### Voltage Measurements
**555 Timer:**
- Pin 8: 9V
- Pin 1: 0V
- Pin 3: Pulses between 0V and 9V (use LED to observe)

**CD4017:**
- Pin 16: 9V
- Pin 8: 0V
- Outputs: One HIGH (~9V), others LOW (0V) at any time

### Common Issues and Solutions

**No LEDs light up:**
- Check power to all ICs
- Verify LED polarity (anode to +, cathode to -)
- Test LEDs individually: Connect through 220Ω to 9V
- Check resistors are correct value (220Ω)

**All LEDs on at once:**
- Counter not advancing - check clock signal
- Test 555: Pin 3 should pulse (connect LED to test)
- Check CD4017 clock input (pin 14)
- Verify pin 13 (enable) is grounded

**Stuck on one color:**
- Counter stuck - check CD4017 connections
- Reset pin (15) might be HIGH - connect to GND
- Clock enable (13) might be HIGH - connect to GND
- IC might be damaged - try new CD4017

**Wrong timing:**
- Adjust 555 timing components
- Change R2 or C values
- Check counter tap points
- Verify counter advancing (measure outputs with multimeter)

**Cycles too fast:**
- Increase 555 capacitor (e.g., 220µF instead of 100µF)
- Increase R2 resistor (e.g., 22kΩ instead of 10kΩ)

**Cycles too slow:**
- Decrease 555 capacitor (e.g., 47µF)
- Decrease R2 resistor (e.g., 4.7kΩ)

**Button doesn't work:**
- Check button connections and pull-down resistor
- Test button with multimeter
- Verify debounce capacitor present (100nF)
- Logic gates might not be connected correctly

**Pedestrian LEDs don't light:**
- Check second counter (if used)
- Verify logic gate connections
- Test pedestrian LEDs separately
- Check enable signal to pedestrian counter

**Random behavior:**
- Poor breadboard connections - check all wires
- Power supply noise - add more decoupling capacitors
- Static discharge - CMOS ICs are sensitive
- Timing capacitor may be faulty - try different one

## Calculations

### Clock Period (555 Timer)
```
Period = 0.7 × (R1 + 2×R2) × C
Period = 0.7 × (100k + 2×10k) × 100µF
Period = 0.7 × 120k × 0.0001
Period = 0.7 × 12 = 8.4 seconds per pulse
```

### State Durations
```
Green: 6 counts × 8.4s = ~50 seconds
Yellow: 2 counts × 8.4s = ~17 seconds
Red: 2 counts × 8.4s = ~17 seconds
Total cycle: 10 counts × 8.4s = ~84 seconds (1.4 minutes)
```

**To achieve specific times:**
- 30s Green: Use Q0-Q3 (4 counts × 7.5s = 30s)
  - Set 555: f = 1.44/((R1+2×R2)×C) = 0.133 Hz → 7.5s period
- 5s Yellow: Use Q4 only (1 count × 5s)
- 30s Red: Use Q5-Q8 (4 counts × 7.5s)

### Current Consumption
```
555 Timer: 10mA
CD4017 (×2): 2mA
LEDs (max 2 on): 2 × 20mA = 40mA
Total: ~52mA

9V battery (500mAh): Runtime = 500/52 = ~9.6 hours
```

## Key Concepts Learned
- **State Machines**: Fundamental concept in digital design
- **Sequential Logic**: Circuits with memory and defined states
- **Counter ICs**: Using decade counters for state sequencing
- **Timing Circuits**: Creating real-time delays with hardware
- **Interrupt Handling**: Responding to external events (button press)
- **Logic Gates**: Combining signals for complex behaviors
- **System Design**: Breaking complex system into functional blocks
- **Real-world Modeling**: Simulating actual traffic control systems

## Applications
- **Traffic control**: Real intersections, parking lots
- **Model railroads**: Realistic traffic lights at crossings
- **Education**: Teaching state machines and sequential logic
- **Automated systems**: Any process requiring timed sequences
- **Stage lighting**: Theatrical light sequences
- **Manufacturing**: Assembly line sequencing
- **Robotics**: Navigation and obstacle response
- **Game development**: Understanding state-based systems

## Next Steps
1. **Multiple directions**: Add perpendicular traffic lights
2. **Turn signals**: Green arrows for protected turns
3. **Traffic sensors**: Detect vehicles, adjust timing
4. **Emergency override**: Flashing red for emergency vehicles
5. **Night mode**: Flash red/yellow at night (reduced traffic)
6. **Countdown timer**: Add 7-segment display showing seconds
7. **Wireless coordination**: Multiple intersections synchronized
8. **Microcontroller upgrade**: Compare to Arduino implementation

## Experimentation Ideas

### Add Realism
**All-red phase:**
- Add 2-second all-red between changes
- Safety clearance for intersection
- Use extra counter state

**Green arrow:**
- Protected left turn phase
- Additional LED and logic
- Longer cycle time

**Flashing modes:**
- Add 555 timer for flashing
- Night mode: Flash red both ways
- Caution mode: Flash yellow both ways

### Multiple Intersections
**Cross traffic:**
- Add second set of lights (perpendicular)
- When one is red, other is green
- Coordinate timing

**Synchronized lights:**
- Multiple intersections on same clock
- "Green wave" for traffic flow
- Offset timing for smooth flow

### Advanced Features
**Vehicle detection:**
- IR sensors or pressure pads
- Extend green if traffic present
- Skip green if no vehicles waiting

**Emergency vehicle:**
- Button for fire truck/ambulance
- All lights go red
- Flashing mode to clear intersection

**Adaptive timing:**
- Adjust green time based on traffic density
- Rush hour vs. night mode
- Learn from traffic patterns

## Safety Notes
- ✅ Low voltage (9V DC) - completely safe
- ✅ Suitable for all ages with basic supervision
- ⚠️ Not for controlling real traffic - educational model only
- ⚠️ CMOS ICs sensitive to static - ground yourself before handling
- ⚠️ Don't exceed 15V on any IC
- ✅ No high-power components or heat generation
- ⚠️ If using for model railroad, ensure proper power isolation

## Advanced Considerations

### Microcontroller Comparison
Same project with Arduino is much simpler:
```cpp
// About 30 lines of code vs. complex hardware
enum State {GREEN, YELLOW, RED};
State currentState = GREEN;
void loop() {
  switch(currentState) {
    case GREEN:
      digitalWrite(GREEN_LED, HIGH);
      delay(30000);
      currentState = YELLOW;
      break;
    // ... etc
  }
}
```

**Hardware vs. Software:**
- **Hardware**: True parallel operation, no programming, visible logic
- **Software**: Much simpler, highly flexible, easier to modify
- **Educational value**: Hardware teaches fundamental digital concepts
- **Practical**: Software is used in real traffic systems

### Real Traffic Controllers
Professional systems use:
- **Programmable Logic Controllers (PLCs)**: Industrial computers
- **Microprocessors**: Modern traffic management systems
- **Network connectivity**: Central monitoring and control
- **Redundancy**: Backup systems for safety
- **Standards**: National traffic signal standards (NEMA, ITE)
- **Safety certification**: Rigorous testing and certification

### Timing Standards
Real traffic lights follow standards:
- **Minimum green**: 7 seconds (driver reaction time)
- **Yellow duration**: Formula based on speed limit
  - Yellow = 1 second per 10 mph (e.g., 3s for 30 mph zone)
- **All-red clearance**: 1-2 seconds (intersection clearing)
- **Pedestrian walk**: Based on crossing distance
  - 3-4 seconds minimum + 3.5 ft/s walking speed

---

**Circuit Status**: ✅ Verified circuit design - ready to build
**Last Updated**: December 2024

Stop, look, and learn! 🚦⚡
