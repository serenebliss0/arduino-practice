#include <Arduino.h>
/*
// ESP32 Blink Example
// Built-in LED is usually on GPIO 2

//#define LED_PIN 2  // Built-in LED pin

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Blink Test");
  
  // Set LED pin as output
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn LED on
  Serial.println("LED ON");
  delay(1000);                   // Wait 1 second
  
  digitalWrite(LED_PIN, LOW);    // Turn LED off
  Serial.println("LED OFF");
  delay(1000);                   // Wait 1 second
}
*/

#include <WiFi.h>

const char* ssid = "WIFINAME";
const char* password = "psswd";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP("Serenity's ESP32 may or may not have a virus", "Serenity");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Your code here
  delay(1000);
}