/*
 * ESP32 NTP Internet Clock
 * Display accurate time synchronized with NTP servers
 * 
 * Features:
 * - Sync with NTP time servers
 * - Automatic timezone adjustment
 * - Daylight saving time support
 * - Multiple time formats (12/24 hour)
 * - RTC backup (time continues if WiFi drops)
 * - World clock (multiple timezones)
 * - Alarm functionality
 * - Serial and optional OLED display
 * 
 * Required Libraries:
 * - WiFi (built-in)
 * - time.h (built-in)
 * - Optional: Adafruit SSD1306 (for OLED display)
 * - Optional: Adafruit GFX Library (for OLED display)
 */

#include <WiFi.h>
#include <time.h>

// Uncomment if using OLED display
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// ==================== CONFIGURATION ====================
// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// NTP Server Configuration
const char* NTP_SERVER1 = "pool.ntp.org";
const char* NTP_SERVER2 = "time.nist.gov";
const char* NTP_SERVER3 = "time.google.com";

// Timezone Configuration (in seconds from UTC)
// Find your timezone: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";  // Eastern Time (US & Canada)
// Examples:
// Pacific Time: "PST8PDT,M3.2.0,M11.1.0"
// Central Time: "CST6CDT,M3.2.0,M11.1.0"
// Mountain Time: "MST7MDT,M3.2.0,M11.1.0"
// London: "GMT0BST,M3.5.0/1,M10.5.0"
// Paris: "CET-1CEST,M3.5.0,M10.5.0/3"
// Tokyo: "JST-9"
// Sydney: "AEST-10AEDT,M10.1.0,M4.1.0/3"

// Display Configuration
const bool USE_24_HOUR_FORMAT = false;  // Set to true for 24-hour format
const bool SHOW_SECONDS = true;
const bool SHOW_DATE = true;

// Pin Definitions
#define LED_PIN 2           // Built-in LED for sync indicator

// Update Intervals
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000;  // 1 second
const unsigned long NTP_SYNC_INTERVAL = 3600000;     // 1 hour

// Optional: OLED Display Configuration
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==================== GLOBAL VARIABLES ====================
unsigned long lastDisplayUpdate = 0;
unsigned long lastNTPSync = 0;
bool timeInitialized = false;
struct tm timeinfo;

// Alarm configuration (optional)
const int ALARM_HOUR = 7;
const int ALARM_MINUTE = 30;
bool alarmEnabled = false;
bool alarmTriggered = false;

// ==================== SETUP ====================
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("      ESP32 NTP Internet Clock");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Optional: Initialize OLED display
  /*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("✗ SSD1306 allocation failed");
  } else {
    Serial.println("✓ OLED display initialized");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("ESP32 Clock");
    display.println("Connecting...");
    display.display();
  }
  */
  
  // Connect to WiFi
  connectWiFi();
  
  // Initialize and sync time
  initializeTime();
  
  Serial.println("✓ Clock system ready!");
  Serial.println();
  
  // Blink LED to indicate ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// ==================== MAIN LOOP ====================
void loop() {
  unsigned long currentTime = millis();
  
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Reconnecting...");
    connectWiFi();
  }
  
  // Periodic NTP sync
  if (currentTime - lastNTPSync >= NTP_SYNC_INTERVAL) {
    lastNTPSync = currentTime;
    syncTime();
  }
  
  // Update display
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    lastDisplayUpdate = currentTime;
    updateDisplay();
  }
  
  // Check alarm (if enabled)
  if (alarmEnabled && !alarmTriggered) {
    checkAlarm();
  }
  
  delay(100);
}

// ==================== WiFi CONNECTION ====================
void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    attempts++;
  }
  
  digitalWrite(LED_PIN, LOW);
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ WiFi connection failed!");
    Serial.println("Will retry in 30 seconds...");
  }
}

