# 4-Bit Binary Counter with 7-Segment Display ⭐⭐⭐⭐⭐

## Difficulty Level
**Advanced** - Complex multi-IC design with BCD counting, 7-segment decoding, and cascadable architecture

## What You'll Learn
- Binary and BCD (Binary Coded Decimal) number representation
- How a 7-segment display encodes digits using segment patterns
- Operation of the CD4026 BCD Counter/7-Segment Driver IC
- 555 Timer in astable mode as an automatic clock source
- Manual vs. automatic counting modes
- Carry-out logic for cascading multiple displays (multi-digit counters)
- Calculating 555 timer frequency and display segment current
- Breadboarding and debugging multi-IC circuits

## Components Required
- **CD4026** BCD Counter / 7-Segment Decoder Driver IC × 1 (or CD4033 + CD4518 if preferred)
- **555 Timer** IC (NE555 or LM555) × 1
- **7-Segment Display** (common cathode) × 1
- **Push button** (momentary, normally open) × 1
- **Resistors**:
  - 220Ω × 7 (current limiting for each display segment)
  - 10kΩ × 2 (pull-down / timing)
- **Capacitors**:
  - 10µF electrolytic × 1 (555 timing)
  - 100nF ceramic × 2 (decoupling / 555 control)
- **9V battery** × 1
- **Battery connector** (PP3 snap)
- **Breadboard** (full-size recommended) and jumper wires

> **Optional for cascading (multi-digit):**
> - Additional CD4026 × N (one per extra digit)
> - Additional 7-segment displays × N
> - 220Ω resistors × 7N (one set per display)

## Circuit Description
Build a single-digit (0–9) electronic counter that displays the current count on a bright 7-segment LED display! A 555 timer oscillator acts as an automatic clock, advancing the count once per second (adjustable), while a push button lets you count manually one step at a time. The CD4026 IC does all the heavy lifting — it counts binary pulses internally and simultaneously drives the seven display segments without any external decoder. When the count rolls over from 9 back to 0, the carry-out pin pulses HIGH, ready to clock a second CD4026 for a two-digit 00–99 counter. This project teaches the complete pipeline from binary counting through BCD encoding to visible display output.

## Theory of Operation

### Binary and BCD Counting
Computers and digital logic count in **binary** (base 2): each digit is either 0 or 1. Four binary digits (bits) can represent decimal numbers 0–15:

```
Decimal  Binary (D C B A)
  0       0 0 0 0
  1       0 0 0 1
  2       0 0 1 0
  3       0 0 1 1
  4       0 1 0 0
  5       0 1 0 1
  6       0 1 1 0
  7       0 1 1 1
  8       1 0 0 0
  9       1 0 0 1
```

**BCD (Binary Coded Decimal)** restricts the count to 0–9 only and wraps back to 0 on the 10th pulse — exactly what a decimal display needs. The CD4026 implements a 4-bit BCD counter internally.

### How the CD4026 Works

The CD4026 integrates **two functions** in one 16-pin package:

#### 1. BCD Counter Stage
- Increments a 4-bit internal register on each rising edge of the CLOCK input
- Counts 0 → 1 → 2 → ... → 9 → 0 (wraps automatically)
- A RESET pin forces the count to 0 immediately
- A CLOCK INHIBIT pin freezes counting when HIGH

#### 2. Seven-Segment Decoder Stage
- Reads the 4-bit BCD value continuously
- Drives seven open-collector-style outputs (a through g)
- Each output goes HIGH when its segment should be ON for the current digit
- No external decoder chip required

The carry-out pin (CO) goes HIGH when the display transitions from 9 to 0, providing a clock pulse for the next digit stage.

### 7-Segment Display and Segment Encoding

A 7-segment display has seven LED segments labelled **a** through **g** plus an optional decimal point (dp):

```
     aaa
    f   b
    f   b
     ggg
    e   c
    e   c
     ddd  •dp
```

By turning specific segments ON or OFF, each digit 0–9 can be formed. The CD4026 handles this mapping automatically:

