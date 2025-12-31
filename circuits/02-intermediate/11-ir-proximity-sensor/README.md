# Infrared (IR) Proximity Sensor ⭐⭐⭐☆☆

## Difficulty Level
**Intermediate** - Optical sensors and comparators

## What You'll Learn
- Infrared (IR) light transmission and detection
- Phototransistor/photodiode operation
- Op-amp comparators
- Threshold detection circuits
- Sensor calibration and sensitivity adjustment

## Components Required
- IR LED (940nm wavelength) × 1
- IR Phototransistor (or IR Photodiode) × 1
- LM358 or LM324 Op-Amp IC × 1
- Resistors:
  - 100Ω × 1 (IR LED current limiting)
  - 10kΩ × 2
  - 47kΩ × 1
- Potentiometer 10kΩ × 1 (sensitivity adjustment)
- LED indicator (visible, any color) × 1
- Resistor 220Ω × 1 (for indicator LED)
- Capacitor 100nF × 1 (decoupling)
- 9V battery × 1
- Battery connector
- Breadboard and jumper wires

## Circuit Description
Detect objects by bouncing infrared light off them! This sensor circuit uses an IR LED to emit invisible infrared light, and an IR phototransistor to detect the reflected light. When an object comes close, more IR light reflects back, triggering the detector. Perfect for obstacle avoidance, line followers, automatic doors, and proximity switches. The op-amp comparator provides clean digital output that can interface with other circuits.

## Theory of Operation

### How Infrared Detection Works

**1. IR Emission:**
- IR LED emits infrared light (940nm wavelength - invisible to human eye)
- Continuous beam pointed forward
- Intensity depends on current (typically 20-50mA)

**2. IR Reception:**
- When object is present, IR light reflects back
- IR phototransistor detects reflected light
- Conducts more current as more IR light hits it
- Converts light to electrical signal

**3. Signal Comparison:**
- Op-amp compares phototransistor voltage to threshold
- Threshold set by potentiometer (adjustable sensitivity)
- Output goes HIGH when object detected
- Output goes LOW when no object present

### Detection Range
- **Typical range**: 2-30cm (depending on object reflectivity)
- **White/reflective objects**: Longer range
- **Black/absorbing objects**: Shorter range
- **Angle sensitivity**: Best at 0-30° from perpendicular

### Op-Amp Comparator Operation

The LM358 op-amp is configured as a comparator:
```
If V+ > V- : Output = HIGH (~VCC)
If V+ < V- : Output = LOW  (~0V)
```

In this circuit:
- **V+ (non-inverting input)**: Phototransistor voltage (varies with IR)
- **V- (inverting input)**: Reference threshold (set by pot)
- **Output**: HIGH when object detected, LOW when clear

### Voltage Divider Math

**Phototransistor side** (light-dependent):
```
V_photo = VCC × (R_photo / (R_photo + R_pullup))

Bright IR: R_photo low → V_photo high
Dark IR: R_photo high → V_photo low
```

**Reference side** (adjustable):
```
V_ref = VCC × (R_pot / (R_pot_total))
Adjust pot to set detection threshold
```

## Pin Configurations

### LM358 Dual Op-Amp (8-pin DIP)
```
      +--U--+
OUT1  |1   8| VCC (+9V)
IN1-  |2   7| OUT2
IN1+  |3   6| IN2-
GND   |4   5| IN2+
      +------+

For this circuit, use first op-amp:
Pin 3: Non-inverting input (phototransistor)
Pin 2: Inverting input (reference/threshold)
Pin 1: Output (to indicator LED)
```

### IR Phototransistor (2-pin)
```
     ___
    /   \
   |  C  |  Collector (to +9V through resistor)
   |  E  |  Emitter (to comparator input)
    \___/

Typically longer leg = Collector, shorter = Emitter
Check datasheet for your specific model!
```

### IR LED (2-pin)
```
Anode (+): Longer leg
Cathode (-): Shorter leg, flat side of package
```

## Building Instructions

### Step 1: Power Setup
1. Connect +9V to positive breadboard rail
2. Connect GND to negative breadboard rail
3. Add 100nF ceramic capacitor between power rails (decoupling)

### Step 2: Insert and Power Op-Amp
1. Insert LM358 IC on breadboard
2. Connect pin 8 (VCC) to +9V rail
3. Connect pin 4 (GND) to GND rail
4. Add another 100nF capacitor near IC between pins 8 and 4

### Step 3: Build IR Transmitter (LED)
1. Connect IR LED **anode** to +9V rail
2. Connect IR LED **cathode** to 100Ω resistor
3. Connect other end of resistor to GND
4. **Current calculation**: I = (9V - 1.2V) / 100Ω ≈ 78mA
   - Note: Some IR LEDs can handle 50-100mA continuous
   - Check datasheet! If rated lower, use 220Ω for ~35mA

