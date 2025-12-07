/*
 * Project Name: Momentary Switch
 * Description: Using a button to control an LED directly from a button
 * Author: Semire Ajayi
 * Date: 7-DEC-2025

 * Hardware:
 * - Arduino board: UNO Rev 3
 * - Push Button
 * - White LED
 * - 2 220-ohm resistors

 * Connections:
 * - Push Button -> Pin 8
 */

 int buttonPin = 8;

 void setup()
 {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
 }

void loop()
{
  int buttonState = digitalRead(buttonPin);
  delay(20);
  Serial.println(buttonState);
}