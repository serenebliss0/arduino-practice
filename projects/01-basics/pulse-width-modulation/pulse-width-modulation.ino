/*
 * Project Name: PWM Fading
 * Description: Using PWM on an LED
 * Author: Semire Ajayi
 * Date: 7-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - White LED 
 * - 220-ohm resistor
 * 
 * Connections:
 * - LED -> Pin 3 //using a PWM pin
 * 
 */

 int ledPin = 3; //choose a PWM pin

 void setup()
 {
  pinMode(ledPin, OUTPUT);
 }

 void loop()
 {
  //first, loop though 0 -> 255 (0-- low; 255-- high)

  for (int i = 0; i <255; i++)
  {
    analogWrite(ledPin, i);
    delay(5); //the led blinks too fast. delay it a little : )
  }
  //this will go from low brightness to high brightness

  //now decrease the brightness

  for (int j = 255; j > 0; j--)
  {
    analogWrite(ledPin, j);
    delay(5);
  }

 }