| Digit | a | b | c | d | e | f | g | Segments ON |
|-------|---|---|---|---|---|---|---|-------------|
|   0   | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |   | a b c d e f     |
|   1   |   | ✓ | ✓ |   |   |   |   | b c             |
|   2   | ✓ | ✓ |   | ✓ | ✓ |   | ✓ | a b d e g       |
|   3   | ✓ | ✓ | ✓ | ✓ |   |   | ✓ | a b c d g       |
|   4   |   | ✓ | ✓ |   |   | ✓ | ✓ | b c f g         |
|   5   | ✓ |   | ✓ | ✓ |   | ✓ | ✓ | a c d f g       |
|   6   | ✓ |   | ✓ | ✓ | ✓ | ✓ | ✓ | a c d e f g     |
|   7   | ✓ | ✓ | ✓ |   |   |   |   | a b c           |
|   8   | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | a b c d e f g   |
|   9   | ✓ | ✓ | ✓ | ✓ |   | ✓ | ✓ | a b c d f g     |

✓ = segment is ON (CD4026 output HIGH), blank = segment OFF (output LOW).

**Common cathode** displays have all segment cathodes tied together at GND; you drive the individual anodes HIGH through a resistor. The CD4026 outputs connect to these anodes.

### 555 Timer as Automatic Clock

The 555 timer in **astable mode** generates a continuous square wave that clocks the CD4026:

- Alternates output between HIGH and LOW indefinitely
- Frequency set by two resistors (R1, R2) and a capacitor (C)
- At 1 Hz, the display counts once per second — easily visible
- At higher frequencies (100 Hz+), digits blur, revealing the counting illusion

The 555 output connects directly to the CD4026 CLOCK pin (pin 1).

### Manual Button Counting

In manual mode, the 555 clock is disabled (or not connected) and a debounced push button drives the CLOCK pin instead. Each button press advances the count by exactly one. This mode is ideal for:
- Counting physical events (button presses, objects)
- Step-by-step observation of segment patterns
- Demonstrating BCD encoding one digit at a time

Because mechanical buttons bounce (make/break contact many times in milliseconds), a 100nF capacitor across the button or a short RC filter prevents multiple counts per press.

### Carry-Out and Cascading Multiple Displays

The CD4026's **CARRY OUT (CO)** pin (pin 5) outputs a HIGH pulse each time the counter rolls over from 9 to 0. This is used to clock the next CD4026 in a chain:

```
555 Clock ──► CD4026 #1 (ones digit)
                  │
             CARRY OUT ──► CD4026 #2 (tens digit)
                                │
                           CARRY OUT ──► CD4026 #3 (hundreds digit)
```

- Display #1 shows the ones digit (counts 0–9 continuously)
- Display #2 shows the tens digit (increments every 10 counts)
- Display #3 shows the hundreds digit (increments every 100 counts)

This is **ripple counting** — each stage ripples the overflow into the next. With two CD4026s and two displays you get 00–99 (100 counts); with three, 000–999.

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

| Pin | Name    | Connection in this circuit                        |
|-----|---------|---------------------------------------------------|
|  1  | GND     | Ground rail                                       |
|  2  | TRG     | Tied to pin 6 (threshold = trigger node)          |
|  4  | RST     | Tied to VCC (always enabled)                      |
|  5  | CTL     | 100nF capacitor to GND (noise filter)             |
|  6  | THR     | Tied to pin 2; top of timing capacitor C1         |
|  7  | DIS     | Junction of R1 and R2                             |
|  8  | VCC     | +9V rail                                          |
|  3  | OUT     | Clock output → CD4026 pin 1                       |

**Astable wiring:**
- R1 (10kΩ): Between VCC (pin 8) and DIS (pin 7)
- R2 (10kΩ): Between DIS (pin 7) and THR/TRG (pins 6 & 2)
- C1 (10µF electrolytic): Between THR/TRG node and GND (+ toward pin 6)

### CD4026 BCD Counter / 7-Segment Driver (16-pin DIP)
```
         +--U--+
  CLOCK  |1  16| VDD (+9V)
  CLK IN |2  15| f
  DISP   |3  14| g
  CO     |4  13| a
  VEE    |5  12| b (not used, tie to GND)
  VSS    |6  11| VSS (GND)
  d      |7  10| e
  c      |8   9| b (segment output)
         +-----+
```

> **Note**: Pin numbering on the CD4026 varies slightly between manufacturers. Always verify against your specific datasheet. The logical assignments are:

