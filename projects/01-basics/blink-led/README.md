# Blink LED

The classic "Hello World" of Arduino programming - makes the built-in LED blink on and off.

## Description

This is the most basic Arduino project that demonstrates digital output. The program turns the built-in LED on for one second, then off for one second, continuously. This project is perfect for:
- Testing that your Arduino board works
- Learning the basic structure of Arduino code (setup and loop)
- Understanding digital output with digitalWrite()
- Getting familiar with the Arduino IDE and upload process

## Components Required

- Arduino board (Uno, Nano, Mega, or any board with built-in LED)
- USB cable for programming

**Note**: No additional components needed! This project uses the built-in LED.

## Circuit Diagram

```
No external wiring needed!

Built-in LED is connected to pin 13 on most Arduino boards.
The LED is already connected internally with a current-limiting resistor.
```

## Wiring Instructions

No wiring required! Just connect your Arduino to your computer via USB cable.

## Code Explanation

### Setup Function
```cpp
pinMode(LED_PIN, OUTPUT);
```
- Configures pin 13 as an output pin
- This tells the Arduino we want to control the voltage on this pin
- Must be done in setup() before using the pin

### Loop Function
```cpp
digitalWrite(LED_PIN, HIGH);  // Turn LED on (5V)
delay(1000);                   // Wait 1 second
digitalWrite(LED_PIN, LOW);   // Turn LED off (0V)
delay(1000);                   // Wait 1 second
```
- `digitalWrite(pin, HIGH)` sets the pin to 5V (LED turns on)
- `digitalWrite(pin, LOW)` sets the pin to 0V (LED turns off)
- `delay(1000)` pauses the program for 1000 milliseconds (1 second)
- This pattern repeats forever in the loop

### Key Concepts
- **setup()**: Runs once when the Arduino starts or resets
- **loop()**: Runs repeatedly after setup() completes
- **pinMode()**: Configures a pin as INPUT or OUTPUT
- **digitalWrite()**: Sets a digital pin HIGH (5V) or LOW (0V)
- **delay()**: Pauses program execution for specified milliseconds

## How to Use

1. Open the Arduino IDE
2. Connect your Arduino board via USB
3. Select your board: Tools > Board > [Your Arduino Board]
4. Select the correct port: Tools > Port > [Your Arduino Port]
5. Open this sketch (blink-led.ino)
6. Click the Upload button (right arrow icon)
7. Watch the built-in LED blink!

## Expected Output

### Visual
The built-in LED (usually near pin 13) will:
1. Turn ON for 1 second
2. Turn OFF for 1 second
3. Repeat continuously

### Serial Monitor Output
```
Blink LED project started!
LED ON
LED OFF
LED ON
LED OFF
...
```

To view: Open Tools > Serial Monitor and set baud rate to 9600

## Troubleshooting

| Issue | Solution |
|-------|----------|
| LED doesn't blink | Check that the correct board and port are selected in the IDE |
| Upload error | Make sure no other program is using the serial port |
| Wrong LED blinking | Some boards have multiple LEDs; pin 13 is the standard one |
| LED always on/off | Verify the code uploaded successfully (IDE shows "Done uploading") |

## Experiments and Modifications

Try these variations to learn more:

1. **Change Blink Speed**
   ```cpp
   delay(500);  // Faster blinking (0.5 seconds)
   delay(2000); // Slower blinking (2 seconds)
   ```

2. **Different Pattern**
   ```cpp
   digitalWrite(LED_PIN, HIGH);
   delay(100);  // Quick flash
   digitalWrite(LED_PIN, LOW);
   delay(2000); // Long pause
   ```

3. **SOS Pattern**
   ```cpp
   // Three short blinks (S)
   // Three long blinks (O)
   // Three short blinks (S)
   ```

4. **Use an External LED**
   - Connect LED anode (+) to pin 13
   - Connect 220Ω resistor to LED cathode (-)
   - Connect resistor to GND
   - Both built-in and external LEDs will blink together!

## What I Learned

- Basic Arduino code structure (setup and loop)
- How to configure pins with pinMode()
- How to control digital outputs with digitalWrite()
- How to create timing delays with delay()
- How to upload code to Arduino
- How to use the Serial Monitor for debugging

## Next Steps

After mastering this project, try:
- **Multiple LEDs**: Blink multiple external LEDs in patterns
- **Button Control**: Add a button to control the LED
- **Fading LED**: Use PWM with analogWrite() for fading effects
- **Traffic Light**: Create a 3-LED traffic light simulator

## References

- [Arduino digitalWrite() Reference](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/)
- [Arduino delay() Reference](https://www.arduino.cc/reference/en/language/functions/time/delay/)
- [Arduino Built-in Examples](https://www.arduino.cc/en/Tutorial/BuiltInExamples)

## License

This is a basic example project available in the public domain.
