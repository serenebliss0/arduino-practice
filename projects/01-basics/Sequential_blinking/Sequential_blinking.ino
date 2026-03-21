

int leds[] = {13,12,11,10,9,8,7,6,5,4,3};
int count = 11; //i believe these two are global variables now

void setup()
{
  for (int i = 0; i < count; i++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (int i =0; i < count; i++)
  {
    digitalWrite(leds[i], HIGH);
    delay(100);           //loop through each led and change state to high
  }


  for (int j = 0; j < count; j++)
  {
    digitalWrite(leds[j], LOW); //loop through each led but now change state to low
    delay(100);
  }
}