
int leds[] = {2,3,4,5,6,7};

int len = sizeof(leds) / sizeof(leds[0]);

void setup()
{
  for (int i = 0; i < len; i ++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (int i = 0; i < len; i ++)
  {
    digitalWrite(leds[i], HIGH);
    delay(100);
  }

  for (int j = 0; j < len; j++)
  {
    digitalWrite(leds[j], LOW);
    delay(100);
  }
}