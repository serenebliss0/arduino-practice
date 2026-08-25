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

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "YOUR_WIFI_SSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#endif

#ifndef AP_SSID
#define AP_SSID "ESP32_AP"
#endif

#ifndef AP_PASSWORD
#define AP_PASSWORD "CHANGE_ME_123"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  
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