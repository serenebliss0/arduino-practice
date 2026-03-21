#include <Arduino.h>

#define LDR_PIN A0
#define LED_PIN 6

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  int total = 0;

  for (int i = 0; i < 20; i++) {
    total += analogRead(LDR_PIN);
    delay(5);
  }

  int lightValue = total / 20;

  Serial.println(lightValue);

  // Hysteresis
  static bool ledState = false;

  if (lightValue < 250) {
    ledState = true;
  }

  if (lightValue > 350) {
    ledState = false;
  }

  digitalWrite(LED_PIN, ledState);

  delay(100);
}