#include <Arduino.h>
#line 1 "C:\\Users\\Semire\\Downloads\\My Programming Journey 3\\arduino-practice\\projects\\02-sensors\\dht11-temp-sensor\\dht11-temp-sensor.ino"
/*
 * Project Name: Temperature sensor with display
 * Description: Displays the temperature and humidity on an LCD display
 * Author: Semire Ajayi
 * Date: 29-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - DHT11 Temperature Sensor
 * - 16x2 LCD Module

 * 
 * Connections:
 * - DHT11 Data Pin -> Pin D2
 		(I'm using I2C Communication for the LCD)
 * - LCD SDA PIN -> Pin A4
 * - LCD SCL PIN -> Pin A5

 * 
 * Libraries Required:
 	* - Wire -> For I2C Communication
  * - LiquidCrystal_I2C (Frank de Brabander) -> For the lcd display module
 	* - DHTlib (Rob Tillaart) -> Reading from the Temperature sensor

 */


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

//addr of my lcd was 0x27 (some might be 0x3F), 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
// if you're unsure of your i2c address, check `02- sensors/ i2c-scanner`for help

#define outPin 2

//create the dht object
dht DHT;

void setup() {
  lcd.init();        // Initialize the display
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HELLO");
}

void loop()
{
  int readData = DHT.read11(outPin);
	
	float t = DHT.temperature;
	float h = DHT.humidity;
	
	lcd.setCursor(0,0);
	lcd.print("Temp.: ");
	lcd.print(t);
	lcd.print((char)223);	//shows degrees character
	lcd.print("C");

  lcd.setCursor(0,1);
	lcd.print("Humi.: ");
	lcd.print(h);
	lcd.print("%");
	
	delay(2000);
}

