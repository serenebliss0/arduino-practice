# Simple Audio Amplifier (LM386) ⭐⭐⭐☆☆

## Difficulty Level

**Beginner** – Intro to real audio amplifier circuits

---

## What You’ll Learn

* What a **power amplifier** is (and how it differs from an op-amp)
* How the **LM386** amplifies audio
* Why **coupling capacitors** are required
* How to safely connect a **phone headphone output**
* How to avoid **hum, screeching, and silence**

---

## Components Required

### IC

* **LM386 Audio Amplifier IC × 1**

### Capacitors

* **10µF electrolytic × 2**

  * Input coupling
  * Optional gain
* **100µF electrolytic × 1**

  * Power supply smoothing
* **220µF electrolytic × 1**

  * Speaker coupling
* **100nF (0.1µF) ceramic × 1**

  * Bypass / decoupling

### Passive

* **10Ω resistor × 1** (optional Zobel)

### I/O

* **8Ω speaker × 1** (0.5–4W)
* **3.5mm audio jack × 1** (TRS)

### Power

* **9V battery × 1**

### Misc

* Breadboard
* Jumper wires

---

## Circuit Description

This circuit takes a **low-level audio signal** from a phone or music player and amplifies it enough to drive an **8Ω speaker**.

The **LM386** is a low-voltage **audio power amplifier**, designed specifically to:

* Run from batteries
* Accept small audio signals
* Drive speakers directly

Unlike chips such as the **LM358**, the LM386 is *meant* to deliver current to a speaker.

---

## Theory of Operation

* Phone audio is an **AC signal** with possible DC offset
* The **input capacitor** blocks DC and passes only audio
* The LM386 amplifies the signal internally
* The **output capacitor** blocks DC so the speaker is safe
* Optional parts reduce noise and instability

---

## LM386 Pin Configuration (8-pin DIP)

```
        +--U--+
GAIN  |1   8|  GAIN
IN–   |2   7|  BYPASS
IN+   |3   6|  VCC (+9V)
GND   |4   5|  OUT
        +-----+
```

---

## Step-by-Step Wiring Instructions

### 1. Power (DO THIS FIRST)

* **Pin 6 → +9V**
* **Pin 4 → GND**

Add supply filtering:

* **100µF capacitor**

  * * → pin 6
  * – → GND
* **100nF capacitor**

  * Between pin 6 and pin 4
  * Close to the IC

✔ Prevents hum and oscillation.

---

### 2. Audio Input (Phone Safe)

**Audio jack pins:**

* Tip = Left
* Ring = Right
* Sleeve = Ground (usually the longest pin)
Wire as mono:

1. Tie **Tip and Ring together**
2. Connect them to **+ side of 10µF capacitor**
3. Connect **– side of that capacitor → pin 3 (IN+)**
4. Connect **Sleeve → GND**
5. Connect **pin 2 (IN–) → GND**

✔ Stable input, no floating noise.

---

### 3. Gain Configuration

* **Pins 1 and 8 NOT connected**

  * Gain = **20** (recommended)

Optional:

* **10µF capacitor between pin 1 (+) and pin 8 (–)**

  * Gain = **200**
  * Louder, noisier

⚠ If squealing or distortion occurs, remove this capacitor.

---

### 4. Bypass Pin

* **Pin 7 → GND through 100nF capacitor**

✔ Reduces hiss and background noise.

---

### 5. Output to Speaker

1. **Pin 5 (OUT) → + side of 220µF capacitor**
2. **– side of 220µF → Speaker +**
3. **Speaker – → GND**

⚠ Never connect the speaker directly to pin 5.

---

### 6. (Optional) Zobel Network

* **10Ω resistor in series with 100nF capacitor**
* From **pin 5 → GND**

✔ Improves stability with real speakers.

---

## Correct Pin Summary

* **Pin 1** – NC
* **Pin 2** – GND
* **Pin 3** – 10µF from phone (L+R tied)
* **Pin 4** – GND
* **Pin 5** – 220µF → speaker
* **Pin 6** – +9V
* **Pin 7** – 0.1µF → GND
* **Pin 8** – NC

---

## Common Mistakes (Verified by Experience)

* Using **LM358 instead of LM386**
* Driving a speaker without an output capacitor
* Forgetting to ground pin 2
* Mixing up VCC and GND
* Powering on with phone volume maxed

---

## Applications

* Portable speakers
* Audio experiments
* Intercoms
* Practice guitar amp (with preamp)

---

**Circuit Status:** ✅ Valid, beginner-safe
**Last Updated:** February 2026
