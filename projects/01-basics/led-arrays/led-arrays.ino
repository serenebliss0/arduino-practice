/*
 * Project Name: LED Arrays
 * Description: Initialize multiple LEDs using an array
 * Author: Semire Ajayi
 * Date: 07-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - 6 LEDs of different colors
 * - 6 220-ohm resistors
 * 
 * Connections:
 * - LEDs -> Pins 2 to 7
 */

int leds[] = {2,3,4,5,6,7};

int len = sizeof(leds) / sizeof(leds[0]); //find the length of the leds array

void setup()
{
  for (int i = 0; i < len; i ++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (int i = 0; i < len; i ++)
  {
    digitalWrite(leds[i], HIGH);
    delay(100);
  }

  for (int j = 0; j < len; j++)
  {
    digitalWrite(leds[j], LOW);
    delay(100);
  }
}