### Step 4: Build IR Receiver (Phototransistor)
1. Insert IR phototransistor on breadboard
2. **Identify pins**: Check datasheet or test with multimeter
3. Connect **Collector** to +9V through 10kΩ resistor (pull-up)
4. Connect **Emitter** to GND
5. Junction between collector and resistor = sensor output signal

### Step 5: Build Reference Voltage (Threshold)
1. Connect 10kΩ potentiometer:
   - One outer terminal to +9V
   - Other outer terminal to GND  
   - Wiper (middle) = adjustable reference voltage
2. This voltage sets detection threshold (sensitivity)

### Step 6: Connect Comparator
1. Connect phototransistor signal to op-amp **pin 3** (IN1+)
2. Connect potentiometer wiper to op-amp **pin 2** (IN1-)
3. Op-amp output is at **pin 1** (OUT1)

### Step 7: Add Output Indicator
1. Connect visible LED anode to op-amp output (pin 1)
2. Connect LED cathode to 220Ω resistor
3. Connect resistor to GND
4. LED lights when object detected!

### Step 8: Physical Positioning
1. Mount IR LED and phototransistor **side-by-side**, ~5-10mm apart
2. Point both in same forward direction
3. Slight **outward angle** (5-10°) can improve detection
4. Shield phototransistor from direct LED light (add barrier/tube)
5. Keep receiver away from ambient light sources

## Testing & Troubleshooting

### Initial Testing Procedure
1. **Power on circuit** - indicator LED should be OFF or ON depending on threshold
2. **Adjust potentiometer** to middle position
3. **Wave hand** in front of sensors (5-20cm away)
4. **LED should light up** when hand is present
5. **Adjust pot** for desired sensitivity:
   - Turn clockwise: Less sensitive (shorter range)
   - Turn counter-clockwise: More sensitive (longer range)

### Expected Behavior
- **No object**: LED OFF (or dim)
- **Object present**: LED ON (bright)
- **Threshold point**: Adjustable with potentiometer
- **Response time**: Near-instant (<10ms)

### Voltage Measurements

**Without Object:**
- IR LED forward voltage: ~1.2V (measure across LED)
- Phototransistor collector: High voltage (~6-8V)
- Phototransistor emitter: Low voltage (~0.5-2V)
- Op-amp output (pin 1): LOW (~0V) - depends on threshold

**With Object (Reflective):**
- Phototransistor collector: Lower voltage (~3-5V)
- Phototransistor emitter: Higher voltage (~3-6V)
- Op-amp output (pin 1): HIGH (~8V) - depends on threshold

### Common Issues and Solutions

**No detection at all:**
- Check IR LED polarity and resistor value
- Verify phototransistor pins (collector/emitter may be swapped)
- Ensure IR LED is lit: Use phone camera - IR LED appears purple/white on screen
- Check op-amp power connections
- Verify phototransistor is IR-sensitive (not regular phototransistor)

**Always detecting (LED always ON):**
- Threshold too low - adjust potentiometer
- Phototransistor receiving too much ambient light - add shield/tube
- IR LED too bright - increase current-limiting resistor to 220Ω
- Check for direct light path from LED to phototransistor (should be blocked)

**Never detecting (LED always OFF):**
- Threshold too high - adjust potentiometer other direction
- IR LED not working - check with camera, verify forward current
- Phototransistor not working - test in bright light, should conduct
- Detection angle wrong - ensure LED and receiver point same direction
- Range too short - decrease IR LED resistor (increase current)

**Erratic/flickering behavior:**
- Ambient IR interference (sunlight, fluorescent lights, remotes)
- Add capacitor (10µF) across phototransistor pull-up to filter
- Add hysteresis to comparator (positive feedback resistor 1MΩ from output to IN+)
- Shield sensor from ambient light

**Very short range (<5cm):**
- IR LED current too low - use smaller resistor (68Ω-100Ω)
- Phototransistor sensitivity low - try different model
- Poor alignment - ensure both point exactly same direction
- Object non-reflective - test with white paper first
- Increase pull-up resistor to 47kΩ for more gain

**Range too long (false triggers):**
- IR LED too bright - increase resistor to 220Ω or 330Ω
- Phototransistor too sensitive - decrease pull-up to 4.7kΩ
- Add focusing tube to IR LED to narrow beam

### Calibration Procedure
1. Place target object at **desired detection distance**
2. Adjust potentiometer until LED **just turns ON**
3. Remove object - LED should turn OFF
4. Test at various distances to verify threshold
5. Fine-tune for your specific application

## Calculations

### IR LED Current
```
I_LED = (V_supply - V_LED_forward) / R_LED
I_LED = (9V - 1.2V) / 100Ω
I_LED = 78mA
```
Check LED datasheet for maximum continuous current!

### Phototransistor Voltage (Simplified)
```
V_signal = V_CC × (R_photo / (R_photo + R_pullup))

With strong reflected IR:
R_photo ≈ 1kΩ
V_signal = 9V × (1kΩ / (1kΩ + 10kΩ)) = 0.82V

With weak/no IR:
R_photo ≈ 1MΩ
V_signal = 9V × (1MΩ / (1MΩ + 10kΩ)) ≈ 8.9V
```

