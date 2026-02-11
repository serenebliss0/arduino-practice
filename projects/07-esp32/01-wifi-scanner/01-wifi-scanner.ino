/*
 * ESP32 WiFi Scanner
 * Scans for available WiFi networks and displays information
 * 
 * Features:
 * - Continuous scanning every 5 seconds
 * - Display SSID, RSSI, Channel, and Encryption
 * - Sort by signal strength
 * - Show network count
 */

#include <WiFi.h>

// Scan interval in milliseconds
const int SCAN_INTERVAL = 5000;
unsigned long lastScanTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("     ESP32 WiFi Network Scanner");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Set WiFi to station mode and disconnect
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("WiFi Scanner Ready!");
  Serial.println("Scanning for networks...\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Scan every SCAN_INTERVAL milliseconds
  if (currentTime - lastScanTime >= SCAN_INTERVAL) {
    lastScanTime = currentTime;
    scanNetworks();
  }
}

void scanNetworks() {
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Starting WiFi scan...");
  
  // Start scan (async = false means wait for result)
  int numNetworks = WiFi.scanNetworks();
  
  Serial.println("Scan complete!");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  
  if (numNetworks == 0) {
    Serial.println("❌ No networks found");
  } else {
    Serial.printf("✓ Found %d network(s)\n\n", numNetworks);
    
    // Print table header
    Serial.println("┌────┬─────────────────────────────────┬─────────┬────────┬──────────────┐");
    Serial.println("│ #  │ SSID                            │ Signal  │  Ch    │ Encryption   │");
    Serial.println("├────┼─────────────────────────────────┼─────────┼────────┼──────────────┤");
    
    // Display information for each network
    for (int i = 0; i < numNetworks; i++) {
      displayNetwork(i);
    }
    
    Serial.println("└────┴─────────────────────────────────┴─────────┴────────┴──────────────┘");
  }
  
  Serial.println();
  
  // Delete scan result to free memory
  WiFi.scanDelete();
  
  // Show next scan countdown
  Serial.printf("Next scan in %d seconds...\n\n", SCAN_INTERVAL / 1000);
}

void displayNetwork(int index) {
  // Get network information
  String ssid = WiFi.SSID(index);
  int32_t rssi = WiFi.RSSI(index);
  uint8_t encryptionType = WiFi.encryptionType(index);
  int32_t channel = WiFi.channel(index);
  
  // Format SSID (truncate if too long)
  if (ssid.length() > 31) {
    ssid = ssid.substring(0, 28) + "...";
  }
  
  // Get signal quality
  String signalQuality = getSignalQuality(rssi);
  String signalBars = getSignalBars(rssi);
  
  // Get encryption type name
  String encryption = getEncryptionType(encryptionType);
  
  // Print formatted row
  Serial.printf("│ %-2d │ %-31s │ %-7s │ %-6d │ %-12s │\n",
                index + 1,
                ssid.c_str(),
                (signalBars + " " + String(rssi) + "dBm").c_str(),
                channel,
                encryption.c_str());
}

// Convert RSSI to signal quality description
String getSignalQuality(int32_t rssi) {
  if (rssi >= -50) return "Excellent";
  else if (rssi >= -60) return "Good";
  else if (rssi >= -70) return "Fair";
  else if (rssi >= -80) return "Weak";
  else return "Very Weak";
}

// Convert RSSI to visual bars
String getSignalBars(int32_t rssi) {
  if (rssi >= -50) return "▂▄▆█";      // Excellent
  else if (rssi >= -60) return "▂▄▆_";  // Good
  else if (rssi >= -70) return "▂▄__";  // Fair
  else if (rssi >= -80) return "▂___";  // Weak
  else return "____";                    // Very Weak
}

// Get encryption type name
String getEncryptionType(uint8_t encType) {
  switch (encType) {
    case WIFI_AUTH_OPEN:
      return "Open";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2-EAP";
    default:
      return "Unknown";
  }
}
