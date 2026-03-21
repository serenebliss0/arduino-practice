#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_PIN 23

//define button pins
#define BUTTON_ONE 18
#define BUTTON_TWO 19
#define BUTTON_THREE 21
#define BUTTON_FOUR 22

void sendButtonOne();
void sendButtonTwo();
void sendButtonThree();
void sendButtonFour();

IRsend irsend(IR_PIN);

void setup() 
{
  irsend.begin();
  pinMode(BUTTON_ONE, INPUT_PULLUP);
  pinMode(BUTTON_TWO, INPUT_PULLUP);
  pinMode(BUTTON_THREE, INPUT_PULLUP);
  pinMode(BUTTON_FOUR, INPUT_PULLUP);

  pinMode(IR_PIN, OUTPUT);

  Serial.begin(115200);

}

void loop() 
{
  //irsend.sendNEC(0xADA, 32);
  //delay(1000);

  //detect button presses

  int buttonStateOne = digitalRead(BUTTON_ONE);
  int buttonStateTwo = digitalRead(BUTTON_TWO);
  int buttonStateThree = digitalRead(BUTTON_THREE);
  int buttonStateFour = digitalRead(BUTTON_FOUR);

  if (buttonStateOne == 0)
  {
    sendButtonOne();
    Serial.println("Button ichi pressed");
  }
  if (buttonStateTwo == 0)
  {
    sendButtonTwo();
    Serial.println("Button ni pressed");

  }
  if (buttonStateThree == 0)
  {
    sendButtonThree();
    Serial.println("Button san pressed");

  }
  if (buttonStateFour == 0)
  {
    sendButtonFour();
    Serial.println("Button shi pressed");

  }

}

void sendButtonOne()
{
  irsend.sendNEC(0xADB, 32);
  delay(200);
}

void sendButtonTwo()
{
  irsend.sendNEC(0xADB, 32);
  delay(200);
}

void sendButtonThree()
{
  irsend.sendNEC(0xADC, 32);
  delay(200);
}

void sendButtonFour()
{
  irsend.sendNEC(0xADD, 32);
  delay(200);
}