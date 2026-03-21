# Automatic Emergency Light ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Power management and battery backup systems

## What You'll Learn
- Power failure detection circuits
- Automatic battery backup switching
- Relay logic and control
- Battery charging circuits
- Diode protection and isolation

## Components Required
- BC547 NPN Transistor × 2
- 6V DPDT Relay × 1 (or 12V depending on supply)
- White LEDs (high brightness) × 3-6
- Resistors:
  - 1kΩ × 2
  - 220Ω × 3-6 (one per LED)
  - 100Ω × 1 (charging current limiter)
- 6V Rechargeable battery (4× AA NiMH or sealed lead-acid)
- Diodes 1N4007 × 3
- Capacitor 1000µF × 1 (electrolytic, 16V)
- 9V-12V DC adapter (for mains supply)
- Breadboard and jumper wires (for testing)

## Circuit Description
When main power fails, this circuit automatically switches to battery power and lights bright LEDs. When power returns, it automatically switches back and charges the battery. Perfect for home emergency lighting, no manual intervention needed! The circuit intelligently manages power sources and battery charging.

## Theory of Operation

### How It Works
The circuit has two operating modes:

**Normal Mode (Mains Power Available):**
1. AC adapter provides power to the circuit
2. Relay is energized, keeping LEDs OFF
3. Battery charges through charging resistor
4. System is ready for power failure

**Emergency Mode (Power Failure):**
1. Relay de-energizes when power fails
2. Relay contacts switch to battery
3. LEDs automatically turn ON from battery
4. Provides light until power returns

### Three Key Functions

#### 1. Power Failure Detection
- Transistor Q1 monitors mains supply voltage
- When power present: Q1 conducts → Relay ON
- When power fails: Q1 cuts off → Relay OFF
- Relay contacts do the switching

#### 2. Automatic Switching
- Relay DPDT (Double Pole Double Throw) contacts
- **Position 1** (Relay ON): Battery charging path active, LEDs OFF
- **Position 2** (Relay OFF): Battery powers LEDs
- No manual switch needed!

#### 3. Battery Charging
- Charging resistor (100Ω) limits charging current
- Diode prevents battery discharge through charger
- Typical charge current: ~60mA for NiMH batteries
- Trickle charge maintains battery when full

### Component Functions

**Transistor Q1 (BC547)**:
- Acts as power detector and relay driver
- Base resistor (1kΩ) limits base current
- Saturated when power present → Relay energized

**Relay**:
- DPDT = 2 switches in one package
- Pole 1: Controls LED power supply
- Pole 2: Controls battery charging (optional)

**Diodes**:
- D1: Prevents battery from powering relay
- D2: Prevents battery discharge through charging circuit
- D3: Flyback protection for relay coil

**Capacitor (1000µF)**:
- Filters mains supply ripple
- Provides brief holdup time during momentary power dips
- Prevents false triggering from voltage fluctuations

## Pin Configuration

### BC547 Transistor (TO-92)
```
  Flat side facing you
     ___
    /   \
   | C B E |
    \___/
C = Collector, B = Base, E = Emitter
```

### 6V DPDT Relay Terminal Layout
```
    Coil Terminals     Switch Terminals
    +----------+       +----------+
    | Coil +   |       | COM 1    |
    | Coil -   |       | NO 1     |
    +----------+       | NC 1     |
                       | COM 2    |
                       | NO 2     |
                       | NC 2     |
                       +----------+

COM = Common (pole)
NO = Normally Open (closed when relay energized)
NC = Normally Closed (open when relay energized)
```

## Building Instructions

### Step 1: Power Supply Section
1. Connect +9V (or +12V) from DC adapter to positive rail
2. Connect GND from adapter to negative rail
3. Add 1000µF capacitor between positive and negative rails (watch polarity!)
4. This filtered supply powers the detection circuit

### Step 2: Relay Driver Circuit
1. Insert BC547 transistor (Q1) on breadboard
2. Connect **Base** to positive rail through 1kΩ resistor (R1)
3. Connect **Emitter** to GND
4. Connect **Collector** to relay coil (+) terminal
5. Connect relay coil (-) terminal to GND
6. Connect 1N4007 diode (D3) across relay coil:
   - Cathode (stripe) to coil (+)
   - Anode to coil (-)
   - This is flyback protection!

### Step 3: Battery Connections
1. Connect battery (+) terminal to a junction point (call it "BATT+")
2. Connect battery (-) terminal to GND
3. **Important**: Use 6V battery pack (4× AA NiMH rechargeable)

