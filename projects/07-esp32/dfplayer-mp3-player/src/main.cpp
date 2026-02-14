#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3(10, 11); // RX, TX
DFRobotDFPlayerMini player;

void setup() {
  mp3.begin(9600);
  Serial.begin(9600);

  if (!player.begin(mp3)) 
  {
    Serial.println("DFPlayer not found 😭");
    while(true);
  }

  player.volume(20);   // 0–30
  player.play(1);      // plays 0001.mp3
}

void loop() {
}
