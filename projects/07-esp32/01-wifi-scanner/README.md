# Project 1: WiFi Scanner 📡

A network scanning tool that discovers and displays all available WiFi networks with detailed information including signal strength, channel, and encryption type.

## 🎯 Project Overview

**Difficulty**: Beginner  
**Time to Complete**: 30 minutes  
**Real-World Applications**:
- Network troubleshooting and diagnostics
- WiFi signal strength mapping
- Channel interference detection
- Network security auditing
- Optimal router placement

## 📦 Components Needed

### Hardware
- ESP32 Development Board (any model)
- USB cable (Micro USB or USB-C)
- Computer with Arduino IDE

### Software
- Arduino IDE with ESP32 board support
- WiFi library (built-in with ESP32 core)

**Total Cost**: ~$8-12 (just the ESP32!)

## 🔌 Wiring

No external wiring needed! This project uses only the ESP32's built-in WiFi capabilities.

```
ESP32 Board
    ├── USB Port ──→ Computer (for power and programming)
    └── Built-in WiFi antenna (internal)
```

## 📋 Circuit Diagram

```
┌─────────────────────┐
│                     │
│      ESP32          │
│   DevKit Board      │
│                     │
│  [Built-in WiFi]    │
│                     │
│     [USB Port]      │
│         │           │
└─────────┼───────────┘
          │
          ▼
     Computer
```

No breadboard or external components required!

## 💻 Code

### Main Sketch: `01-wifi-scanner.ino`

```cpp
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
```

### Enhanced Version with MAC Addresses

Create a file `01-wifi-scanner-advanced.ino`:

