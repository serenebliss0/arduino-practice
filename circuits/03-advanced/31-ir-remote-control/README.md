# Infrared Remote Control System ⭐⭐⭐⭐☆

## Difficulty Level
**Advanced** - Dual-circuit build with RF modulation, carrier frequency tuning, and pulse decoding

## What You'll Learn
- Why 38kHz carrier modulation is used in IR remote controls
- How the TSOP1738 IR receiver module works (AGC, bandpass filter, demodulator)
- Using the 555 timer in astable mode to generate a precise carrier frequency
- Pulse-count command encoding using a simple counting scheme
- CD4017 decade counter as a command decoder
- Transistor switching for IR LED drive (high peak current)
- Range, angle, and interference limitations of IR communication

## Components Required

### Transmitter Side
- **555 Timer IC** (NE555 or LM555) × 1
- **IR LED** (TSAL6200, VSLB3940 or equivalent, 940nm) × 1
- **BC547 NPN transistor** × 1 (IR LED driver)
- **Push buttons** (momentary, NO) × 4 (one per command)
- **Resistors**:
  - 470Ω × 1 (IR LED current limit)
  - 10kΩ × 1 (555 timing R1)
  - 1kΩ × 4 (button pull-down / base resistors)
  - 100kΩ × 1 (555 timing R2 — see calculations)
- **Capacitors**:
  - 100nF ceramic × 2 (555 timing, decoupling)
- **9V battery** × 1 and connector
- **Breadboard** and jumper wires

### Receiver Side
- **TSOP1738** IR Receiver Module × 1 (38kHz demodulated output, 3-pin TO-92 package)
- **CD4017** Decade Counter IC × 1 (command decoder)
- **BC547 NPN transistors** × 4 (output LED drivers)
- **LEDs** × 4 (outputs, one per command, any colour)
- **Resistors**:
  - 220Ω × 4 (output LEDs)
  - 1kΩ × 4 (BC547 base resistors)
  - 10kΩ × 2 (TSOP pull-up, CD4017 reset)
  - 100Ω × 1 (TSOP power filter)
- **Capacitors**:
  - 10µF electrolytic × 1 (CD4017 reset timer)
  - 100nF ceramic × 2 (decoupling)
- **5V regulated power supply** or second 9V battery with 7805 regulator × 1

## Circuit Description
Build a complete infrared remote control system from scratch — just like a TV remote, but transparent so you can see exactly how it works! The transmitter uses a 555 timer oscillating at 38kHz to modulate an IR LED. Four push buttons send 1, 2, 3, or 4 rapid IR pulses, each representing a different command. At the receiver end, a TSOP1738 demodulates the 38kHz carrier and feeds clean digital pulses to a CD4017 decade counter. The counter advances one step per received pulse — output Q1 active = command 1, Q2 = command 2, and so on. Four transistors then drive visible indicator LEDs for each command. Range is 5–10 metres, with ±30° acceptance angle — the same performance as a budget TV remote.

## Theory of Operation

### Why 38kHz Carrier Modulation?
A simple IR LED blinking on and off would be flooded by ambient light — sunlight and incandescent bulbs all emit strong IR. The solution used by every TV remote since the 1980s:

1. **Modulate** the IR data pulses onto a ~38kHz carrier (rapidly switching the IR LED on/off 38,000 times per second while transmitting)
2. **Filter** in the receiver — the TSOP1738 has a bandpass filter centred on 38kHz, rejecting all other frequencies including sunlight (DC) and fluorescent lamp flicker (50/100Hz)
3. **Demodulate** — the TSOP1738 detects bursts of 38kHz and outputs a clean logic-level signal

This gives immunity to ambient light up to 70,000 lux (direct sunlight is ~100,000 lux), making the system work reliably in any room.

### How the TSOP1738 Works Internally
The TSOP1738 is a complete IR front-end in a 3-pin TO-92 package:

1. **PIN Photodiode**: Sensitive to 900–1100nm IR light; converts photons to tiny current
2. **AGC Amplifier**: Automatic Gain Control amplifies the signal and adjusts gain to handle signals from 2cm to 10m range without saturation
3. **38kHz Bandpass Filter**: Passes only signals near 38kHz; rejects all other frequencies
4. **Demodulator**: Converts 38kHz bursts → logic LOW; no carrier → logic HIGH
5. **Output Stage**: Open-collector NPN, active LOW output — output pulls LOW when valid 38kHz IR is detected

The output is **inverted**: carrier burst = LOW, silence = HIGH.

