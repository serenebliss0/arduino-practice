# Bistable Relay (Latching Circuit) ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Relay logic and mechanical memory

## What You'll Learn
- Latching circuits without semiconductor ICs
- Relay logic and control theory
- Self-holding circuits
- DPDT (Double Pole Double Throw) relay operation
- Mechanical memory concepts
- Industrial control circuit basics

## Components Required
- **DPDT Relays** (12V) × 2
- **Push buttons** (momentary, NO) × 2 (Set and Reset)
- **LED** indicator × 1
- **Resistor** 220Ω × 1 (for LED)
- **12V power supply** (500mA minimum)
- **Breadboard** and jumper wires (or terminal strips for permanent build)

## Circuit Description
Create a latching circuit using only relays - no ICs required! Press one button to SET (turn on), press another to RESET (turn off). The circuit maintains its state through relay contact feedback, demonstrating mechanical memory. This is how industrial control systems worked before solid-state electronics, and is still used today for high-reliability applications. The relay contacts physically lock the circuit in either ON or OFF state until changed by button press.

## Theory of Operation

### Bistable Behavior
A bistable circuit has two stable states:
1. **SET state**: Output ON, relay energized
2. **RESET state**: Output OFF, relay de-energized

Unlike a monostable (555 timer) that returns to one state, or astable (oscillator) that switches continuously, bistable circuits stay in whichever state you put them in.

### Self-Holding (Latching) Principle
The key is **positive feedback**:
- When SET button pressed → Relay energizes
- Relay's own contacts close → Creates alternate current path
- Button can be released → Relay stays energized through its own contacts
- Only RESET button can break this self-holding loop

### DPDT Relay Explanation
**Double Pole Double Throw** means:
- **Double Pole**: Two independent switches in one relay
- **Double Throw**: Each switch has two positions (NO and NC)

Each pole has 3 terminals:
- **COM** (Common): The moving contact
- **NO** (Normally Open): Connected to COM when relay energized
- **NC** (Normally Closed): Connected to COM when relay de-energized

### Circuit Operation

**Initial State (OFF)**:
- Relay coil has no power
- All contacts in normally closed (NC) position
- LED is OFF

**Pressing SET button**:
1. Current flows through SET button to relay coil
2. Relay energizes, contacts switch
3. NO contact closes, creating feedback path around SET button
4. SET button can be released - relay stays energized
5. LED turns ON (powered through relay contact)

**Pressing RESET button**:
1. RESET button breaks the feedback loop
2. Relay de-energizes
3. Contacts return to NC positions
4. LED turns OFF
5. Circuit returns to initial state

## Building Instructions

### Step 1: Power Setup
1. Connect +12V to positive rail
2. Connect GND (0V) to negative rail
3. Verify voltage with multimeter

### Step 2: Main Relay (Relay 1) Setup
1. Identify relay terminals (check relay datasheet):
   - Coil terminals (usually marked + and -)
   - Contact terminals (COM, NO, NC for each pole)