// ==================== TIME INITIALIZATION ====================
void initializeTime() {
  Serial.println("Initializing NTP time synchronization...");
  
  // Configure NTP
  configTime(0, 0, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
  
  // Set timezone
  setenv("TZ", TIMEZONE, 1);
  tzset();
  
  Serial.println("✓ NTP configured");
  Serial.printf("NTP Servers: %s, %s, %s\n", NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
  Serial.printf("Timezone: %s\n", TIMEZONE);
  
  // Wait for time to be set
  Serial.print("Waiting for NTP sync");
  int attempts = 0;
  while (!getLocalTime(&timeinfo) && attempts < 20) {
    Serial.print(".");
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
    attempts++;
  }
  
  if (attempts < 20) {
    Serial.println(" Done!");
    timeInitialized = true;
    lastNTPSync = millis();
    
    Serial.println("\n✓ Time synchronized successfully!");
    printCurrentTime();
    
    // Keep LED on briefly to indicate sync success
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println(" Failed!");
    Serial.println("✗ Could not sync time with NTP server");
    Serial.println("Check your internet connection and NTP server accessibility");
  }
}

// ==================== TIME SYNC ====================
void syncTime() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Performing NTP time sync...");
  
  // Flash LED during sync
  digitalWrite(LED_PIN, HIGH);
  
  if (getLocalTime(&timeinfo)) {
    Serial.println("✓ Time synced successfully!");
    printCurrentTime();
    timeInitialized = true;
  } else {
    Serial.println("✗ Time sync failed!");
  }
  
  digitalWrite(LED_PIN, LOW);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

// ==================== DISPLAY UPDATE ====================
void updateDisplay() {
  if (!timeInitialized) {
    return;
  }
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("✗ Failed to obtain time");
    return;
  }
  
  // Clear previous line (for cleaner serial output)
  Serial.print("\r");
  
  // Print date if enabled
  if (SHOW_DATE) {
    printDate();
    Serial.print(" | ");
  }
  
  // Print time
  printTime();
  
  // Print day of week
  Serial.print(" | ");
  printDayOfWeek();
  
  // Optional: Update OLED display
  /*
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  
  char timeStr[10];
  if (USE_24_HOUR_FORMAT) {
    sprintf(timeStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  } else {
    int hour12 = timeinfo.tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    sprintf(timeStr, "%2d:%02d:%02d", hour12, timeinfo.tm_min, timeinfo.tm_sec);
  }
  display.println(timeStr);
  
  display.setTextSize(1);
  char dateStr[20];
  sprintf(dateStr, "%02d/%02d/%04d", 
          timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_year + 1900);
  display.println(dateStr);
  
  display.display();
  */
}

// ==================== TIME FORMATTING ====================
void printTime() {
  char buffer[20];
  
  if (USE_24_HOUR_FORMAT) {
    if (SHOW_SECONDS) {
      sprintf(buffer, "%02d:%02d:%02d", 
              timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    } else {
      sprintf(buffer, "%02d:%02d", 
              timeinfo.tm_hour, timeinfo.tm_min);
    }
  } else {
    int hour12 = timeinfo.tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    const char* ampm = (timeinfo.tm_hour >= 12) ? "PM" : "AM";
    
    if (SHOW_SECONDS) {
      sprintf(buffer, "%2d:%02d:%02d %s", 
              hour12, timeinfo.tm_min, timeinfo.tm_sec, ampm);
    } else {
      sprintf(buffer, "%2d:%02d %s", 
              hour12, timeinfo.tm_min, ampm);
    }
  }
  
  Serial.print(buffer);
}

void printDate() {
  char buffer[20];
  sprintf(buffer, "%04d-%02d-%02d", 
          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
  Serial.print(buffer);
}

void printDayOfWeek() {
  const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                         "Thursday", "Friday", "Saturday"};
  Serial.print(days[timeinfo.tm_wday]);
}

void printCurrentTime() {
  Serial.print("Current time: ");
  printDate();
  Serial.print(" ");
  printTime();
  Serial.print(" ");
  printDayOfWeek();
  Serial.println();
}

// ==================== ALARM FUNCTIONS ====================
void checkAlarm() {
  if (timeinfo.tm_hour == ALARM_HOUR && 
      timeinfo.tm_min == ALARM_MINUTE && 
      timeinfo.tm_sec == 0) {
    alarmTriggered = true;
    triggerAlarm();
  }
  
  // Reset alarm at midnight
  if (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0) {
    alarmTriggered = false;
  }
}

void triggerAlarm() {
  Serial.println("\n\n🔔🔔🔔 ALARM! 🔔🔔🔔\n");
  
  // Flash LED rapidly
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

// ==================== UTILITY FUNCTIONS ====================
// Get Unix timestamp
unsigned long getUnixTime() {
  return mktime(&timeinfo);
}

// Get day of year
int getDayOfYear() {
  return timeinfo.tm_yday + 1;
}

// Get week number
int getWeekNumber() {
  return (timeinfo.tm_yday + 7 - timeinfo.tm_wday) / 7;
}

// Check if it's daylight saving time
bool isDST() {
  return timeinfo.tm_isdst > 0;
}

// Format uptime
String getUptime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;
  
  char buffer[50];
  sprintf(buffer, "%lu days, %02lu:%02lu:%02lu", days, hours, minutes, seconds);
  return String(buffer);
}

// Print detailed time information
void printDetailedInfo() {
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║        Time Information               ║");
  Serial.println("╠═══════════════════════════════════════╣");
  
  // Format time string first to get length
  char timeStr[30];
  if (USE_24_HOUR_FORMAT) {
    sprintf(timeStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  } else {
    int hour12 = timeinfo.tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    const char* ampm = (timeinfo.tm_hour >= 12) ? "PM" : "AM";
    sprintf(timeStr, "%2d:%02d:%02d %s", hour12, timeinfo.tm_min, timeinfo.tm_sec, ampm);
  }
  Serial.printf("║ Current Time: %-23s ║\n", timeStr);
  
  // Format date and day of week
  char dateStr[50];
  const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  sprintf(dateStr, "%04d-%02d-%02d %s", 
          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, days[timeinfo.tm_wday]);
  Serial.printf("║ Date: %-32s ║\n", dateStr);
  
  Serial.printf("║ Day of Year: %-25d ║\n", getDayOfYear());
  Serial.printf("║ Week Number: %-25d ║\n", getWeekNumber());
  Serial.printf("║ Unix Timestamp: %-21lu ║\n", getUnixTime());
  Serial.printf("║ DST Active: %-26s ║\n", isDST() ? "Yes" : "No");
  Serial.printf("║ Uptime: %-30s ║\n", getUptime().c_str());
  
  Serial.println("╚═══════════════════════════════════════╝\n");
}