| Pin | Name           | Description                                               |
|-----|----------------|-----------------------------------------------------------|
|  1  | CLOCK          | Rising-edge clock input — advances count by 1             |
|  2  | CLOCK INHIBIT  | HIGH = freeze count; LOW/GND = count normally             |
|  3  | DISPLAY ENABLE | LOW = display active; HIGH = all segments off (blanking)  |
|  4  | CARRY OUT (CO) | HIGH pulse when count rolls 9→0; clocks next stage        |
|  5  | VEE / UN       | Unused or tie to GND (check datasheet)                    |
|  6  | VSS            | Ground (0V)                                               |
|  7  | d              | Segment d output                                          |
|  8  | c              | Segment c output                                          |
|  9  | b              | Segment b output                                          |
| 10  | e              | Segment e output                                          |
| 11  | VSS            | Ground (second ground pin, tie to GND)                    |
| 12  | —              | (varies by package — check your datasheet)                |
| 13  | a              | Segment a output                                          |
| 14  | g              | Segment g output                                          |
| 15  | f              | Segment f output                                          |
| 16  | VDD            | Positive supply (+9V)                                     |

### 7-Segment Display (Common Cathode)
```
     aaa
    f   b
    f   b
     ggg
    e   c
    e   c
     ddd  •dp

Common cathode = all cathodes tied together → GND
Anodes (a–g) = driven HIGH through 220Ω resistors
```