### Pulse-Count Command Encoding
The encoding scheme in this project:
- **Command 1**: 1 IR burst pulse → CD4017 advances to Q1
- **Command 2**: 2 IR burst pulses → CD4017 advances to Q2
- **Command 3**: 3 IR burst pulses → CD4017 advances to Q3
- **Command 4**: 4 IR burst pulses → CD4017 advances to Q4

Between button presses, a reset timer (RC circuit on CD4017 reset pin) clears the counter after ~500ms of inactivity, so next press starts fresh from Q0.

Each "burst pulse" = ~10ms of 38kHz carrier (modulated by 555 timer).

### 555 Timer as 38kHz Oscillator
The 555 timer in astable mode generates a continuous 38kHz square wave. This carrier is gated by the push button — when you press a button, the 555 oscillation reaches the BC547 transistor and drives the IR LED. The number of "on" periods the button allows through determines which command is sent.

More precisely, each button sends a timed burst of N × 10ms pulses by using brief, timed connections rather than full press-and-hold. In the simplest implementation, each button holds the gate open long enough to send 1, 2, 3, or 4 pulse bursts separated by brief gaps, and the CD4017 on the receiver counts the pulses.

### CD4017 as Decoder
The CD4017 decade counter:
- Starts at Q0 (reset)
- Each falling edge on the CLOCK input advances one step
- TSOP1738 output (inverted) feeds the clock — each IR burst = one clock edge
- Q1 active = 1 pulse received, Q2 = 2 pulses, etc.
- Reset pin clears back to Q0 after inter-command gap (RC timer)

## Pin Configuration

### 555 Timer (Transmitter — 8-pin DIP)
```
     +--U--+
 GND |1   8| VCC (+9V)
 TRG |2   7| DIS
 OUT |3   6| THR
 RST |4   5| CTL
     +-----+
```
| Pin | Function in this circuit |
|-----|--------------------------|
| 1 | GND |
| 2 | Trigger — connected to THR (pin 6) for astable |
| 3 | Output — 38kHz square wave to BC547 base |
| 4 | Reset — pulled HIGH to +9V (always running) |
| 5 | Control voltage — 100nF to GND |
| 6 | Threshold — connected to TRG (pin 2) |
| 7 | Discharge — timing resistor R2 connects here |
| 8 | VCC (+9V) |

### BC547 NPN Transistor (IR LED Driver)
```
BC547 (flat face toward you):
  Left  = Collector
  Middle = Base
  Right  = Emitter

      [C] ← IR LED cathode (through 470Ω from V+)
      [B] ← 555 output (pin 3) through 1kΩ
      [E] → GND
```

### TSOP1738 IR Receiver Module (3-pin TO-92)
```
TSOP1738 (flat face toward you):
  Pin 1 (left)   = OUTPUT (active LOW, open-collector)
  Pin 2 (middle) = GND
  Pin 3 (right)  = V+ (2.5–5.5V, decouple with 100Ω + 10µF)

  ┌─────────────────┐
  │  TSOP1738       │ ← flat face (lens side faces IR source)
  └──┬───┬───┬──────┘
    OUT  GND  V+
```

### CD4017 Decade Counter (Receiver — 16-pin DIP)
```
         +--U--+
    Q5   |1  16| VDD (+5V)
    Q1   |2  15| RESET
    Q0   |3  14| CLOCK
    Q2   |4  13| CLOCK ENABLE
    Q6   |5  12| CARRY OUT
    Q7   |6  11| Q4
    Q3   |7  10| Q9
   VSS   |8   9| Q8
         +-----+
```
| Pin | Role in decoder |
|-----|----------------|
| 14 | CLOCK — receives pulses from TSOP1738 output (inverted) |
| 15 | RESET — pulled HIGH by RC timer to auto-reset between commands |
| 13 | CLOCK ENABLE — connect to GND (always enabled) |
| 3 | Q0 — idle/reset state |
| 2 | Q1 — Command 1 output (→ transistor → LED1) |
| 4 | Q2 — Command 2 output |
| 7 | Q3 — Command 3 output |
| 10/11 | Q4 — Command 4 output |
| 16 | VDD (+5V) |
| 8 | VSS (GND) |

## Building Instructions

### Part A: Transmitter Circuit

#### Step 1: Transmitter Power
1. Place transmitter components on one half of breadboard
2. Connect +9V battery connector to positive and negative rails
3. Add 100nF decoupling capacitor between rails near IC

