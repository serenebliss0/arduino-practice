#include <IRremote.h>

int receiverPin = 11;
int redPin = 3;
int bluePin = 5;
int greenPin = 13;
int whitePin = 6;

IRrecv irrecv(receiverPin);
decode_results results;

void setup() {
  Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value, HEX);  // show the code

    if (results.value != 0xFFFFFFFF) {
        Serial.println(results.value, HEX);
    }
    
    if (results.value == 0XFF9A65 )  
    {
      digitalWrite(greenPin, HIGH);
    }
    else if (results.value == 0xFFA25D)
    {
      digitalWrite(bluePin, HIGH);
    }
    else if (results.value == 0xFF1AE5)
    {
      digitalWrite(redPin, HIGH);
    }
    else if (results.value == 0xFF22DD)
    {
      digitalWrite(whitePin, HIGH);
    }
    else if (results.value != 0xFFFFFFFF)
    {
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      digitalWrite(redPin, LOW);
      digitalWrite(whitePin, LOW);
    }

    irrecv.resume(); // ready for next
    delay(100);
  }
}