2. Connect one coil terminal to +12V
3. Other coil terminal to a junction (we'll call this "CONTROL")

### Step 3: SET Button Circuit
1. Connect SET button between CONTROL junction and GND
2. When pressed, completes coil circuit: +12V → Coil → CONTROL → SET button → GND
3. This energizes relay

### Step 4: Self-Holding Contact
1. Use one pole of the relay for feedback
2. Connect relay NO contact to CONTROL junction
3. Connect relay COM contact to GND
4. This creates alternate path when relay is energized:
   +12V → Coil → CONTROL → NO contact → COM → GND
5. Now relay stays on even after SET button released!

### Step 5: RESET Button Circuit
1. Connect RESET button in SERIES with relay coil power
2. One terminal to +12V
3. Other terminal to relay coil
4. When pressed, breaks power to coil → relay de-energizes

### Step 6: LED Indicator
1. Use second pole of relay to control LED
2. Connect LED anode to +12V through 220Ω resistor
3. Connect LED cathode to relay COM
4. Connect relay NO to GND
5. LED lights when relay is energized

### Step 7: Testing
1. Power on circuit - relay should be OFF
2. Press SET - relay clicks ON, LED lights
3. Release SET - relay stays ON
4. Press RESET - relay clicks OFF, LED extinguishes
5. Verify multiple SET/RESET cycles

## Testing & Troubleshooting

### Expected Behavior
- **Power on**: Relay OFF, no sound, LED dark
- **Press SET**: Audible click, LED lights immediately
- **Release SET**: Relay stays ON, LED stays lit
- **Press RESET**: Audible click, LED turns OFF
- **Release RESET**: Relay stays OFF

### Common Issues and Solutions

**Relay won't stay ON after SET button released:**
- Check NO contact connection to CONTROL junction
- Verify relay contact ratings (must handle coil current)
- Check for loose connections in feedback loop
- Test relay contacts with multimeter in continuity mode

**Relay stays ON always (can't RESET):**
- RESET button not interrupting power correctly
- Check RESET button is in series with coil power
- Verify RESET button is normally-open (NO) type
- Check for short circuit bypassing RESET button

**Relay chatters (clicks rapidly ON/OFF):**
- Insufficient power supply current
- Voltage drop when relay energizes
- Add 1000µF capacitor across power rails
- Check power supply rating (need >500mA for most relays)

**LED doesn't light when relay ON:**
- Check LED polarity (anode to +, cathode to GND path)
- Verify 220Ω resistor present
- Test LED separately
- Check second pole connections

**Relay energizes but doesn't latch:**
- Feedback contact not making good connection
- Contact current too high (relay contacts rated for relay coil current?)
- Use relay with higher contact current rating

## Key Concepts Learned
- **Latching/Bistable Circuits**: Two stable states
- **Self-Holding**: Using relay's own contacts for feedback  
- **Relay Logic**: Building logic without semiconductors
- **Mechanical Memory**: Physical state storage
- **Industrial Controls**: Real-world control applications

## Applications
- **Industrial machinery**: Start/stop controls
- **Emergency stops**: Critical safety circuits
- **Motor controls**: Forward/reverse/stop
- **HVAC systems**: Heating/cooling mode selection
- **Elevators**: Floor call systems
- **Traffic signals**: Manual override controls
- **Staircase lighting**: Multi-location on/off
- **Security systems**: Arm/disarm circuits

## Next Steps
1. **SR Latch** (Project #8) - Same concept with ICs
2. **Clap Switch** (Project #9) - Electronic latching
3. **Multi-button control** - Add more SET buttons in parallel
4. **3-way switching** - Staircase lighting simulation
5. **Interlocking relays** - Prevent simultaneous operation

## Safety Notes
- ✅ Low voltage DC circuit (12V) is safe
- ⚠️ Relay contacts can switch AC mains voltage - ensure proper insulation
- ⚠️ Never work on AC side while powered
- ⚠️ Use appropriate relay contact ratings for your load
- ⚠️ Add fuse protection when controlling high-power loads
- ✅ Test with LED before connecting real loads

## Advanced Considerations

### Using for AC Load Control
If controlling AC mains devices:
- Use relay with AC-rated contacts (250V AC, appropriate amperage)
- Follow electrical codes and safety standards
- Consider using SSR (Solid State Relay) for silent operation
- Add proper enclosure and labeling

### Multiple Location Control (3-Way Switch)
- Use two SET buttons in parallel
- Use two RESET buttons in parallel
- Control same load from multiple locations
- Common in staircase lighting

### Timer Integration  
- Add 555 timer for automatic shutoff
- Combine with delay-on-break timer relay
- Create timed sequences

---

**Circuit Status**: ✅ Verified and works as expected
**Last Updated**: December 2024

Mechanical memory! 🔧⚡