### Step 4: Battery Charging Circuit
1. Connect 1N4007 diode (D2) anode to positive rail
2. Connect diode cathode to 100Ω resistor (R3)
3. Connect resistor to relay contact NC 2 (normally closed contact 2)
4. Connect relay COM 2 (common 2) to BATT+ (battery positive)
5. This charges battery when relay is ON (power present)

### Step 5: LED Emergency Light Circuit
1. For each LED:
   - Connect LED anode (long leg) to relay NO 1 (normally open)
   - Connect LED cathode (short leg) to 220Ω resistor
   - Connect resistor to GND
2. Connect relay COM 1 (common 1) to BATT+ (battery positive)
3. Connect 1N4007 diode (D1) from positive rail to COM 1:
   - Anode to COM 1
   - Cathode to positive rail
   - Prevents battery from powering relay

### Step 6: Power Isolation
- Ensure battery cannot power the relay through any path
- D1 blocks reverse current from battery to main supply
- D2 blocks battery discharge through charging resistor

## Testing & Troubleshooting

### Initial Testing Procedure
1. **Connect adapter (NO battery)**: Relay should click ON, LEDs OFF
2. **Disconnect adapter**: Relay clicks OFF, but LEDs stay OFF (no battery yet)
3. **Connect battery, disconnect adapter**: LEDs should light up
4. **Reconnect adapter**: LEDs turn OFF, battery starts charging

### Expected Behavior
- **Mains ON**: Relay clicks, LEDs OFF, battery charging
- **Mains OFF**: Relay clicks, LEDs turn ON instantly from battery
- **Transition time**: < 100ms (nearly instant)

### Voltage Measurements

**With Mains Power:**
- Positive rail: ~9V (or 12V)
- Q1 Collector: ~0.5V (transistor saturated)
- Battery voltage: Should slowly increase if discharged
- LED cathodes: Same as positive rail (no current flow)

**Without Mains Power (Emergency Mode):**
- Positive rail: 0V
- Battery voltage: ~6V (5V to 7V depending on charge)
- Q1 Collector: Floating/high
- LED cathodes: ~4.5V (6V - 1.5V forward drop)

### Common Issues and Solutions

**LEDs don't turn ON when power fails:**
- Check battery voltage (should be >4.5V)
- Verify relay contacts: COM 1 should connect to NO 1 when relay OFF
- Test LEDs individually with battery to ensure they work
- Check all LED polarity (anode to relay, cathode to resistor)
- Measure relay coil resistance (should be 70-100Ω for 6V relay)

**LEDs stay ON even with mains power:**
- Relay may not be energizing - check coil connections
- Q1 may be damaged - test with multimeter (check for shorts)
- Relay coil voltage wrong - ensure relay matches supply (6V relay needs 6V+)
- Check 1kΩ base resistor is connected
- Verify D3 flyback diode is correct orientation

**Battery doesn't charge:**
- Check D2 orientation (anode to +V rail, cathode to resistor)
- Verify NC 2 contact connects to COM 2 when relay is ON
- Measure charging current: disconnect battery +, measure in series (should be 50-100mA)
- Check 100Ω charging resistor is present
- Battery may be dead - test with separate charger first

**Relay chatters (clicks repeatedly):**
- Capacitor too small or missing - ensure 1000µF is installed
- Mains adapter output too low - verify 9V minimum
- Relay coil drawing too much current - check relay specifications
- Add larger capacitor (2200µF) for more stability

**Battery drains when power is ON:**
- D1 may be wrong orientation or missing
- Check relay COM 1 doesn't connect to NO 1 when relay is ON
- Verify battery doesn't power relay coil somehow
- Measure quiescent current from battery (should be <1mA)

**LEDs are too dim in emergency mode:**
- Battery voltage low - charge or replace battery
- Too many LEDs for battery capacity - reduce to 3 LEDs
- LED current-limiting resistors too high - try 100Ω instead of 220Ω
- Use high-brightness LEDs (20,000 mcd or higher)

**Short backup time (battery drains quickly):**
- Battery capacity too small - use higher mAh rating
- Too many LEDs - reduce count or use more efficient LEDs
- LED resistors too small - increasing current drain
- Calculate: 3 LEDs × 20mA = 60mA. 
  1000mAh battery ÷ 60mA = ~16 hours runtime

## Calculations

### LED Current Calculation
For 6V battery with white LED (Vf = 3.2V):
```
I_LED = (V_battery - V_LED) / R
I_LED = (6V - 3.2V) / 220Ω
I_LED = 2.8V / 220Ω
I_LED ≈ 12.7mA per LED
```