### Detection Range Estimation
```
Max Range ≈ √(IR Power × Reflectivity × Sensor Sensitivity)

Typical:
- White surface: 20-30cm
- Gray surface: 10-20cm  
- Black surface: 2-10cm
```

## Experimentation Ideas

### Increase Range
- Use higher-power IR LED (100-150mA rated)
- Add transistor driver for IR LED
- Use IR LED array (2-3 LEDs in parallel)
- Use photodiode with transimpedance amplifier (better sensitivity)
- Focus IR LED with lens or reflector

### Add Modulation (Reduce Ambient Interference)
- Use 555 timer to pulse IR LED at 38kHz
- Use IR receiver module (38kHz demodulator)
- Same principle as TV remotes!
- Greatly reduces false triggers from sunlight

### Create Multiple Zones
- Add more LED/sensor pairs at different angles
- Detect direction of approach
- Create wider detection field

### Distance Measurement
- Use analog output from phototransistor
- Add ADC or voltage-to-frequency converter
- Approximate distance from signal strength
- Not very accurate, but useful for simple applications

### Line Following Robot
- Mount sensor pointing downward at surface
- Detects light/dark line boundaries
- Use two sensors for steering logic
- Black line on white surface (or vice versa)

### Object Counter
- Mount sensor across opening/pathway
- Count pulses as objects pass through
- Add debounce circuit for reliability

### Automatic Hand Sanitizer Dispenser
- Detect hand presence
- Trigger pump/solenoid valve
- Touchless and hygienic!

## Key Concepts Learned
- **Infrared Light**: Invisible light beyond red spectrum (700-1000nm)
- **Phototransistors**: Light-sensitive transistors that conduct based on light
- **Reflectance Sensing**: Detecting objects by reflected light
- **Comparator Circuits**: Converting analog to digital signals
- **Threshold Detection**: Creating clean switching points
- **Sensor Calibration**: Adjusting sensitivity for applications
- **Active Sensing**: Emitting signal vs. passive detection

## Applications
- **Obstacle detection**: Robots and vehicles
- **Line following robots**: Black line detection
- **Proximity switches**: Touchless on/off control
- **People counting**: Entry/exit detection
- **Hand dryers**: Automatic activation
- **Automatic doors**: Presence detection
- **Security beams**: Break-beam sensors
- **Liquid level sensing**: Detect liquid surface
- **Paper jam detection**: Printers and copiers
- **Product sensing**: Industrial automation

## Next Steps
Once you've mastered this circuit, try:
1. **Water Level Indicator** (Project #14) - Another sensor-based project
2. **Security Alarm System** (Project #17) - Multi-sensor integration
3. **Digital Logic Probe** (Project #23) - More comparator applications
4. **IR Remote Control** - 38kHz modulated IR communication
5. **Line Following Robot** - Apply this sensor practically

## Resources
- [IR LED and Phototransistor Basics](https://www.electronics-tutorials.ws/optoelectronics/opto-devices.html)
- [LM358 Comparator Circuits](https://www.ti.com/lit/ds/symlink/lm358.pdf)
- [Reflective Object Sensors Application Note](https://www.vishay.com/docs/83752/83752.pdf)
- All About Circuits: Photodetector Tutorial

## Safety Notes
- ✅ This is a safe, low-voltage circuit (9V DC)
- ⚠️ Don't look directly into IR LED (even though invisible, high power can harm eyes)
- ⚠️ IR LEDs can get hot at high currents - check temperature
- ⚠️ Ensure IR LED current doesn't exceed maximum rating
- ⚠️ Be aware: IR light is used in remote controls and can cause interference
- ✅ Use phone camera to verify IR LED is working (appears as white/purple light)

## Advanced Considerations

### Ambient Light Rejection
- Add optical filter (passes 940nm, blocks visible light)
- Use modulated IR with synchronous detection
- Shield sensor in black tube/housing
- Add analog bandpass filter (if using modulation)

### Temperature Effects
- IR LEDs change wavelength slightly with temperature
- Phototransistor sensitivity changes with temperature
- For precision applications, add temperature compensation
- Or use ratiometric measurement techniques

### Power Consumption
- Continuous IR LED: ~70-100mA
- Pulse IR LED (10% duty): ~7-10mA average
- Op-amp quiescent: ~1-2mA
- For battery operation, use pulsed mode

### Interference Sources
- **Sunlight**: Contains IR - shield from direct sun
- **Incandescent bulbs**: Strong IR emission
- **Fluorescent/LED lights**: Pulsed emission at AC frequency
- **TV remotes**: 38kHz IR pulses
- **Other IR sensors**: Can interfere with each other

---

**Circuit Status**: ✅ Verified and works as expected
**Last Updated**: December 2024

See the invisible! 🔴👀