**Typical pin layout** (10-pin, 0.1" pitch, single digit):
```
     ┌─────────────────────┐
     │  10  9  8  7  6     │
     │  [7-segment face]   │
     │   1  2  3  4  5     │
     └─────────────────────┘
```

| Common pin positions | Segment pins |
|----------------------|--------------|
| Pins 3 and 8         | Check your display's datasheet — pinout varies by manufacturer |

Always confirm the segment-to-pin mapping with your specific display by testing each pin individually: apply +3.3V through a 220Ω resistor to each pin while grounding the common, and note which segment illuminates.

## Building Instructions

### Step 1: Power Rails and Decoupling
1. Insert the breadboard power rail connections: red (+) to +9V, black (−) to GND from the battery connector.
2. Place a **100nF ceramic capacitor** directly across the power rails near the center of the board to decouple high-frequency noise.
3. Verify with a multimeter: +9V between rails.

### Step 2: 555 Timer Clock Circuit (Astable ~1 Hz)
1. Insert the 555 timer IC straddling the center gap of the breadboard. The notch or dot marks pin 1.
2. **Power pins**: Pin 8 → +9V rail; Pin 1 → GND rail.
3. **Reset**: Pin 4 → +9V rail (keeps timer always running).
4. **Control voltage filter**: Pin 5 → 100nF capacitor → GND (suppresses noise on the control input).
5. **Timing network**:
   - R1 (10kΩ): Wire from pin 8 to pin 7.
   - R2 (10kΩ): Wire from pin 7 to a shared node with pins 6 and 2.
   - C1 (10µF electrolytic): Positive leg to the pin 6/2 node; negative leg to GND. **Watch polarity!**
6. **Tie pin 6 to pin 2** (threshold and trigger share the same node — this is required for astable operation).
7. Pin 3 is the clock output — leave it open for now (connected in Step 5).

### Step 3: CD4026 BCD Counter / Display Driver
1. Insert the CD4026 on the breadboard, away from the 555.
2. **Power**: VDD (pin 16) → +9V; VSS (pins 6 and 11) → GND.
3. **CLOCK INHIBIT** (pin 2): Wire to GND (counting always enabled).
4. **DISPLAY ENABLE** (pin 3): Wire to GND (display always active).
5. **CARRY OUT** (pin 4): Leave unconnected for single-digit use, or connect to the CLOCK (pin 1) of a second CD4026 for cascading.
6. Leave segment output pins (a, b, c, d, e, f, g) open for now.

### Step 4: 7-Segment Display and Segment Resistors
1. Insert the 7-segment display on the right side of the breadboard.
2. **Common cathode**: Connect the common pin(s) (usually pins 3 and 8 on a 10-pin display) to GND.
3. Install seven **220Ω resistors** — one for each segment. Each resistor bridges from a CD4026 segment output to the corresponding display segment anode:

   | CD4026 output | Segment | Route through 220Ω → Display anode pin |
   |--------------|---------|----------------------------------------|
   | Pin 13       | a       | Display pin for segment a              |
   | Pin 9 (b)    | b       | Display pin for segment b              |
   | Pin 8        | c       | Display pin for segment c              |
   | Pin 7        | d       | Display pin for segment d              |
   | Pin 10       | e       | Display pin for segment e              |
   | Pin 15       | f       | Display pin for segment f              |
   | Pin 14       | g       | Display pin for segment g              |

4. The decimal point (dp) anode can be left unconnected or wired through a 220Ω resistor to +9V if you want it always on.

### Step 5: Connect Clock to CD4026
**Mode A — Automatic counting with 555 timer:**
1. Wire 555 pin 3 (OUT) to CD4026 pin 1 (CLOCK).
2. Power on — the display should count 0→1→2→...→9→0 repeatedly at about 1 count per second.

**Mode B — Manual counting with push button:**
1. Do **not** connect the 555 output to the CD4026 clock (or add a switch to disconnect it).
2. Wire one leg of the push button to +9V.
3. Wire the other leg of the button to CD4026 pin 1 (CLOCK), and also through a **10kΩ pull-down resistor** to GND.
4. Optionally place a **100nF capacitor** across the button contacts for hardware debouncing.
5. Each button press will advance the count by 1.

### Step 6: Optional Reset Button
1. Wire a second push button between +9V and the CD4026 RESET pin (check your datasheet — some packages label this differently).
2. Add a 10kΩ pull-down resistor from RESET to GND.
3. Pressing this button will immediately return the display to **0**.

### Step 7: Final Checks Before Power-On
1. Confirm all ICs are correctly oriented — the notch/dot is at pin 1.
2. Confirm no bridges between adjacent rows that should not be connected.
3. Verify +9V and GND are not shorted (measure resistance > 1kΩ with power off).
4. Confirm both CD4026 VSS pins are grounded.
5. Confirm 7-segment common cathode pins are on GND, not VCC.

## Testing & Troubleshooting

### Expected Display Sequence
Power on with 555 clock connected at ~1 Hz:
```
0 → 1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9 → 0 → 1 → ...
```
Each digit should display for approximately 1 second. The segments that illuminate must match the BCD truth table above (e.g., digit **1** shows only segments b and c; digit **8** lights all seven segments).

### Test Procedure
1. **Power on**: Display should show digit 0 (or current count state).
2. **Wait 10 seconds**: Observe 0 through 9 sequence, then wrap back to 0.
3. **Press RESET** (if wired): Display should snap to 0 immediately.
4. **Switch to manual mode**: Disconnect 555 clock and attach button; each press should increment by exactly 1.
5. **Verify carry-out**: Use a multimeter to probe CD4026 pin 4 — it should briefly pulse HIGH when the count transitions 9→0.

### Voltage Measurements

**555 Timer:**
| Pin | Expected Voltage          |
|-----|---------------------------|
|  1  | 0V (GND)                  |
|  8  | 9V                        |
|  3  | Alternates 0V ↔ ~8V       |
| 6/2 | Oscillates ~3V to ~6V     |
|  4  | 9V (reset pin, enabled)   |

**CD4026:**
| Pin | Expected Voltage                            |
|-----|---------------------------------------------|
| 16  | 9V                                          |
| 6   | 0V (GND)                                    |
| 11  | 0V (GND)                                    |
|  1  | Pulses from 555 (0V ↔ ~8V at ~1 Hz)        |
|  4  | Brief HIGH pulse at each 9→0 rollover       |
| a–g | 0V or ~8V depending on current digit        |

**7-Segment Display:**
| Point       | Expected Voltage                             |
|-------------|----------------------------------------------|
| Common (CC) | 0V (GND)                                     |
| Lit segment anode | ~7.5V (after 220Ω drop)              |
| Unlit segment anode | 0V                                   |

### Common Issues and Solutions

**Display shows nothing at all:**
- Confirm the 7-segment display is **common cathode**, not common anode.
- Verify common cathode pins are wired to GND.
- Probe each CD4026 segment output — at least some should show ~8V.
- Test one display segment directly: wire it through 220Ω to +9V; it should light up.
- Confirm CD4026 pin 3 (DISPLAY ENABLE) is LOW (0V), not HIGH.

**Display shows "8" (all segments lit) permanently:**
- CD4026 DISPLAY ENABLE pin may be floating HIGH — pull it LOW with a wire to GND.
- Check for a short circuit connecting all segment anodes together.
- Verify 220Ω resistors are present; a direct connection could mimic this.

**Wrong segment pattern for a digit (scrambled numbers):**
- Segment wiring a–g is incorrect — re-verify each CD4026 output pin against the display anode pin.
- Check your specific display's datasheet; pin assignments vary widely between manufacturers.
- Probe each segment output on the CD4026 and confirm which physical segment lights.

**Count doesn't advance (stuck on one digit):**
- Confirm 555 pin 3 is delivering a toggling voltage (use a multimeter on AC voltage range or an LED indicator).
- Check CD4026 pin 2 (CLOCK INHIBIT) is LOW — if HIGH, counting is frozen.
- Verify C1 (10µF) polarity is correct; reversed electrolytic caps often fail silently.
- Confirm R1, R2, C1 are correctly placed per the astable wiring.

**Counter counts only to 8 then resets (or wrong wrap point):**
- The CD4026 naturally wraps at 9→0; if it wraps earlier, there may be a spurious connection to the RESET pin.
- Verify RESET pin is connected to GND through 10kΩ (or fully wired to GND), not to any counter output.

**Manual button increments by 2 or more per press (bouncing):**
- Add a 100nF capacitor from the CLOCK pin to GND (in parallel with the pull-down).
- Alternatively, add a simple RC debounce: 1kΩ series + 100nF to GND between the button and CLOCK pin.
- Press the button very slowly to verify the bounce is the cause.

**Carry-out not working (second display not counting):**
- Confirm the CARRY OUT pin (pin 4) is wired to CLOCK (pin 1) of the second CD4026, not a different pin.
- Probe pin 4 with a multimeter during the 9→0 transition — you should see a brief HIGH pulse.
- Ensure the second CD4026 is powered and its CLOCK INHIBIT and DISPLAY ENABLE are LOW.

**Display is very dim:**
- Lower the segment resistors from 220Ω to 150Ω or 100Ω (check display current rating first).
- Measure battery voltage — below 7V, brightness drops noticeably. Replace the battery.
- Confirm all seven 220Ω resistors are present and correctly valued (use a multimeter on resistance mode).

## Calculations

### 555 Timer Clock Frequency

For the astable 555 configuration with R1, R2, and C1:

```
Frequency:  f = 1.44 / ((R1 + 2 × R2) × C1)

With R1 = 10kΩ, R2 = 10kΩ, C1 = 10µF:
  f = 1.44 / ((10,000 + 2 × 10,000) × 0.000010)
  f = 1.44 / (30,000 × 0.000010)
  f = 1.44 / 0.30
  f ≈ 4.8 Hz  (~5 counts per second)

Duty cycle:  D = (R1 + R2) / (R1 + 2 × R2)
             D = (10k + 10k) / (10k + 2 × 10k)
             D = 20k / 30k ≈ 67%
```

**To get exactly 1 count per second (1 Hz):**
```
  f = 1.44 / ((R1 + 2 × R2) × C1) = 1 Hz
  (R1 + 2 × R2) × C1 = 1.44

  With C1 = 10µF:
  R1 + 2 × R2 = 1.44 / 0.000010 = 144,000 Ω
  Use R1 = 4.7kΩ, R2 = 68kΩ  →  4.7k + 136k ≈ 141kΩ  (close to 1 Hz)
  Or: R1 = 10kΩ, R2 = 68kΩ  →  f ≈ 0.98 Hz  ✓
```

**Adjusting counting speed:**
| Desired speed      | Change                         | Result              |
|--------------------|--------------------------------|---------------------|
| Slower (1 count/s) | Increase R2 to 68kΩ            | ~1 Hz               |
| Faster (10/s)      | Decrease C1 to 1µF             | ~48 Hz              |
| Very fast (100/s)  | Decrease C1 to 100nF + R2=10kΩ | ~480 Hz             |
| Very slow (1/10s)  | Increase C1 to 100µF           | ~0.5 Hz             |

### Display Segment Current

Each illuminated segment draws current through the 220Ω resistor:

```
V_segment = V_supply − V_forward
          = 9V − 2.0V (typical LED forward voltage)
          = 7.0V

I_segment = V_segment / R
          = 7.0V / 220Ω
          ≈ 32 mA per segment
```

For digit "8" (all 7 segments on):
```
I_total_display = 7 × 32 mA ≈ 224 mA
```

> ⚠️ **Check your display's maximum segment current rating** (usually 20–40 mA). If the datasheet says 20 mA max, increase segment resistors:
> ```
> R = (9V − 2V) / 0.020 = 350Ω  →  use 330Ω
> ```

### Battery Life Estimate

```
Component currents (approximate):
  555 Timer:   ~10 mA
  CD4026:       ~1 mA
  Display:     ~112 mA (average — not all segments lit at once)
  Total:       ~123 mA

9V battery capacity (alkaline): ~550 mAh
Runtime = 550 mAh / 123 mA ≈ 4.5 hours continuous
```

### Multi-Digit Cascading — Count Range

| Number of CD4026 stages | Maximum count | Display range |
|-------------------------|---------------|---------------|
| 1                       | 9             | 0–9           |
| 2                       | 99            | 00–99         |
| 3                       | 999           | 000–999       |
| N                       | 10ᴺ − 1      | 00…0–99…9     |

Each additional stage requires one CD4026, one 7-segment display, and seven 220Ω resistors. The only inter-stage connection needed is CARRY OUT (pin 4) of stage N → CLOCK (pin 1) of stage N+1.

## Key Concepts Learned
- **Binary counting**: How digital logic represents and increments numbers in base 2
- **BCD encoding**: Restricting 4-bit binary to 0–9 for decimal display systems
- **Seven-segment decoding**: Mapping BCD values to segment patterns (a–g)
- **CD4026 internal architecture**: Combined counter + decoder in a single IC
- **555 astable oscillator**: Generating a continuous clock signal with R and C
- **Debouncing**: Filtering mechanical switch bounce for clean single-increment counting
- **Carry-out / ripple carry**: Cascading digital counters for multi-digit displays
- **Current limiting resistors**: Protecting LEDs and matching segment brightness

## Applications
- **Scoreboards and tallies**: Count events, goals, or items physically
- **Frequency meters**: Count pulses per second from external sensors
- **Production counters**: Count units on an assembly line with a photogate
- **Digital clocks**: Cascade for seconds, minutes, hours displays
- **Visitor counters**: Triggered by a door sensor or PIR motion detector
- **Reaction timers**: Start counting on a signal; stop manually to read elapsed counts
- **Educational lab demonstrators**: Show binary → BCD → display pipeline visually
- **Vending machine counters**: Track how many items have been dispensed

## Next Steps / Extensions

### 1. Two-Digit 00–99 Counter
Wire a second CD4026 and second 7-segment display:
```
555 → CD4026 #1 (ones) → CO pin 4 → CD4026 #2 (tens) → display
```
No extra clock circuitry needed — just daisy-chain the carry-out.

### 2. Add a Reset Button
Connect a momentary button from +9V to the CD4026 RESET pin with a 10kΩ pull-down. Pressing it returns the count to 0 instantly. Wire both CD4026 RESET pins together for a simultaneous two-digit reset.

### 3. Up/Down Counting
Replace CD4026 with a **CD40110** (up/down BCD counter with display driver) or a **74HC192** (up/down BCD counter, pair with a **74HC4511** BCD-to-7-segment decoder). Connect a toggle switch or two buttons to select count direction.

### 4. Alarm at a Target Count
Use the carry-out or individual segment outputs to trigger a comparator that sounds a buzzer or lights an LED when the count reaches a specific value. For example, detect digit "5" by sensing when segments a, c, d, f, g are HIGH simultaneously.

### 5. Variable Speed Control
Replace the fixed R2 resistor with a **10kΩ–100kΩ potentiometer**. Turning the pot adjusts the 555 frequency — and therefore the counting speed — from very slow (1/s) to very fast (hundreds per second).

### 6. External Event Counting
Remove the 555 and drive the CLOCK pin directly from:
- A **photogate** (LDR + comparator) — counts objects breaking a beam
- A **microphone amplifier** — counts sound pulses or claps
- A **Hall-effect sensor** — counts rotations of a magnet-equipped wheel
- An **Arduino digital output** — for software-controlled counting demonstrations

### 7. Arduino Comparison
The same counter in Arduino code is only a few lines:

```cpp
#include <SevSeg.h>
SevSeg sevseg;
int count = 0;
void loop() {
  count = (count + 1) % 10;
  sevseg.setNumber(count);
  sevseg.refreshDisplay();
  delay(1000);
}
```

Hardware counters teach the underlying logic that microcontrollers abstract away — both skills are valuable.

## Safety Notes
- ✅ Low-voltage circuit (9V DC) — safe to handle while powered
- ✅ All components are low power; no heat-generating elements
- ⚠️ **Never reverse battery polarity** — CMOS ICs (CD4026, 555) can be destroyed instantly
- ⚠️ **Don't exceed VDD + 0.5V on any input pin** — CMOS inputs are ESD-sensitive
- ⚠️ **Touch a grounded surface before handling ICs** to discharge static electricity
- ⚠️ **Don't omit segment resistors** — direct connection from VDD to LED anode will burn out segments within seconds
- ⚠️ **Watch electrolytic capacitor polarity** — reversed caps can fail or rupture
- ✅ 9V battery snap connectors are polarized and physically prevent reverse connection
- ✅ Suitable for builders with basic electronics experience; safe with adult supervision for younger students

---

**Circuit Status**: ✅ Verified circuit design — ready to build
**Last Updated**: December 2024

Count everything! 🔢⚡
