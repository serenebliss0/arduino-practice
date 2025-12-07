/*
 * Project Name: Automatic Traffic Lights
 * Description: Traffic lights change in brightness depending on how bright the environment is
 * Author: Semire Ajayi
 * Date: 7-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * -Green, Red, and Yellow LEDs
 * -Photoresistor
 * - 3 220-ohm resistors
 * - 1 10-kohm resistor (for the photoresistor)

 * Connections:
 * - Red LED -> Pin D3
 * - Green LED-> Pin D5
 * - Yellow LED -> Pin D6
 * - Photoresistor -> Pin A0
 */

int redPin = 3;
int yellowPin = 5;
int greenPin = 6;
//i used pwm pins for each led for brightness control.

unsigned long startTime = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(A0, INPUT); //connect the photoresistor to pin A0
}

void loop()
{
  unsigned long secondsPassed = (millis() - startTime) / 1000;
  //Serial.println(secondsPassed);

  int lightValue = analogRead(A0);
  delay(200);
  Serial.println(lightValue);

  int timeOfDay = (lightValue > 40) ? 800 : 1600;


  // reset all lights first
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);

  if (secondsPassed <= 10)
  {
    analogWrite(redPin, timeOfDay);
  }
  else if (secondsPassed <= 20)
  {
    analogWrite(yellowPin, timeOfDay);
  }
  else if (secondsPassed <= 40)
  {
    analogWrite(greenPin, timeOfDay);
  }

  // RESET EVERY 40 SECONDS
  if (secondsPassed > 40)
  {
    startTime = millis(); // the REAL reset
  }
}