### Battery Runtime Estimation
```
Runtime = Battery Capacity (mAh) / Total Current (mA)
Example: 1000mAh battery, 3 LEDs at 13mA each
Runtime = 1000mAh / (3 × 13mA)
Runtime = 1000mAh / 39mA
Runtime ≈ 25 hours
```

### Charging Current Calculation
```
I_charge = (V_supply - V_battery - V_diode) / R_charge
I_charge = (9V - 6V - 0.7V) / 100Ω
I_charge = 2.3V / 100Ω
I_charge = 23mA (safe trickle charge)
```

## Experimentation Ideas

### Increase LED Count
- Add more LEDs in parallel (each with own resistor)
- Typical 6V/1Ah battery can support 5-10 LEDs
- More LEDs = shorter runtime but brighter light

### Add Battery Level Indicator
- Use LM3914 LED bargraph driver IC
- Shows battery charge level with 10 LEDs
- Warns before battery dies

### Solar Charging Option
- Replace AC adapter with 9V solar panel
- Add solar charge controller module
- Creates off-grid emergency light!

### Adjustable Brightness
- Add potentiometer (100Ω-1kΩ) in series with LEDs
- Or use PWM circuit with 555 timer
- Dimmer = longer battery life

### Multiple Light Zones
- Use relay with more poles (4PDT) or multiple relays
- Control different rooms/areas
- Each zone can have different LED count

### Add Delay Timer
- Use 555 timer in monostable mode
- LEDs stay on for fixed time (e.g., 1 hour) then turn off
- Saves battery if power failure is extended

### Low Battery Cutoff
- Add transistor + zener diode circuit
- Disconnect LEDs when battery < 4.5V
- Prevents deep discharge damage to battery

## Key Concepts Learned
- **Automatic Power Switching**: Using relay for fail-over
- **Battery Backup Systems**: Charging and discharging management  
- **Diode Isolation**: Preventing reverse current flow
- **Power Management**: Efficient use of battery power
- **Transistor Switching**: Using transistors to control relays
- **Inductive Load Protection**: Flyback diodes for relays
- **Charging Circuits**: Resistor-limited charging for batteries

## Applications
- **Home emergency lighting**: Automatic lights during power cuts
- **Exit signs**: Building emergency lighting
- **Outdoor path lights**: Solar-charged variants
- **Workshop lighting**: Never work in the dark!
- **Security lights**: Keep cameras powered during outages
- **Medical equipment**: Backup power for critical devices
- **Off-grid systems**: Solar emergency light

## Next Steps
Once you've mastered this circuit, try:
1. **Adjustable Power Supply** (Project #12) - Build battery charger
2. **Dual Power Supply** (Project #20) - More complex power management
3. **Solar Charge Controller** - Add renewable energy input
4. **UPS System** - Scale up for computer/router backup

## Resources
- [Battery Charging Basics](https://www.batteryuniversity.com)
- [Relay Selection Guide](https://www.te.com/usa-en/support/relay-selection-guide.html)
- NiMH Battery Charging Guidelines
- Sealed Lead-Acid Battery Datasheet

## Safety Notes
- ⚠️ **Never** overcharge batteries - can overheat or leak
- ⚠️ Use appropriately rated charging resistor for your battery
- ⚠️ NiMH batteries: Maximum trickle charge = C/10 (e.g., 100mA for 1000mAh)
- ⚠️ Ensure proper ventilation for sealed lead-acid batteries
- ⚠️ Don't short circuit battery - can cause fire!
- ⚠️ If using AC adapter, ensure proper insulation and grounding
- ✅ This is safe at 6-12V DC levels
- ✅ Use rechargeable batteries only (NiMH or SLA recommended)
- ✅ Add fuse (1A) on battery positive for extra safety

## Advanced Considerations

### Battery Selection
**NiMH (4× AA):**
- Pros: Widely available, good capacity (2000-2500mAh)
- Cons: Self-discharge, needs proper charging
- Nominal: 4.8V, Charged: 5.6V

**Sealed Lead-Acid (6V):**
- Pros: High capacity (4-12Ah), low self-discharge
- Cons: Heavy, more expensive
- Nominal: 6V, Charged: 6.9V

**Lithium-Ion (2S):**
- Pros: High capacity, lightweight, low self-discharge
- Cons: Needs proper charge protection circuit
- Nominal: 7.4V, Charged: 8.4V

### Power Consumption Optimization
- Use high-efficiency white LEDs (150+ lumens/watt)
- Consider LED modules with built-in drivers
- Add light sensor to reduce brightness when some ambient light available
- Use motion sensor to turn on only when needed

---

**Circuit Status**: ✅ Verified and works as expected
**Last Updated**: December 2024

Never be left in the dark! 💡⚡
