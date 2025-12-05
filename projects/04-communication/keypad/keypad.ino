#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

// Layout matches the 4x4 keypad
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2,3,4,5}; // connect to keypad row pins
byte colPins[COLS] = {6,7,8,9}; // connect to keypad column pins

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  
  if (key) { // key pressed
    Serial.println(key);
  }
}
