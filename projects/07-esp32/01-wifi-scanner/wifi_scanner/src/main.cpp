#include<Arduino.h>
#include<WiFi.h>
#include <Wire.h>
#include<LiquidCrystal_I2C.h>

const int SCAN_INTERVAL = 5000;
unsigned long lastScanTime = 0;

LiquidCrystal_I2C lcd(0x3F, 16,2);

void setup()
{
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.autoscroll();


  Serial.println("\n\n");
  Serial.println("-----------------------------------");
  Serial.println("Semire's ESP32 WIFI Scanner!");
  Serial.println("-----------------------------------\n");
  lcd.print("Semire WiFi Scanner");

  //Set wifi to station mode
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

delay(100);

//time to start cooking
Serial.println("Wifi scanner ready to activate! \nScanning for networks...");

}

void displayNetwork(int index)
{

  String ssid = WiFi.SSID(index);

  lcd.clear();  // clear previous display
  lcd.setCursor(0,0);
  lcd.print(WiFi.SSID(index).substring(0,16)); // first line

  lcd.setCursor(0,1);
  lcd.print("RSSI:");
  lcd.print(WiFi.RSSI(index));
  lcd.print("dBm");

}


void scanNetworks()
{
  Serial.println("Starting Scan");

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.println("Scan Complete");

  if (numberOfNetworks == 0)
  {
    Serial.println("No networks found bro!");
  }
  else
  {
    Serial.printf("Found %d network(s)\n\n", numberOfNetworks);

    //print out the table exclusive to serial monitor

    Serial.println("┌────┬─────────────────────────────────┬─────────┬────────┬──────────────┐");
    Serial.println("│ #  │ SSID                            │ Signal  │  Ch    │ Encryption   │");
    Serial.println("├────┼─────────────────────────────────┼─────────┼────────┼──────────────┤");
    
    // Display information for each network
    for (int i = 0; i < numberOfNetworks; i++) {
      displayNetwork(i);
    }
    
    Serial.println("└────┴─────────────────────────────────┴─────────┴────────┴──────────────┘");
  }

  WiFi.scanDelete();

  //countdown to next
  Serial.printf("Next scan in %d seconds...", SCAN_INTERVAL/1000);
  delay(1000);
}


void loop()
{
  unsigned long currentTime = millis();

  if(currentTime - lastScanTime >= SCAN_INTERVAL)
  {
    lastScanTime = currentTime;
    scanNetworks();
  }

}
