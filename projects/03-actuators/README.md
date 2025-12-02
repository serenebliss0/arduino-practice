# Actuator Projects

This folder contains projects that control physical outputs like motors, servos, and relays.

## Common Actuators

### Motors
- **DC Motors**: Simple motors with motor drivers (L298N, L293D)
- **Servo Motors**: Precise angle control (SG90, MG995)
- **Stepper Motors**: Precise position control (28BYJ-48, NEMA17)

### Switching Devices
- **Relays**: Control high-power devices
- **Solenoids**: Linear motion actuators
- **Transistors**: Electronic switches

### Sound Devices
- **Buzzers**: Passive and active buzzers
- **Speakers**: Audio output

## Key Concepts

- PWM (Pulse Width Modulation)
- Motor control and direction
- Speed control
- Power requirements and external power supplies
- Back-EMF protection (diodes)
- H-bridge circuits
- Servo library usage
- Stepper motor sequencing

## Safety Considerations

⚠️ **Important Safety Notes:**
- Never connect motors directly to Arduino pins
- Use motor drivers or transistors
- Use external power supply for motors
- Add flyback diodes to prevent voltage spikes
- Be careful with high-voltage relay projects
- Check current ratings of all components

## Common Components

- Motor drivers (L298N, L293D, DRV8825)
- Power supplies (9V, 12V battery or adapter)
- Diodes for back-EMF protection
- Transistors (TIP120, 2N2222)
- Capacitors for noise filtering
