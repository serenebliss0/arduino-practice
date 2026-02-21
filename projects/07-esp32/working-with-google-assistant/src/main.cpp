/*
 * Project Name: working with google assistant
 * Description: Uses `Sinric Pro` to communicate with Google Assistant to control LEDs
 * Author: Semire Ajayi
 * Date: 14-02-2026 (Happy Valentines Day ❤️)
 * 
 * Hardware:
 * - ESP32 DevKit V1 (as long as it has WiFi, it should be good)
 * - 10x White LEDs (but any color will do)
 * 
 * Connections:
 * - LEDs 1-10 -> Pins [2,4,5,12,13,14,15,18,19,23]
 * 
 * Libraries Required:
 * - WiFi - Using WiFi features for an esp32
 * -SinricPro- Communicating with SinricPro
 * -SinricProSwitch- The type of device we're creating: a switch
 */

#include <Arduino.h>
#include<WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

int LEDPINS[] = {2,4,5,12,13,14,15,18,19,23};
const char* ssid = "IZEHI_HOTSPOT4";
const char* password = "  semire ajayi";

//from sinric pro dashbaord
//get yours at `portal.sinric.pro`
#define APP_KEY    "9741b1b9-8d9a-435b-894c-d4a2e2b392ed" 
#define APP_SECRET "6314c3eb-8a42-40df-8f5e-1dc01b5f8fbe-acb4bf79-057a-4c54-8c97-05d265118535" 
#define DEVICE_ID "698f54d1da2ae47a6c956e4d" 

//this function is what gives the leds functionality
bool onPowerState(const String &deviceId, bool &state) 
{
  for (int i =0; i<10; i++)
  {
  digitalWrite(LEDPINS[i], state ? HIGH : LOW);
  }
  return true;

}

void setup()
{
  unsigned long startAttempt = millis();

    // Initialize your setup code here
    Serial.begin(115200);

    for (int i=0; i < 10; i++)
    {
      pinMode(LEDPINS[i], OUTPUT);
    }

    //WiFi connection handler
    Serial.println("Attempting to connect to internet");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
      delay(500);
      yield(); //i have no idea what this does : ) It just works
      Serial.print(".");
    }
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nWiFi connection FAILED successfully");
    } 
    else 
    {
      Serial.println("\nWiFi connected");
      Serial.print("Connected to internet with IP addr: ");
      Serial.println(WiFi.localIP());
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      SinricProSwitch& mySwitch = SinricPro[DEVICE_ID];
      mySwitch.onPowerState(onPowerState);
  
      SinricPro.begin(APP_KEY, APP_SECRET);
      Serial.println("SinricPro initialization complete!");
      Serial.println("You can now use voice commands:");
      Serial.println("Try saying: `Hey Google, Turn on the lights`");
      //you will need to setup the google home app to use google assistant!
      //Open google home > Link app or service (works with google) > Search SinricPro > Sign In> Add Light
    } 
    else 
    {
      Serial.println("Skipping SinricPro init due to WiFi failure");
    }
  
  // Start SinricPro
    //SinricPro.begin(APP_KEY, APP_SECRET);

}

void loop() 
{
  SinricPro.handle(); // Handle SinricPro commands
  delay(10);
}