
int resistorPin = A0;
int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  float lightValue = analogRead(A0);
  delay(200);
  Serial.println(lightValue);

  if (lightValue == )



  

}
