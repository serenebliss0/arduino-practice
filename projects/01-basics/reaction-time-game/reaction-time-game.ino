/*
 * Project Name: Simple reaction time game 
 * Author: Semire Ajayi
 * Date: 25-DEC-2025 (Merry Christmas!)
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - 1 LED 
 * - 1 220-ohm resistor (for the led)
 * - 1 10k-ohm resistor
 * - 1 momentary switch
 * - 1 9v battery (optional)
 * 
 * Connections:
 * - LED -> Pin 10
 * - Push Button -> Pin 9
 */

#define LEDPIN 10
#define BUTTON 9

long startTime = 0;
long timeToPush = 0;

bool ledOn = false;
bool ledTriggered = false;
bool lastButtonState = HIGH;
unsigned long ledOnTime = 0;

long chooseRandomTime() 
{
  return random(1000, 10000);
}

void setup()
{
  Serial.begin(19200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  startTime = millis();
  timeToPush = chooseRandomTime();
}

void loop() 
{
  long currentTime = millis() - startTime;
  bool buttonState = digitalRead(BUTTON);

  // Turn LED on ONCE
  if (!ledTriggered && currentTime >= timeToPush) 
  {
    digitalWrite(LEDPIN, HIGH);
    ledOn = true;
    ledTriggered = true;
    ledOnTime = millis();
  }

  // Turn LED off after 1 seconds
  if (ledOn && millis() - ledOnTime >= 1000) 
  {
    digitalWrite(LEDPIN, LOW);
    ledOn = false;
  }

  // Detect button press during LED window
  if (lastButtonState == HIGH && buttonState == LOW && ledOn) 
  {
    Serial.println("You pushed at the right time");
  }

  if(lastButtonState == HIGH && buttonState == LOW && !ledOn)
  {
    Serial.println("You didn't push at the right time though...");
  }

  lastButtonState = buttonState;

  // Restart game
  if (currentTime >= 10000)
  {
    digitalWrite(LEDPIN, LOW);
    ledOn = false;
    ledTriggered = false;
    startTime = millis();
    timeToPush = chooseRandomTime();
    Serial.println("--- GAME RESTARTED ---");
  }
}