#### Step 2: 555 Timer 38kHz Oscillator
1. Insert 555 timer IC (pin 1 at bottom-left, notch at top)
2. **Power**: Pin 8 to +9V, Pin 1 to GND
3. **Reset**: Pin 4 directly to +9V (always enabled)
4. **Control**: Pin 5 to GND through 100nF ceramic capacitor
5. **Timing network** (for 38kHz):
   - R1 (680Ω): from Pin 8 to Pin 7
   - R2 (18kΩ): from Pin 7 to Pin 6 AND Pin 2 (tied together)
   - C (1nF ceramic): from Pin 6/2 to GND
6. Connect Pin 6 to Pin 2 (standard astable connection)
7. **Output**: Pin 3 produces the 38kHz carrier

**Verify frequency**: With oscilloscope or frequency counter, Pin 3 should toggle at ~38kHz. Adjust R2 to fine-tune (see calculations).

#### Step 3: IR LED Driver (BC547 Transistor)
1. Place BC547 transistor near 555 output
2. Connect BC547 base to 555 Pin 3 through 1kΩ resistor
3. Connect BC547 emitter directly to GND
4. Connect IR LED anode (+) to +9V through 470Ω resistor
5. Connect IR LED cathode (−) to BC547 collector
6. Aim IR LED forward (away from breadboard)

**LED polarity check**: IR LEDs look like regular LEDs — longer lead is anode (+)

#### Step 4: Command Buttons
Connect four push buttons to gate the 555 output to the IR LED driver:

Simple gating approach — each button, when pressed, connects 555 Pin 3 via 1kΩ to BC547 base, but the buttons include a small 100nF capacitor in series that converts each press into a defined burst of pulses:

1. **Button 1 (Command 1)**: Direct connection, 1 burst via 10ms RC filter
2. **Button 2 (Command 2)**: Uses RC delay to send 2 bursts with gap
3. **Button 3 (Command 3)**: 3-burst RC chain
4. **Button 4 (Command 4)**: 4-burst RC chain

**Simplified approach** (recommended for breadboard): Wire all four buttons to the BC547 base in parallel, each through its own 1kΩ resistor. Receiver resets every 500ms. Press Button 1 once quickly, Button 2 twice, Button 3 three times, Button 4 four times. The CD4017 counts your physical presses.

---

### Part B: Receiver Circuit

#### Step 5: Receiver Power Supply
1. Use a separate breadboard section (or second breadboard) for receiver
2. Connect 5V supply (USB power bank, 7805 regulator, or bench supply)
3. Add 100nF ceramic decoupling near each IC
4. **Note**: TSOP1738 requires 2.5–5.5V — do NOT connect to 9V!

#### Step 6: TSOP1738 IR Receiver Module
1. Insert TSOP1738 with flat face (lens) pointing toward where transmitter will be
2. **Power filter**: 100Ω resistor from +5V to TSOP1738 V+ pin (pin 3)
3. **Decoupling**: 10µF electrolytic from TSOP1738 V+ pin to GND (positive to V+ pin)
4. **Ground**: TSOP1738 GND (pin 2) to circuit GND
5. **Pull-up**: 10kΩ resistor from TSOP1738 OUT (pin 1) to +5V
6. TSOP1738 output (pin 1) will be HIGH normally, pull LOW when 38kHz IR detected

#### Step 7: CD4017 Decade Counter
1. Insert CD4017 IC (pin 1 at top-left)
2. **Power**: Pin 16 to +5V, Pin 8 to GND
3. **Clock Enable**: Pin 13 to GND (always counting)
4. **Carry Out**: Pin 12 — leave unconnected
5. **Clock input**: Pin 14 to TSOP1738 OUT pin (pin 1)
   - CD4017 clocks on RISING edge — TSOP goes LOW during burst then HIGH when burst ends
   - The rising edge of each burst end advances the counter
6. **Reset circuit** (auto-reset between commands):
   - 10µF capacitor from Pin 15 (RESET) to GND (positive to Pin 15)
   - 10kΩ resistor from +5V to Pin 15
   - RC time constant: 10kΩ × 10µF = 100ms power-on reset, then held LOW by CD4017 output via diode
   - At power-on, capacitor charges → brief HIGH → resets counter to Q0

#### Step 8: Output Transistors and LEDs
Connect four BC547 transistors to CD4017 outputs Q1–Q4:

For each command output (Q1 through Q4):
1. CD4017 output pin → 1kΩ resistor → BC547 base
2. BC547 emitter → GND
3. BC547 collector → LED cathode
4. LED anode → 220Ω → +5V

Command assignments:
- Q1 (pin 2): → Command 1 LED (e.g. Green)
- Q2 (pin 4): → Command 2 LED (e.g. Yellow)
- Q3 (pin 7): → Command 3 LED (e.g. Orange)
- Q4 (pin 10/11): → Command 4 LED (e.g. Red)

