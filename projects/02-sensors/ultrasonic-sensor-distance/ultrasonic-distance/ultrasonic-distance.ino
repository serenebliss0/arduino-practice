/*
 * Project Name: Ultrasonic-distance-sensor
 * Description: Displays how far or close an object is
 * Author: Semire Ajayi
 * Date: 29-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - HC-SR04 Ultrasonic Sensor
 * - 16x2 LCD Module
 * - Passive Buzzer

 * 
 * Connections:
 * - Buzzer Pin -> Pin 8
 * - Trig Pin (Ultrasonic Sensor) -> Pin 9
 * - Echo Pin (Ultrasonic Sensor) -> Pin 10

 		(I'm using I2C Communication for the LCD)
 * - LCD SDA PIN -> Pin A4
 * - LCD SCL PIN -> Pin A5


 * Libraries Required:
 * - NewPing -> Easily reading values from the ultrasonic sensor
  * - LiquidCrystal_I2C -> Communicating with the LCD Module
 	* - Wire -> For I2C Communication
 */


#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== HC-SR04 CONFIG =====
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 400

//Buzzer config
#define BUZZER_PIN 8
//the lower the alert distance, the closer the object must be to start beeping
#define ALERT_DISTANCE 50


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// ===== LCD CONFIG =====
//addr of my lcd was 0x27 (some might be 0x3F), 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
// if you're unsure of your i2c address, check `02- sensors/ i2c-scanner`for help

unsigned int distance;

//this will generate a data bar based on the distance
void drawBar(LiquidCrystal_I2C &lcd, int value, int maxValue, int row) {
  int cols = 16;
  int barLength = map(value, 0, maxValue, 0, cols);

  lcd.setCursor(0, row);
  for(int i=0; i<cols; i++)
  {
    if(i < barLength) lcd.write(byte(255)); // full block
    else lcd.print(" ");
  }
}

//controls the buzzer
void beepPassiveBuzzer(int buzzerPin, int frequency, int duration) 
{
  long delayPeriod = 1000000L / frequency / 2; // half period in microseconds
  long cycles = (long)frequency * duration / 1000; // total cycles

  for(long i = 0; i < cycles; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(delayPeriod);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(delayPeriod);
  }
}

void setup() 
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() 
{
  distance = sonar.ping_cm();
  if(distance == 0) distance = MAX_DISTANCE;

  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm   ");
  drawBar(lcd, distance, MAX_DISTANCE, 1);

  if(distance <= ALERT_DISTANCE)
  {
    int beepDelay = map(distance, 0, ALERT_DISTANCE, 50, 500); // closer = faster
    beepPassiveBuzzer(BUZZER_PIN, 1000, 50);
    delay(beepDelay);
  } 
  else 
  {
    delay(100);
  }
}
