/*
 * Project Name: RGB LED color mixer
 * Description: Uses 3 buttons representing Red, Green, and Blue to mix colors
 * Author: Semire Ajayi
 * Date: 11-DEC-2025
 * 
 * Hardware:
 * - Arduino board: UNO Rev 3
 * - 1 RGB LED
 * - 6 220-ohm resistors (one for each led and button)
 * - 1 220-ohm resistor (if using a 9v battery)
 * - 3 momentary switches
 * - 1 9v battery (optional)
 * 
 * Connections:
 * - Red LED -> Pin 9
 * - Green LED -> Pin 10
 * - Blue LED -> Pin 11
 * - Red Button -> Pin 2
 * - Green Button -> Pin 3
 * - Blue Button -> Pin 12
 */


//define each pin to use throughout the program
#define REDLED 9
#define GREENLED 10
#define BLUELED 11
#define  REDBUTTON 2
#define GREENBUTTON 3
#define BLUEBUTTON 12

//the default delay time
#define DELAY 200

void setup()
{
  Serial.begin(9600);
  //change each led pin to output
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);

  //change each button pin to input
  pinMode(REDBUTTON, INPUT);
  pinMode(GREENBUTTON, INPUT);
  pinMode(BLUEBUTTON, INPUT);
}

//create a toggle function to avoid repetitively doing this
void ToggleLED(int whichPin)
{
  digitalWrite(whichPin, HIGH);
  delay(DELAY);
  digitalWrite(whichPin, LOW);
}

//do the same for buttons
int buttonToggle(int whichButton)
{
  int buttonState = digitalRead(whichButton);
  delay(100);
  return buttonState;
}

void loop()
{

  //create a variable to store the status of each button
  int redStatus = buttonToggle(REDBUTTON);
  int greenStatus = buttonToggle(GREENBUTTON);
  int blueStatus = buttonToggle(BLUEBUTTON);

  if (redStatus == 1)
  {
    ToggleLED(REDLED);
  }

  if (greenStatus == 1)
  {
    ToggleLED(GREENLED);
  }

  if (blueStatus == 1)
  {
    ToggleLED(BLUELED);
  }

}