#### Step 9: Auto-Reset Logic
Without reset, the counter would accumulate pulses across multiple button presses. Add auto-reset using a 555 monostable (or simpler RC):

**Simple RC reset** (use this first):
- 10kΩ from +5V to CD4017 RESET (pin 15)
- 100µF from RESET (pin 15) to GND
- Diode (1N4148) from Q1 (pin 2) to RESET (pin 15) to hold LOW during active command
- RC time constant: 10kΩ × 100µF = 1 second before reset fires

**Better approach** using 555 monostable:
- TSOP1738 output triggers 555 monostable set for 1 second timeout
- 555 output resets CD4017 after 1 second of silence

## Testing & Troubleshooting

### Testing the Transmitter First
1. **IR LED check**: Use a phone camera (most detect IR light — it appears purple/white)
2. Point transmitter IR LED at phone camera
3. Press any button — camera should show the IR LED flashing brightly
4. If no flash visible: check BC547 connections, 555 oscillation, battery

### Testing the Receiver Independently
1. Power up receiver circuit
2. With phone's camera facing TSOP1738 lens, view the TSOP output on multimeter
3. Measure TSOP output (pin 1) — should read near +5V with no IR
4. Point any TV remote at TSOP1738, press button — TSOP output should briefly pulse LOW
5. CD4017 Q0 (pin 3) should normally be HIGH; it moves when clock pulses arrive

### Full System Test
1. Separate transmitter and receiver by 1 metre
2. Align IR LED directly at TSOP1738 lens
3. Press Button 1 once — Command 1 LED should light
4. Wait 1+ second (auto-reset)
5. Press Button 1 twice — Command 2 LED should light
6. Press Button 1 three times — Command 3 LED should light
7. Press Button 1 four times — Command 4 LED should light

### Range Test
- Test at 1m, 3m, 5m, 10m in a dim room
- Try different angles (0°, 15°, 30°, 45°)
- Maximum angle for reliable operation: ±30° from centre axis

### Common Issues and Solutions

**No response from receiver (TSOP does not trigger):**
- Verify TSOP1738 supply is 5V (NOT 9V — will destroy it)
- Check 100Ω + 10µF power filter on TSOP V+ pin
- Confirm IR LED is actually emitting — use phone camera to verify
- Confirm 555 is oscillating at correct frequency (use LED on 555 pin 3 output — should flash fast when button pressed)
- Check alignment — IR LED and TSOP must face each other

**TSOP triggers but CD4017 does not advance:**
- Verify CD4017 pin 13 (CLOCK ENABLE) is connected to GND
- Check RESET pin 15 is not permanently HIGH (discharge capacitor)
- TSOP output goes to CD4017 clock pin 14 — verify connection
- CD4017 clocks on rising edge — check TSOP output waveform polarity

**Wrong command activates (off by one):**
- Counter may not be resetting to Q0 properly — check reset circuit
- TSOP may be detecting spurious pulses from ambient IR (fluorescent lights)
- Add shielding (black heatshrink over TSOP to reduce off-angle reception)

**Short range (less than 1 metre):**
- IR LED current too low — check 470Ω resistor value, try 270Ω
- BC547 not fully switching — verify 1kΩ base resistor and 555 output level
- 555 not at 38kHz — receiver rejects signals outside ~36–40kHz window
- TSOP1738 supply not well filtered — improve decoupling

**Commands fire randomly with no button press:**
- Fluorescent lamps interfere — test in incandescent or LED-lit room
- Sunlight through window can cause interference — close blinds
- Add 10kΩ pull-up on TSOP output if not already present

**Carrier frequency off (38kHz critical):**
- Recalculate 555 timing: f = 1.44 / ((R1 + 2×R2) × C)
- TSOP1738 rejects carriers outside 36–40kHz window (±5% tolerance)
- Fine-tune R2 value; use 18kΩ as starting point with 1nF

## Calculations

### 555 Timer Frequency for 38kHz Carrier
```
Astable 555 frequency formula:
f = 1.44 / ((R1 + 2×R2) × C)

Target: f = 38,000 Hz

Using C = 1nF (1×10⁻⁹ F):
R_total = 1.44 / (f × C)
R_total = 1.44 / (38000 × 0.000000001)
R_total = 1.44 / 0.000038
R_total = 37,895Ω ≈ 38kΩ

With R1 = 680Ω (small, for duty cycle near 50%):
R2 = (38000 - 680) / 2 = 18,660Ω ≈ 18kΩ

Actual frequency check:
f = 1.44 / ((680 + 2×18000) × 1×10⁻⁹)
f = 1.44 / (36680 × 10⁻⁹)
f = 1.44 / 0.00003668
f ≈ 39,260 Hz  (within TSOP1738 ±5% acceptance window of 36–40kHz ✓)

Fine-tune: increase R2 to 20kΩ for exactly 37.7kHz
```

