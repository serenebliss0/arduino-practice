void setup() {
  
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1);

  if (sensorValue > 100)
  {
    Serial.println("Bruh the music is getting too loud now");
  }
}
