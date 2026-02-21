#include <WiFi.h>
#include <Audio.h>
#include <AudioFileSourceICYStream.h>
#include <AudioOutputPWM.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// create objects
Audio audio;
AudioFileSourceICYStream stream;
AudioOutputPWM pwmOutput;

void setup() {
  Serial.begin(115200);
  
  // connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  
  // set PWM pins
  pwmOutput.begin(25); // your speaker pin

  // connect audio to output
  audio.connect(&stream, &pwmOutput);
  
  // example stream from PC / phone cast server
  stream.open("http://192.168.1.100:8000/stream.mp3"); 
}

void loop() {
  audio.loop();
}