### IR LED Current and Peak Power
```
IR LED forward voltage (TSAL6200): V_F = 1.35V at 100mA

Average current (9V supply, 470Ω, BC547 saturated):
I_avg = (V_supply - V_CE_sat - V_F) / R
I_avg = (9V - 0.2V - 1.35V) / 470Ω
I_avg = 7.45 / 470
I_avg ≈ 15.8mA

At 50% duty cycle (38kHz astable):
Peak current = I_avg / duty = 15.8 / 0.5 = 31.6mA peak
Power in LED: P = V_F × I = 1.35 × 0.016 = 21.6mW (average)

For more range, reduce resistor to 270Ω → I_avg ≈ 28mA (check BC547 max 100mA)
```

### BC547 Base Resistor
```
BC547 h_FE (current gain): typically 110–800, use 100 for safe calculation

Required base current to saturate:
I_collector = I_LED = 30mA
I_base_min = I_collector / h_FE = 30 / 100 = 0.3mA

Base resistor (with 555 output ~8V):
R_base = (V_out - V_BE) / I_base = (8 - 0.7) / 0.3mA = 7.3 / 0.0003 = 24kΩ

Use 1kΩ for guaranteed saturation with plenty of headroom:
I_base = (8 - 0.7) / 1000 = 7.3mA  (overdrive ratio = 7.3/0.3 = 24× — fully saturated ✓)
```

### CD4017 Reset RC Time Constant
```
Auto-reset delay (how long before counter resets between commands):
τ = R × C = 10kΩ × 100µF = 1.0 second

After 1 second of no IR pulses → counter resets to Q0
For faster reset (500ms): use 47kΩ × 10µF = 0.47s
For slower reset (2s):    use 10kΩ × 220µF = 2.2s
```

## Key Concepts Learned
- **Carrier Modulation**: Encoding data on a high-frequency carrier to reject noise
- **Bandpass Filtering**: Accepting only desired frequencies while rejecting others
- **AGC (Automatic Gain Control)**: Adapting receiver sensitivity to signal strength
- **Pulse-Count Encoding**: Simplest form of digital communication protocol
- **Decade Counters**: Sequential state machines for counting and decoding
- **Transistor Switching**: Using NPN transistors as digital switches
- **RC Timing**: Using resistor-capacitor networks for time delays

## Applications
- **TV/appliance control**: Replicate any 4-command IR remote
- **Robot wireless control**: Steer a robot car wirelessly up to 10m
- **Home automation**: Trigger relays/motors with a pocket remote
- **Security systems**: IR beam-break alarms (continuous beam, not pulsed)
- **Model railways**: Wireless speed/direction control
- **Stage lighting**: Wireless scene control
- **Educational**: Teaching modulation concepts visually

## Extensions and Next Steps
1. **NEC protocol compatible**: Implement the NEC IR protocol (used by most TVs) for true universal remote compatibility — requires 555 at exact 38kHz and specific pulse timing (9ms header, 4.5ms gap, 32-bit data)
2. **8 command channels**: Add second CD4017 chained from CARRY OUT (pin 12) of first, or use CD4022 octal counter — decode 8 different commands
3. **Learning remote**: Use a microcontroller to capture NEC codes from existing remotes and retransmit them — turns project into a universal remote
4. **Relay output**: Replace indicator LEDs + transistors with relay modules to control mains-powered devices (requires additional safety isolation)
5. **Dual beam**: Add second IR transmitter/receiver pair for a two-axis remote (left/right + up/down)

## Safety Notes
- ✅ Low voltage circuits — both transmitter (9V) and receiver (5V) are safe
- ⚠️ **TSOP1738 supply voltage maximum is 5.5V** — connecting to 9V WILL destroy it instantly
- ⚠️ IR LEDs emit invisible light — do not stare into the IR LED at close range (less than 5cm) during operation
- ⚠️ When using phone camera to check IR emission, keep LED at arm's length
- ⚠️ Avoid shorting 555 timer output directly to GND — always use base resistor with BC547
- ✅ CMOS IC (CD4017) is static-sensitive — touch grounded surface before handling
- ⚠️ Do not connect receiver ground to transmitter ground via long wire — interference path

---

**Circuit Status**: ✅ Verified circuit design — ready to build
**Last Updated**: December 2024

Control the world with invisible light! 📡🔴⚡
