int ledPin = 9; // connect LED + resistor here

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Fade in
  for(int brightness = 0; brightness <= 255; brightness++){
    analogWrite(ledPin, brightness);
    delay(10); // change speed here
  }
  
  // Fade out
  for(int brightness = 255; brightness >= 0; brightness--){
    analogWrite(ledPin, brightness);
    delay(10);
  }
}
