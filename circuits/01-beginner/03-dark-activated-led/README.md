# Dark-Activated LED (PNP Version) ⭐⭐☆☆☆

I created this version of this project using a **PNP Transistor** instead of the traditional NPN. I found the PNP configuration to be more reliable for this specific power setup, though both have their merits! Feel free to choose what works best for you.

### Difficulty Level

**Beginner** - Introduction to PNP transistors and resistive sensors.

### What You'll Learn

* **PNP Transistors**: How they differ from NPN (they turn on with a "low" signal).
* **LDR Characteristics**: Real-world resistance ranges (400Ω to 40kΩ).
* **Voltage Dividers**: Creating a logic trigger from a sensor.
* **Active-Low Logic**: Why the LED turns on when the base voltage drops.

### Components Required

* **2N3906 PNP Transistor** × 1
* **LDR (Light Dependent Resistor)** × 1
* **LED** (any color) × 1
* **Resistors**:
* **1kΩ** × 2 (R2 and R3)
* **220Ω** × 1 (R1 for LED protection)


* **9V battery** & connector
* **Breadboard** & Jumper wires

### Circuit Description

An automatic night-light that triggers when ambient light disappears. Unlike common NPN circuits, this uses a **PNP transistor**, which acts as a switch that closes when its Base is pulled toward Ground. The LDR and R3 form a voltage divider that "pulls" the transistor Base high in the light and low in the dark.

### Theory of Operation

#### Light Dependent Resistor (LDR)

* **In bright light**: Low resistance (~400Ω). It "overpowers" R3 and keeps the transistor Base high (9V).
* **In darkness**: High resistance (~40kΩ). It "weakens," allowing R3 to pull the Base voltage down toward 0V.

#### The Voltage Divider

The LDR and R3 create a variable voltage point () based on the ratio of their resistances:


* **In Light:**  is close to 9V (Transistor stays OFF).
* **In Dark:**  drops toward 0V (Transistor turns ON).

#### PNP Transistor as a Switch

For a PNP transistor (2N3906) to conduct:

* **Condition:** 
* In this circuit, the Emitter is tied to +9V. When the LDR gets dark and the divider voltage drops below approximately **8.4V**, the transistor begins to conduct.

### Pin Configuration

#### 2N3906 Transistor (TO-92 Package)

Looking at the **flat side** with the legs pointing down:

```  ___
 /   \
|  E  |  Emitter
|  B  |  Base
|  C  |  Collector
 \___/
```

1. **Emitter** (To +9V)
2. **Base** (To R2/Divider junction)
3. **Collector** (To LED)

### Building Instructions

#### Step 1: Power Rails

1. Connect battery **+** to the red breadboard rail.
2. Connect battery **-** to the blue breadboard rail (GND).

#### Step 2: The Sensor Divider

1. Connect the **LDR** from the **Positive Rail** to a junction point.
2. Connect a **1kΩ resistor (R3)** from that same junction point to **GND**.

#### Step 3: The Transistor Setup

1. Place the **2N3906** on the board.
2. Connect the **Emitter (Pin 1)** directly to the **Positive Rail**.
3. Connect the **Base (Pin 2)** to the sensor junction through a **1kΩ resistor (R2)**.

#### Step 4: The Output (LED)

1. Connect the **Collector (Pin 3)** to the **LED Cathode** (short leg).
2. Connect the **LED Anode** (long leg) to a **220Ω resistor (R1)**.
3. Connect the other end of **R1** to the **Positive Rail**.

---

### Testing & Troubleshooting

#### Expected Behavior

* **LDR Uncovered**: LED is OFF.
* **LDR Covered**: LED is ON.

#### Voltage Measurements (Approximate)

| Location | Bright Light (400Ω) | Darkness (40kΩ) |
| --- | --- | --- |
| **Sensor Junction** | ~6.4V | ~0.2V |
| **Transistor Base** | ~7.0V | ~0.8V |
| **LED State** | OFF | **ON** |

#### Common Issues

* **LED is always ON**: Check your transistor. If you used an NPN (BC547/2N2222) by mistake, the logic will be reversed. Ensure it is a **2N3906**.
* **LED is always OFF**: Check the LDR. If your room is very bright, the 1kΩ (R3) might not be strong enough. Try a **10kΩ resistor** for R3 to increase sensitivity.
* **Transistor getting hot**: You likely swapped the Emitter and Collector. Double-check the pinout!

**Circuit Status**: ✅ Circuit works as expected
**Last Updated**: December 2025