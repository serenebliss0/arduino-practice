/*
 * Project Name: I2C Scanner
 * Description: Find the address of your I2C device when you're unsure
 * Author: Semire Ajayi
 * Date: 29-DEC-2025
 * 
 * Hardware:
 * - An I2C Device
 * - An Arduino (or pretty much any microcontroller that supports I2C)

 * 
 * Connections:
 * - SDA -> Pin A4
 * - SCL -> Pin A5
 * 
 * Libraries Required:
 	* - Wire -> For I2C Communication

 */

#include <Wire.h>
#include<Arduino.h>

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Scanning I2C...");
}

void loop()
{
  for (byte addr = 1; addr < 127; addr++) 
  {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) 
    {
      Serial.print("Found I2C device at 0x");
      Serial.println(addr, HEX);
    }
  }
  delay(2000); // wait 2 sec before scanning again
}
