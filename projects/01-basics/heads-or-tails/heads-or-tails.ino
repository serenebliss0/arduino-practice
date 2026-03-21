/*
 * Project Name: Heads or Tails
 * Description: Turns on an LED to represent either heads or tails
 * Author: Semire Ajayi
 * Date: 11-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - 2 LEDs of different colors
 * - 2 220-ohm resistors
 * 
 * Connections:
 * - Heads LED  -> Pin 11
 * - Tails LED -> Pin 10
*/

#define HEADIOPIN 11
#define TAILIOPIN    10
#define PAUSE 50

int headsCounter;
int tailsCounter;

long loopCounter;
long randomNumber = 0L;

void setup()
{
  Serial.begin(115200);
  headsCounter = 0;
  tailsCounter = 0;
  loopCounter = 0;

  pinMode(HEADIOPIN, OUTPUT);
  pinMode(TAILIOPIN, OUTPUT);
  randomSeed(analogRead(A0)); //seeds the random number generator
}

void loop()
{
  randomNumber = generateRandomNumber();
  digitalWrite(HEADIOPIN, LOW);
  digitalWrite(TAILIOPIN, LOW);

  delay(PAUSE);

  if (randomNumber % 2 == 1) //treat each odd number as heads
  {
    digitalWrite(HEADIOPIN, HIGH);
    headsCounter++; //increment the number of times heads is chosen
  }
  else
  {
  digitalWrite(TAILIOPIN, HIGH); //treat even numbers as tails
  tailsCounter++; //increment number of times tails is chosen
  }

  loopCounter++;

  if (loopCounter % 100 == 0)
  {
    Serial.print("After ");
    Serial.print(loopCounter);
    Serial.print(" Coin flips, heads =  ");
    Serial.print(headsCounter);
    Serial.print(" and tails = ");
    Serial.print(tailsCounter);
    Serial.println();
  }
  delay(PAUSE);
}

//remember to declare functions outside of setup or loop!

long generateRandomNumber()
{
  return random(0, 1000000);
}