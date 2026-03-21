#include <Arduino.h>

int soundPin = A1;

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
}

void loop() {
  int value = analogRead(soundPin);
  Serial.println(value);
  delay(1000);

  if (value > 650)
  {
    digitalWrite(7, HIGH);
    delay(4000);

  }
  else
  {
    digitalWrite(7, LOW);
  }
}