```cpp
/*
 * ESP32 WiFi Scanner - Advanced Version
 * Includes MAC addresses, hidden networks detection, and JSON export option
 */

#include <WiFi.h>

const int SCAN_INTERVAL = 5000;
unsigned long lastScanTime = 0;
bool jsonOutput = false;  // Set to true for JSON format output

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  if (!jsonOutput) {
    Serial.println("\n\n");
    Serial.println("═══════════════════════════════════════");
    Serial.println("  ESP32 WiFi Scanner - Advanced Mode");
    Serial.println("═══════════════════════════════════════");
    Serial.println();
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastScanTime >= SCAN_INTERVAL) {
    lastScanTime = currentTime;
    
    if (jsonOutput) {
      scanNetworksJSON();
    } else {
      scanNetworksAdvanced();
    }
  }
}

void scanNetworksAdvanced() {
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Scanning...");
  
  int numNetworks = WiFi.scanNetworks(false, true);  // async=false, show_hidden=true
  
  Serial.printf("Found %d network(s)\n\n", numNetworks);
  
  if (numNetworks > 0) {
    Serial.println("┌────┬─────────────────────────┬──────────────────┬─────────┬────┬──────────────┐");
    Serial.println("│ #  │ SSID                    │ MAC Address      │ Signal  │ Ch │ Encryption   │");
    Serial.println("├────┼─────────────────────────┼──────────────────┼─────────┼────┼──────────────┤");
    
    for (int i = 0; i < numNetworks; i++) {
      String ssid = WiFi.SSID(i);
      String bssid = WiFi.BSSIDstr(i);
      int32_t rssi = WiFi.RSSI(i);
      uint8_t encType = WiFi.encryptionType(i);
      int32_t channel = WiFi.channel(i);
      
      // Handle hidden networks
      if (ssid.length() == 0) {
        ssid = "<Hidden Network>";
      }
      
      // Truncate long SSIDs
      if (ssid.length() > 23) {
        ssid = ssid.substring(0, 20) + "...";
      }
      
      String signalBars = getSignalBars(rssi);
      String encryption = getEncryptionType(encType);
      
      Serial.printf("│ %-2d │ %-23s │ %-17s │ %s %3ddBm │ %-2d │ %-12s │\n",
                    i + 1,
                    ssid.c_str(),
                    bssid.c_str(),
                    signalBars.c_str(),
                    rssi,
                    channel,
                    encryption.c_str());
    }
    
    Serial.println("└────┴─────────────────────────┴──────────────────┴─────────┴────┴──────────────┘");
    
    // Statistics
    printStatistics(numNetworks);
  }
  
  Serial.println();
  WiFi.scanDelete();
  Serial.printf("Next scan in %d seconds...\n\n", SCAN_INTERVAL / 1000);
}

void scanNetworksJSON() {
  int numNetworks = WiFi.scanNetworks(false, true);
  
  Serial.println("{");
  Serial.printf("  \"timestamp\": %lu,\n", millis());
  Serial.printf("  \"count\": %d,\n", numNetworks);
  Serial.println("  \"networks\": [");
  
  for (int i = 0; i < numNetworks; i++) {
    Serial.println("    {");
    Serial.printf("      \"index\": %d,\n", i + 1);
    Serial.printf("      \"ssid\": \"%s\",\n", WiFi.SSID(i).c_str());
    Serial.printf("      \"bssid\": \"%s\",\n", WiFi.BSSIDstr(i).c_str());
    Serial.printf("      \"rssi\": %d,\n", WiFi.RSSI(i));
    Serial.printf("      \"channel\": %d,\n", WiFi.channel(i));
    Serial.printf("      \"encryption\": \"%s\"\n", getEncryptionType(WiFi.encryptionType(i)).c_str());
    Serial.print("    }");
    if (i < numNetworks - 1) Serial.println(",");
    else Serial.println();
  }
  
  Serial.println("  ]");
  Serial.println("}\n");
  
  WiFi.scanDelete();
}

void printStatistics(int numNetworks) {
  int openNetworks = 0;
  int secureNetworks = 0;
  int channel2_4GHz[14] = {0};
  
  for (int i = 0; i < numNetworks; i++) {
    if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
      openNetworks++;
    } else {
      secureNetworks++;
    }
    
    int ch = WiFi.channel(i);
    if (ch >= 1 && ch <= 14) {
      channel2_4GHz[ch - 1]++;
    }
  }
  
  Serial.println("\n📊 Statistics:");
  Serial.printf("   Open Networks: %d\n", openNetworks);
  Serial.printf("   Secured Networks: %d\n", secureNetworks);
  
  Serial.println("\n   Channel Usage:");
  for (int i = 0; i < 14; i++) {
    if (channel2_4GHz[i] > 0) {
      Serial.printf("   Channel %2d: ", i + 1);
      for (int j = 0; j < channel2_4GHz[i]; j++) {
        Serial.print("█");
      }
      Serial.printf(" (%d)\n", channel2_4GHz[i]);
    }
  }
}

String getSignalBars(int32_t rssi) {
  if (rssi >= -50) return "▂▄▆█";
  else if (rssi >= -60) return "▂▄▆_";
  else if (rssi >= -70) return "▂▄__";
  else if (rssi >= -80) return "▂___";
  else return "____";
}

String getEncryptionType(uint8_t encType) {
  switch (encType) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-EAP";
    default: return "Unknown";
  }
}
```

## 🚀 How to Use

### 1. Setup
1. Connect ESP32 to computer via USB
2. Open Arduino IDE
3. Select ESP32 board (Tools → Board → ESP32 Dev Module)
4. Select correct port (Tools → Port)

### 2. Upload
1. Copy code to Arduino IDE
2. Click Verify (✓) to compile
3. Click Upload (→) to upload
4. Open Serial Monitor (Tools → Serial Monitor)
5. Set baud rate to **115200**

### 3. View Results
- Networks will be scanned automatically every 5 seconds
- Information displayed includes:
  - Network name (SSID)
  - Signal strength (RSSI in dBm)
  - Channel number
  - Encryption type
  - MAC address (advanced version)

### 4. Interpreting Results

**Signal Strength (RSSI)**:
- **-30 to -50 dBm**: Excellent - maximum performance
- **-50 to -60 dBm**: Good - reliable performance
- **-60 to -70 dBm**: Fair - may have issues
- **-70 to -80 dBm**: Weak - poor performance
- **Below -80 dBm**: Very weak - unreliable

**Encryption Types**:
- **Open**: No password (unsecure!)
- **WEP**: Outdated, easily cracked
- **WPA**: Old standard, less secure
- **WPA2**: Current standard, secure
- **WPA3**: Newest standard, most secure

## 📊 Example Output

