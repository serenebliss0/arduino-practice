/*
 * Project Name: Blink LED
 * Description: The classic "Hello World" of Arduino - blinks the built-in LED
 * Author: Arduino Beginner
 * Date: 2025-12-02
 * 
 * Hardware:
 * - Arduino board (any model with built-in LED)
 * - USB cable for programming
 * 
 * Connections:
 * - Uses built-in LED on pin 13 (no external wiring needed)
 * 
 * Libraries Required:
 * - None (uses built-in functions only)
 */

// Define the LED pin (most Arduino boards have built-in LED on pin 13)
#define LED_PIN 13

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  
  // Optional: Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Blink LED project started!");
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  
  // Wait for 1 second (1000 milliseconds)
  delay(1000);
  
  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  
  // Wait for 1 second
  delay(1000);
}
