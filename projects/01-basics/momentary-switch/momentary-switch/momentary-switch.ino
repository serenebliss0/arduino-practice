
int buttonPin = 2;
int buttonPin2 = 3;
int ledPin = 7;
int ledPin2 = 8;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  int buttonState = digitalRead(buttonPin);
  int buttonState2 = digitalRead(buttonPin2);
  Serial.println(buttonState);

  if (buttonState == 1 && buttonState2 == 1)
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);

  }
}
