/*
 * Project Name: RGB LED color mixer
 * Description: Uses 3 potentiometers representing Red, Green, and Blue to mix colors
 * Author: Semire Ajayi
 * Date: 18-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - 1 RGB LED
 * - 6 220-ohm resistors (one for each led and button)
 * - 3 potentiometers
 * - 1 9v battery (optional)
 * 
 * Connections:
 * - Red LED -> Pin 9
 * - Green LED -> Pin 10
 * - Blue LED -> Pin 11

 * - Red Potentiometer -> Pin A0
 * - Green Potentiometer -> Pin A1
 * - Blue  Potentiometer-> Pin A2
 */
 
#define REDPOTPIN   A0
#define GREENPOTPIN A1
#define BLUEPOTPIN  A2

#define REDPIN   9
#define GREENPIN 10
#define BLUEPIN  11

void setup()
{
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  int redPot   = analogRead(REDPOTPIN);
  int greenPot = analogRead(GREENPOTPIN);
  int bluePot  = analogRead(BLUEPOTPIN);

  int red   = map(redPot,   0, 1023, 0, 255);
  int green = map(greenPot, 0, 1023, 0, 255);
  int blue  = map(bluePot,  0, 1023, 0, 255);

  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);

  Serial.println(red);
}
