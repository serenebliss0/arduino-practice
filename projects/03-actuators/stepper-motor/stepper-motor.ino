#include <Stepper.h>

const int stepsPerRevolution = 2048; // for 28BYJ-48

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(10); // RPM
}

void loop() {
  myStepper.step(stepsPerRevolution);   // one full turn
  delay(1000);
  
  myStepper.step(-stepsPerRevolution);  // turn back
  delay(1000);
}