```
═══════════════════════════════════════
     ESP32 WiFi Network Scanner
═══════════════════════════════════════

WiFi Scanner Ready!
Scanning for networks...

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Starting WiFi scan...
Scan complete!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✓ Found 12 network(s)

┌────┬─────────────────────────────────┬─────────┬────────┬──────────────┐
│ #  │ SSID                            │ Signal  │  Ch    │ Encryption   │
├────┼─────────────────────────────────┼─────────┼────────┼──────────────┤
│ 1  │ MyHomeNetwork                   │ ▂▄▆█ -45dBm │ 6      │ WPA2         │
│ 2  │ Neighbor_WiFi                   │ ▂▄▆_ -58dBm │ 11     │ WPA2         │
│ 3  │ CoffeeShop_Guest                │ ▂▄__ -67dBm │ 1      │ Open         │
│ 4  │ Office_5G                       │ ▂___ -78dBm │ 36     │ WPA2         │
└────┴─────────────────────────────────┴─────────┴────────┴──────────────┘

Next scan in 5 seconds...
```

## 🎓 Understanding the Code

### Key Concepts

**WiFi Modes**:
```cpp
WiFi.mode(WIFI_STA);  // Station mode (client)
```
Station mode allows ESP32 to scan for and connect to networks.

**Scanning**:
```cpp
int n = WiFi.scanNetworks();  // Returns number of networks found
```
Scans for all available networks and returns count.

**Getting Network Info**:
```cpp
String ssid = WiFi.SSID(i);           // Network name
int32_t rssi = WiFi.RSSI(i);          // Signal strength
uint8_t enc = WiFi.encryptionType(i); // Security type
int32_t ch = WiFi.channel(i);         // WiFi channel
String mac = WiFi.BSSIDstr(i);        // MAC address
```

**Memory Management**:
```cpp
WiFi.scanDelete();  // Free memory after scan
```
Important to prevent memory leaks!

## 🔧 Customization Ideas

### 1. Filter Networks
```cpp
// Only show networks with good signal
if (rssi >= -70) {
  displayNetwork(i);
}
```

### 2. Sort by Signal Strength
Sort networks before displaying (strongest first).

### 3. Save to SD Card
Log scan results to SD card for historical analysis.

### 4. OLED Display
Show strongest 5 networks on an OLED screen.

### 5. Web Interface
Create web page to display scan results graphically.

### 6. Auto-Connect
Automatically connect to strongest known network.

### 7. Channel Recommendation
Suggest best channel for new router based on congestion.

### 8. Mobile App Integration
Send scan results to phone app via Bluetooth.

## 🐛 Troubleshooting

### No Networks Found
- **Check antenna**: Some ESP32 boards have external antenna connector
- **Try different location**: Move away from interference
- **Check WiFi.mode()**: Ensure STA mode is set
- **Verify 2.4GHz**: ESP32 only supports 2.4GHz, not 5GHz

### Scan Takes Too Long
- Normal scan takes 1-3 seconds
- Reduce scan interval if needed
- Use async scanning for non-blocking operation

### ESP32 Resets During Scan
- **Power issue**: Use better power supply or powered USB hub
- **Add delay**: Add small delay after WiFi.mode()
- **Brownout**: Add capacitor (100µF) near ESP32

### Garbled Serial Output
- **Check baud rate**: Must be 115200
- **EN button**: Press EN/Reset button to restart
- **USB cable**: Try different cable (must support data)

## 📚 Learning Outcomes

After completing this project, you'll understand:
- ✅ How to use ESP32's WiFi capabilities
- ✅ WiFi scanning and network detection
- ✅ Signal strength measurement and interpretation
- ✅ WiFi channels and frequency bands
- ✅ Network security types
- ✅ Serial communication and data formatting
- ✅ Memory management in ESP32

## 🚀 Next Steps

Ready for more? Try these projects:
1. **Web Server** - Control devices from browser
2. **WiFi Connection** - Actually connect to a network
3. **RSSI Signal Mapper** - Create signal strength heat map
4. **Network Monitor** - Track network availability over time

## 📖 References

- [ESP32 WiFi Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [WiFi Channels Explained](https://www.metageek.com/training/resources/wifi-channels.html)
- [RSSI Signal Strength Guide](https://eyesaas.com/wi-fi-signal-strength/)

---

**Project Complete!** 🎉

You've built a useful tool for WiFi network analysis. This foundational knowledge will be essential for all future WiFi-based ESP32 projects!
