# Project 10: BLE Beacon 📡

Transform your ESP32 into an iBeacon transmitter! Broadcast location data using Apple's BLE beacon protocol for proximity marketing, indoor navigation, attendance tracking, and asset management.

## 🎯 Project Overview

**Difficulty**: Beginner to Intermediate  
**Time to Complete**: 30-60 minutes  
**Real-World Applications**:
- Proximity marketing and retail analytics
- Indoor positioning and navigation
- Museum and exhibition guides
- Attendance and event check-in systems
- Asset tracking and inventory management
- Contact tracing and social distancing
- Smart home automation triggers
- Lost item tracking (like AirTag)

This project implements Apple's iBeacon protocol, turning your ESP32 into a BLE beacon that broadcasts a unique identifier. Any smartphone or BLE-enabled device can detect the beacon, measure distance, and trigger actions based on proximity.

## 📦 Components Needed

### Hardware
- ESP32 Development Board - $8-12
- USB cable for programming - $3
- Optional: Battery pack (18650, LiPo, or USB power bank) for portable operation - $5-15

### Software
- Arduino IDE with ESP32 board support
- ESP32 BLE Arduino library (included with ESP32 core)
- Beacon scanner app for testing:
  - **iOS**: Locate Beacon, BeaconScanner, Nordic nRF Connect
  - **Android**: Beacon Scanner, nRF Connect, Beacon Simulator

**Total Cost**: ~$8-30 (depending on power solution)

## 🔌 Wiring Diagram

### The Beauty of This Project

**No wiring required!** The ESP32's built-in Bluetooth Low Energy radio handles everything. Just connect via USB or battery.

```
                ┌─────────────────────┐
                │                     │
                │                     │
                │      ESP32          │
                │   Development       │
                │       Board         │
                │                     │
                │   [Built-in BLE]    │
                │        📡           │
                │                     │
                └─────────────────────┘
                          │
                          │ USB Cable or Battery
                          │
                      Power Source
```

### Power Options

**Option 1: USB Power** (Development)
```
Computer USB ──→ ESP32 USB Port
```

**Option 2: Battery Pack** (Portable)
```
Battery Pack ──→ ESP32 VIN/5V Pin
```

**Option 3: LiPo Battery** (Compact)
```
3.7V LiPo ──→ ESP32 Battery Connector (if available)
             or
3.7V LiPo ──→ 3V3 Pin (with protection circuit)
```

**Important Notes**:
- **Power Consumption**: BLE beacons use very little power (10-50mA)
- **Battery Life**: Can run for days or weeks on a single battery
- **Range**: Typical 10-50 meters, up to 100m in open space
- **Transmission Power**: Adjustable from -12dBm to +3dBm
- **Update Rate**: Configurable advertising interval (100ms - 10s)

## 💻 Code

See the complete Arduino sketch in `10-ble-beacon.ino`

## 📋 How It Works

### iBeacon Protocol Overview

Apple's iBeacon is a simple but powerful BLE advertising protocol:

```
iBeacon Advertisement Packet Structure
┌────────────────────────────────────────┐
│ Apple Company Identifier: 0x004C       │ (2 bytes)
├────────────────────────────────────────┤
│ iBeacon Type: 0x02                     │ (1 byte)
├────────────────────────────────────────┤
│ Data Length: 0x15 (21 bytes)           │ (1 byte)
├────────────────────────────────────────┤
│ Proximity UUID                         │ (16 bytes)
│ Example: E2C56DB5-DFFB-48D2-B060-...  │
├────────────────────────────────────────┤
│ Major                                  │ (2 bytes)
│ Example: 0x0001 = 1                    │
├────────────────────────────────────────┤
│ Minor                                  │ (2 bytes)
│ Example: 0x0064 = 100                  │
├────────────────────────────────────────┤
│ TX Power (at 1m)                       │ (1 byte)
│ Example: 0xC5 = -59 dBm                │
└────────────────────────────────────────┘
```

### iBeacon Identifiers Explained

**1. Proximity UUID** (16 bytes)
- Universally Unique Identifier for your organization/application
- All beacons in your deployment typically share the same UUID
- Example: A retail chain uses one UUID for all stores
- Generated once and reused across beacon network

**2. Major Value** (0-65535)
- Identifies a subset of beacons within UUID group
- Example: Each store location gets a unique Major number
  - Store #1: Major = 1
  - Store #2: Major = 2
- Allows grouping beacons by location or function

**3. Minor Value** (0-65535)
- Identifies individual beacons within a Major group
- Example: Different departments in Store #1
  - Electronics: Minor = 1
  - Clothing: Minor = 2
  - Food: Minor = 3
- Enables precise location within a space

**4. TX Power** (-128 to 0 dBm)
- Reference RSSI value at 1 meter distance
- Used for distance estimation
- Must be calibrated for each beacon
- Typical value: -59 dBm

### Hierarchy Example

```
Company: "TechMart Stores"
UUID: E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
│
├─ Major 1: San Francisco Store
│  ├─ Minor 1: Entrance
│  ├─ Minor 2: Electronics
│  ├─ Minor 3: Clothing
│  └─ Minor 4: Checkout
│
├─ Major 2: Los Angeles Store
│  ├─ Minor 1: Entrance
│  ├─ Minor 2: Electronics
│  └─ Minor 3: Checkout
│
└─ Major 3: Seattle Store
   ├─ Minor 1: Entrance
   └─ Minor 2: Electronics
```

### Distance Estimation

Beacon receivers estimate distance using RSSI (Received Signal Strength Indicator):

```
Distance = 10 ^ ((TX Power - RSSI) / (10 * N))

Where:
- TX Power = Calibrated signal strength at 1m
- RSSI = Measured signal strength
- N = Path loss exponent (typically 2-4)
```

**Proximity Zones**:
- **Immediate**: < 0.5 meters (very close)
- **Near**: 0.5 - 3 meters (nearby)
- **Far**: 3 - 10+ meters (distant)
- **Unknown**: Signal too weak or unstable

**Factors Affecting Accuracy**:
- Physical obstacles (walls, furniture, people)
- Antenna orientation
- Interference from other devices
- Temperature and humidity
- Multipath reflections
- Device manufacturing variations

### BLE Advertising Process

```
Step 1: Initialize BLE Stack
        └─→ Configure ESP32 BLE parameters

Step 2: Create Advertising Data
        └─→ Build iBeacon packet

Step 3: Start Broadcasting
        └─→ Transmit at regular intervals
        
Step 4: Continuous Operation
        └─→ Loop: Advertise → Wait → Repeat

Smartphone detects beacon
        ↓
App measures RSSI
        ↓
Calculate distance
        ↓
Trigger action based on proximity
```

### Power Optimization

BLE beacons are incredibly power-efficient:

**Advertising Intervals**:
- **100ms**: High responsiveness, higher power (20-40mA)
- **1000ms**: Balanced (10-20mA) - **Default**
- **10000ms**: Maximum battery life (5-10mA)

**Transmission Power Levels**:
- **+3 dBm**: Maximum range (~100m outdoor)
- **0 dBm**: Standard range (~50m)
- **-12 dBm**: Low power, short range (~10m)

**Battery Life Estimates** (2000mAh battery):
```
Interval  TX Power  Current  Battery Life
────────────────────────────────────────
100ms     +3 dBm    35mA     ~60 hours
1000ms    0 dBm     15mA     ~130 hours
10000ms   -12 dBm   8mA      ~250 hours
```

## 🚀 Setup Instructions

### 1. Install ESP32 Board Support

If not already installed:
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search "ESP32" and install "esp32 by Espressif Systems"

### 2. Select Board and Port

1. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
2. **Tools → Port → (Select your ESP32's COM/Serial port)**

### 3. Upload the Code

1. Open `10-ble-beacon.ino`
2. Optionally customize UUID, Major, Minor values
3. Click **Upload**
4. Open **Serial Monitor** at 115200 baud

### 4. Install Beacon Scanner App

**For iOS**:
- [Locate Beacon](https://apps.apple.com/app/locate-beacon/id738709014) - Free, simple
- [BeaconScanner](https://apps.apple.com/app/beaconscanner/id1041291738) - Detailed info
- [nRF Connect](https://apps.apple.com/app/nrf-connect-for-mobile/id1054362403) - Professional

**For Android**:
- [Beacon Scanner](https://play.google.com/store/apps/details?id=com.bridou_n.beaconscanner) - Popular choice
- [nRF Connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp) - Professional
- [Beacon Simulator](https://play.google.com/store/apps/details?id=net.alea.beaconsimulator) - Testing tool

### 5. Test Your Beacon

1. **Power on ESP32** and wait for "Beacon advertising..." message
2. **Open scanner app** on your smartphone
3. **Grant permissions** (Location, Bluetooth)
4. **Start scanning** - look for your UUID
5. **Observe data**:
   - UUID matches your configuration
   - Major and Minor values are correct
   - RSSI changes as you move closer/farther
   - Distance estimation updates

### 6. Monitor Serial Output

```
═══════════════════════════════════════
   ESP32 BLE Beacon (iBeacon)
═══════════════════════════════════════

Configuration:
  UUID:  E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
  Major: 1
  Minor: 100
  TX Power: -59 dBm

✓ BLE initialized
✓ Beacon advertising started

Beacon is broadcasting...
Use a beacon scanner app to detect!

Press RESET to restart beacon.
```

## 📊 Example Output

### Serial Monitor

```
═══════════════════════════════════════
   ESP32 BLE Beacon (iBeacon)
═══════════════════════════════════════

Configuration:
  UUID:  E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
  Major: 1
  Minor: 100
  TX Power: -59 dBm

✓ BLE initialized
✓ Beacon advertising started

Beacon is broadcasting...
Use a beacon scanner app to detect!

Advertising Interval: 1000 ms
Transmission Power: 0 dBm
Expected Range: ~50 meters

Battery-saving tip:
Increase advertising interval for longer battery life!
```

### Beacon Scanner App Output

**nRF Connect (iOS/Android)**:
```
Device Name: ESP32_Beacon
Address: 24:0A:C4:XX:XX:XX
RSSI: -58 dBm
Type: iBeacon

iBeacon Data:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UUID:     E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
Major:    1 (0x0001)
Minor:    100 (0x0064)
TX Power: -59 dBm
Distance: ~1.2 meters (Near)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**Locate Beacon (iOS)**:
```
┌─────────────────────────────┐
│     ESP32 Beacon #100       │
├─────────────────────────────┤
│ Proximity: Near             │
│ Distance: ~1.5m             │
│ RSSI: -62 dBm               │
│ Accuracy: ±0.8m             │
├─────────────────────────────┤
│ UUID:                       │
│ E2C56DB5-DFFB-48D2-B060-... │
│ Major: 1                    │
│ Minor: 100                  │
└─────────────────────────────┘
```

### Distance Testing Results

Walk away from beacon and observe RSSI changes:

```
Distance    RSSI      Proximity   Accuracy
─────────────────────────────────────────
0.5m        -50 dBm   Immediate   ±0.2m
1.0m        -59 dBm   Near        ±0.5m
2.0m        -65 dBm   Near        ±0.8m
5.0m        -75 dBm   Far         ±2m
10.0m       -85 dBm   Far         ±5m
20.0m       -95 dBm   Unknown     ±10m
```

**Through obstacles**:
- Wooden door: -10 to -15 dBm signal loss
- Concrete wall: -20 to -30 dBm signal loss
- Metal barrier: -30+ dBm signal loss (may block completely)

## 🎓 Understanding the Code

### iBeacon Data Structure

The core of iBeacon is a 25-byte advertising packet:

```cpp
// iBeacon prefix (Apple's company identifier)
uint8_t beaconData[] = {
    0x02, 0x01, 0x06,     // Flags: General discoverable, BR/EDR not supported
    0x1A, 0xFF,           // Length=26, Type=Manufacturer Specific Data
    0x4C, 0x00,           // Apple Company Identifier (0x004C)
    0x02,                 // iBeacon type
    0x15,                 // iBeacon data length (21 bytes)
    
    // Proximity UUID (16 bytes)
    0xE2, 0xC5, 0x6D, 0xB5,
    0xDF, 0xFB, 0x48, 0xD2,
    0xB0, 0x60, 0xD0, 0xF5,
    0xA7, 0x10, 0x96, 0xE0,
    
    // Major (2 bytes)
    0x00, 0x01,           // Major = 1
    
    // Minor (2 bytes)
    0x00, 0x64,           // Minor = 100
    
    // TX Power (1 byte)
    0xC5                  // TX Power = -59 dBm (0xC5 in two's complement)
};
```

### Customizing Your Beacon

**1. Generate Your Own UUID**:
```cpp
// Use online UUID generator: https://www.uuidgenerator.net/
// Or Linux command: uuidgen
// Or macOS terminal: uuidgen
// Example result: 550e8400-e29b-41d4-a716-446655440000

String uuid = "550E8400-E29B-41D4-A716-446655440000";
```

**2. Set Major/Minor Values**:
```cpp
uint16_t major = 5;      // Your location identifier (0-65535)
uint16_t minor = 250;    // Your beacon identifier (0-65535)
```

**3. Calibrate TX Power**:
```cpp
// Measure RSSI at exactly 1 meter distance
// Use average of 10-20 readings
// Common values: -59, -55, -65
int8_t txPower = -59;
```

### Advertising Configuration

**Interval Settings**:
```cpp
// Fast updates (responsive, higher power)
BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
pAdvertising->setMinInterval(0x20);  // 32 * 0.625ms = 20ms
pAdvertising->setMaxInterval(0xA0);  // 160 * 0.625ms = 100ms

// Balanced (default)
pAdvertising->setMinInterval(0x320); // 800 * 0.625ms = 500ms
pAdvertising->setMaxInterval(0x640); // 1600 * 0.625ms = 1000ms

// Battery saver (long intervals)
pAdvertising->setMinInterval(0x4000); // 16384 * 0.625ms = 10.24s
pAdvertising->setMaxInterval(0x4000);
```

**Transmission Power**:
```cpp
// Set TX power level
esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P3);   // +3 dBm (max)
esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_N0);   //  0 dBm (default)
esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_N12);  // -12 dBm (min)
```

### Beacon Lifecycle

```cpp
void setup() {
    // 1. Initialize BLE
    BLEDevice::init("ESP32_Beacon");
    
    // 2. Create advertising object
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    
    // 3. Set advertising data
    pAdvertising->setAdvertisementData(oAdvertisementData);
    
    // 4. Configure advertising parameters
    pAdvertising->setMinInterval(/* ... */);
    pAdvertising->setMaxInterval(/* ... */);
    
    // 5. Start advertising
    pAdvertising->start();
}

void loop() {
    // Beacon runs autonomously
    // Optional: Add sleep mode for battery saving
    delay(1000);
}
```

## 🔧 Customization Ideas

### 1. **Multiple Beacon Rotation**

Cycle through different beacon IDs:

```cpp
const int NUM_BEACONS = 3;
struct BeaconConfig {
    uint16_t major;
    uint16_t minor;
    String name;
};

BeaconConfig beacons[NUM_BEACONS] = {
    {1, 100, "Entrance"},
    {1, 101, "Store Floor"},
    {1, 102, "Checkout"}
};

int currentBeacon = 0;

void rotateBeacon() {
    BLEDevice::deinit();
    delay(100);
    
    BeaconConfig &beacon = beacons[currentBeacon];
    setupBeacon(beacon.major, beacon.minor);
    
    Serial.printf("Now broadcasting: %s (Major: %d, Minor: %d)\n",
                  beacon.name.c_str(), beacon.major, beacon.minor);
    
    currentBeacon = (currentBeacon + 1) % NUM_BEACONS;
}

void loop() {
    delay(10000);  // Switch every 10 seconds
    rotateBeacon();
}
```

### 2. **Eddystone Protocol Support**

Google's open-source beacon format:

```cpp
// Eddystone-UID format (unique beacon identifier)
uint8_t eddystoneUID[] = {
    0x03, 0x03, 0xAA, 0xFE,           // Service UUID: 0xFEAA
    0x16, 0x16, 0xAA, 0xFE,           // Service Data
    0x00,                             // Frame Type: UID
    0xF0,                             // TX Power: -16 dBm
    
    // 10-byte Namespace ID
    0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A,
    
    // 6-byte Instance ID
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    
    0x00, 0x00                        // Reserved
};

// Eddystone-URL format (broadcast web address)
uint8_t eddystoneURL[] = {
    0x03, 0x03, 0xAA, 0xFE,
    0x10, 0x16, 0xAA, 0xFE,
    0x10,                             // Frame Type: URL
    0xF0,                             // TX Power
    0x03,                             // URL Scheme: https://
    'e', 's', 'p', '3', '2', '.', 'c', 'o', 'm'
};
```

### 3. **AltBeacon Format**

Open-source alternative to iBeacon:

```cpp
uint8_t altBeaconData[] = {
    0x1B, 0xFF,                       // Length and type
    0x18, 0x01,                       // AltBeacon manufacturer ID
    0xBE, 0xAC,                       // AltBeacon advertisement code
    
    // 20-byte Beacon ID (16-byte UUID + 2-byte Major + 2-byte Minor)
    0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2,
    0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0,
    0x00, 0x01,                       // Major
    0x00, 0x64,                       // Minor
    
    0xC5,                             // Reference RSSI
    0x00                              // Reserved
};
```

### 4. **Battery Level Monitoring**

Add battery status to beacon name or custom service:

```cpp
#include <esp_adc_cal.h>

float readBatteryVoltage() {
    // Read voltage from GPIO34 (connected to battery through voltage divider)
    int raw = analogRead(34);
    float voltage = (raw / 4095.0) * 3.3 * 2.0;  // Assuming 1:1 voltage divider
    return voltage;
}

int getBatteryPercent(float voltage) {
    // LiPo: 4.2V = 100%, 3.0V = 0%
    if (voltage >= 4.2) return 100;
    if (voltage <= 3.0) return 0;
    return (int)((voltage - 3.0) / 1.2 * 100);
}

void updateBeaconName() {
    float voltage = readBatteryVoltage();
    int percent = getBatteryPercent(voltage);
    
    char name[20];
    sprintf(name, "ESP32_B%d%%", percent);
    BLEDevice::init(name);
}
```

### 5. **Deep Sleep for Ultra-Low Power**

Wake periodically to broadcast:

```cpp
#define BEACON_DURATION 5000          // Advertise for 5 seconds
#define SLEEP_DURATION 55             // Sleep for 55 seconds

void enterDeepSleep() {
    Serial.println("Entering deep sleep...");
    Serial.flush();
    
    // Configure wake-up timer
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000);
    
    // Enter deep sleep
    esp_deep_sleep_start();
}

void loop() {
    // Advertise for specified duration
    delay(BEACON_DURATION);
    
    // Enter deep sleep
    enterDeepSleep();
}

// Power consumption:
// Active (advertising): ~80mA for 5 seconds
// Deep sleep: ~10µA for 55 seconds
// Average: ~7mA → 280+ hours on 2000mAh battery
```

### 6. **Encrypted Beacon (Security)**

Rotating encrypted identifiers:

```cpp
#include "mbedtls/aes.h"

uint8_t encryptionKey[16] = { /* Your AES-128 key */ };
uint32_t counter = 0;

void encryptBeaconID() {
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, encryptionKey, 128);
    
    // Encrypt counter to create rotating beacon ID
    uint8_t input[16] = {0};
    uint8_t output[16];
    memcpy(input, &counter, sizeof(counter));
    
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, output);
    mbedtls_aes_free(&aes);
    
    // Use encrypted data as beacon UUID
    memcpy(&beaconData[9], output, 16);
    counter++;
}

void loop() {
    delay(60000);  // Rotate every minute
    encryptBeaconID();
    updateAdvertising();
}
```

### 7. **Conditional Broadcasting**

Only advertise when conditions are met:

```cpp
#include <DHT.h>

DHT dht(15, DHT22);  // Temperature sensor on GPIO15

bool shouldBroadcast() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Only broadcast if temperature is in range
    if (temp < 20.0 || temp > 30.0) {
        Serial.println("Temperature out of range - pausing beacon");
        return false;
    }
    
    return true;
}

void loop() {
    if (shouldBroadcast()) {
        if (!advertising) {
            BLEDevice::startAdvertising();
            advertising = true;
        }
    } else {
        if (advertising) {
            BLEDevice::stopAdvertising();
            advertising = false;
        }
    }
    delay(5000);
}
```

### 8. **Multi-Protocol Beacon**

Broadcast iBeacon, Eddystone, and custom data simultaneously:

```cpp
void setupMultiProtocol() {
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    
    // Primary advertising (iBeacon)
    BLEAdvertisementData oAdvertisementData;
    oAdvertisementData.setFlags(0x06);
    oAdvertisementData.setManufacturerData(std::string((char*)iBeaconData, sizeof(iBeaconData)));
    
    // Scan response (Eddystone or custom data)
    BLEAdvertisementData oScanResponseData;
    oScanResponseData.setName("ESP32_Beacon");
    oScanResponseData.setCompleteServices(BLEUUID("FEAA"));  // Eddystone service
    
    pAdvertising->setAdvertisementData(oAdvertisementData);
    pAdvertising->setScanResponseData(oScanResponseData);
    pAdvertising->start();
}
```

### 9. **Web Configuration Interface**

Configure beacon over WiFi:

```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
    String html = R"(
        <html><body>
        <h1>ESP32 Beacon Configuration</h1>
        <form action="/update" method="POST">
            Major: <input name="major" value=")" + String(currentMajor) + R"("><br>
            Minor: <input name="minor" value=")" + String(currentMinor) + R"("><br>
            <input type="submit" value="Update">
        </form>
        </body></html>
    )";
    server.send(200, "text/html", html);
}

void handleUpdate() {
    currentMajor = server.arg("major").toInt();
    currentMinor = server.arg("minor").toInt();
    updateBeaconData();
    server.send(200, "text/html", "Updated! <a href='/'>Back</a>");
}

void setup() {
    // Start WiFi AP
    WiFi.softAP("ESP32-Beacon-Config");
    
    server.on("/", handleRoot);
    server.on("/update", HTTP_POST, handleUpdate);
    server.begin();
    
    setupBeacon();
}
```

### 10. **Proximity-Based Actions**

Respond to nearby beacons (requires scanner mode):

```cpp
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveManufacturerData()) {
            std::string data = advertisedDevice.getManufacturerData();
            
            // Check if it's an iBeacon
            if (data[0] == 0x4C && data[1] == 0x00) {
                int rssi = advertisedDevice.getRSSI();
                
                // Trigger action based on proximity
                if (rssi > -50) {
                    Serial.println("Very close beacon detected!");
                    digitalWrite(LED_BUILTIN, HIGH);
                } else {
                    digitalWrite(LED_BUILTIN, LOW);
                }
            }
        }
    }
};

// Alternate between advertising and scanning
void loop() {
    // Advertise for 5 seconds
    BLEDevice::startAdvertising();
    delay(5000);
    BLEDevice::stopAdvertising();
    
    // Scan for 2 seconds
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->start(2);
}
```

## ⚠️ Troubleshooting

### Beacon Not Detected

**Problem**: Scanner app doesn't find beacon

**Solutions**:
- **Verify upload**: Check Serial Monitor confirms "Beacon advertising started"
- **Bluetooth enabled**: Ensure phone Bluetooth is on
- **Permissions**: Grant location permission to scanner app (required on Android)
- **Distance**: Move within 5-10 meters of ESP32
- **Interference**: Turn off other Bluetooth devices
- **Restart ESP32**: Press reset button
- **Restart app**: Force close and reopen scanner app
- **Try different app**: Some apps are better than others
- **iOS restrictions**: iOS requires specific beacon apps, won't show in Settings

### Wrong UUID/Values Displayed

**Problem**: Scanner shows different UUID or Major/Minor values

**Solutions**:
- **Verify code**: Check UUID string is formatted correctly (with dashes)
- **Byte order**: Ensure Major/Minor are big-endian (MSB first)
- **Hex conversion**: Verify decimal to hex conversion is correct
- **Character encoding**: UUID must be uppercase hex digits
- **Re-upload**: Make sure latest code is uploaded
- **Clear cache**: Restart ESP32 to clear old advertising data

### Short Range/Weak Signal

**Problem**: Beacon only detected at very short range (< 2 meters)

**Solutions**:
- **Increase TX power**: Change to ESP_PWR_LVL_P3 (+3 dBm)
- **Antenna orientation**: Try rotating ESP32 for better signal
- **Remove enclosure**: Metal or thick plastic blocks signal
- **Check power**: Low voltage can reduce transmission power
- **Environment**: Move to open space, away from metal/water
- **Test antenna**: Some boards have external antenna connector

### High Power Consumption

**Problem**: Battery drains too quickly

**Solutions**:
- **Increase interval**: Set advertising interval to 1000ms or higher
- **Reduce TX power**: Lower transmission power to -12 dBm
- **Deep sleep**: Use deep sleep between advertising periods
- **WiFi off**: Disable WiFi radio: `WiFi.mode(WIFI_OFF)`
- **Reduce logging**: Remove Serial.print() statements
- **Optimize code**: Remove delays in loop()
- **Check battery**: Verify battery capacity and connections

```cpp
// Ultra-low power configuration
void setupLowPower() {
    // Disable WiFi
    WiFi.mode(WIFI_OFF);
    btStop();
    
    // Minimum TX power
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_N12);
    
    // Long advertising interval
    pAdvertising->setMinInterval(0x4000);  // 10.24 seconds
    pAdvertising->setMaxInterval(0x4000);
    
    // Reduce CPU frequency
    setCpuFrequencyMhz(80);  // Default is 240 MHz
}
```

### Compilation Errors

**Problem**: `BLEDevice.h: No such file or directory`

**Solutions**:
- **ESP32 core**: Install ESP32 board support in Board Manager
- **Update core**: Update to latest ESP32 Arduino core (min 1.0.4)
- **Board selected**: Verify ESP32 board is selected in Tools → Board

**Problem**: `undefined reference to 'BLEDevice::init'`

**Solutions**:
- **Partition scheme**: Change to larger partition: Tools → Partition Scheme → "Huge APP"
- **BLE enabled**: Some ESP32 variants don't support BLE (ESP32-S2)
- **Core version**: Update to ESP32 core 2.0.0 or newer

**Problem**: Sketch too large

**Solutions**:
- **Partition**: Tools → Partition Scheme → "Huge APP (3MB No OTA)"
- **Remove debug**: Delete Serial.print() statements
- **Optimize**: Remove unused libraries

### Unstable RSSI Readings

**Problem**: Distance jumps erratically

**Solutions**:
- **This is normal**: RSSI fluctuates naturally due to interference
- **Average readings**: Scanner apps typically average over several samples
- **Reduce movement**: Physical movement causes rapid changes
- **Environmental factors**: People, furniture, and obstacles affect signal
- **Multipath**: Reflections cause signal strength variations
- **Calibration**: Proper TX power calibration improves accuracy
- **Filter**: Implement Kalman filter or moving average in app

### Multiple Beacons Conflict

**Problem**: Multiple ESP32 beacons interfere with each other

**Solutions**:
- **Unique IDs**: Ensure each beacon has unique Major/Minor combination
- **Stagger intervals**: Offset advertising start times
- **Different UUIDs**: Use different UUIDs for separate applications
- **Increase interval**: Longer intervals reduce collision probability
- **Physical separation**: Space beacons adequately (3+ meters)

## 📚 Learning Outcomes

After completing this project, you'll understand:

✅ **Bluetooth Low Energy (BLE)**
- BLE architecture and roles (broadcaster, observer, peripheral, central)
- GAP (Generic Access Profile) and advertising
- Advertisement packets and scan responses
- Advertising intervals and timing
- BLE vs Bluetooth Classic differences
- BLE channels (37, 38, 39 for advertising)
- Power consumption optimization

✅ **iBeacon Protocol**
- Apple iBeacon specification
- Proximity UUID purpose and generation
- Major and Minor value hierarchies
- TX Power calibration and meaning
- iBeacon vs Eddystone vs AltBeacon
- Industry-standard beacon formats
- Compatibility considerations

✅ **Proximity Detection**
- RSSI (Received Signal Strength Indicator)
- Distance estimation algorithms
- Path loss and signal propagation
- Proximity zones (Immediate, Near, Far)
- Accuracy limitations and factors
- Environmental interference
- Trilateration and positioning

✅ **BLE Advertising**
- Advertisement data structure
- Manufacturer-specific data format
- Flags and service UUIDs
- Broadcast vs connectable modes
- Advertising channels (37, 38, 39)
- Scan response data
- Advertisement intervals and timing

✅ **Power Management**
- BLE power consumption profiles
- Advertising interval vs battery life
- Transmission power levels (-12 to +3 dBm)
- Deep sleep modes for ESP32
- Ultra-low power design techniques
- Battery life estimation and testing
- Power budgeting for portable devices

✅ **Practical Applications**
- Indoor positioning systems
- Proximity marketing strategies
- Asset tracking implementations
- Attendance and access control
- Museum and exhibition guides
- Retail analytics and customer flow
- Contact tracing protocols
- Smart home automation triggers

✅ **Protocol Comparison**
- **iBeacon**: Apple's proprietary format, wide iOS support
- **Eddystone**: Google's open format, URL broadcasting
- **AltBeacon**: Open-source alternative
- **Custom protocols**: Creating proprietary beacon formats

✅ **Signal Characteristics**
- Radio frequency propagation
- Inverse square law
- Free space path loss
- Obstacle attenuation
- Reflection and multipath
- Fresnel zones
- Antenna patterns and polarization

✅ **Security Considerations**
- Beacon spoofing risks
- Privacy implications
- Encrypted beacon protocols
- Rotating identifiers
- Secure beacon networks
- GDPR and privacy compliance

## 🚀 Next Steps

Ready to advance? Try these challenges:

### Beginner
1. **Change beacon parameters** - Experiment with different Major/Minor values
2. **Measure battery life** - Test how long beacon runs on battery
3. **Test range** - Map signal strength at different distances
4. **Try different apps** - Compare detection across multiple scanner apps

### Intermediate
5. **Add temperature sensor** - Broadcast temperature in custom beacon format
6. **Web configuration** - Create WiFi interface to change beacon settings
7. **Multiple beacons** - Deploy 3+ beacons with different locations
8. **OLED display** - Show current beacon configuration on screen
9. **Button control** - Change Major/Minor with physical buttons
10. **Battery monitor** - Display battery level in beacon name

### Advanced
11. **Indoor positioning** - Use 4+ beacons for triangulation
12. **Geofencing app** - Create mobile app that triggers on beacon proximity
13. **Eddystone implementation** - Add Eddystone-URL broadcasting
14. **Encrypted beacons** - Implement rotating encrypted identifiers
15. **Mesh network** - Beacons relay data between each other
16. **Asset tracking system** - Track equipment location in warehouse
17. **Contact tracing** - Log beacon encounters for contact tracing

### Real-World Projects

**Retail Store Navigation**:
- Deploy beacons throughout store (one per department)
- Mobile app guides customers to products
- Track customer flow and dwell time
- Send proximity-based promotions

**Museum Audio Guide**:
- Place beacon at each exhibit
- App plays audio description when visitor approaches
- Track visitor path through museum
- Collect analytics on popular exhibits

**Office Attendance System**:
- Beacon at each entrance/office
- App logs entry/exit times automatically
- Generate attendance reports
- Integrate with calendar/booking system

**Smart Home Automation**:
- Beacon in car (phone detects approaching home)
- Trigger garage door, lights, HVAC
- Different actions for different family members
- Energy-saving modes when away

**Conference/Event Check-in**:
- Beacon at registration desk
- Automatic check-in when attendee approaches
- Track session attendance
- Networking features (find nearby attendees)

**Warehouse Asset Tracking**:
- Beacon on each piece of equipment
- Scanner tracks location as items move
- Alert when items leave designated area
- Inventory management integration

## 📖 References

### Documentation
- [ESP32 BLE Arduino](https://github.com/nkolban/ESP32_BLE_Arduino) - Official BLE library
- [Apple iBeacon Specification](https://developer.apple.com/ibeacon/) - Official iBeacon docs
- [Bluetooth Core Specification](https://www.bluetooth.com/specifications/specs/core-specification/) - BLE standard
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

### Protocols & Standards
- [iBeacon Protocol Details](https://en.wikipedia.org/wiki/IBeacon)
- [Eddystone Protocol](https://github.com/google/eddystone) - Google's beacon format
- [AltBeacon Specification](https://github.com/AltBeacon/spec) - Open beacon standard
- [Physical Web](https://google.github.io/physical-web/) - Web URLs via beacons

### Tutorials & Guides
- [Introduction to BLE Beacons](https://www.bluetooth.com/blog/a-primer-on-bluetooth-beacon-technology/)
- [iBeacon Indoor Positioning](https://www.mokosmart.com/ibeacon-indoor-positioning/)
- [RSSI Distance Estimation](https://www.rn.inf.tu-dresden.de/dargie/papers/ekberg.pdf)
- [BLE Advertising Tutorial](https://www.argenox.com/library/bluetooth-low-energy/ble-advertising-primer/)

### Apps & Tools
- [nRF Connect](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-mobile) - Professional BLE tool (iOS/Android)
- [Locate Beacon](https://apps.apple.com/app/locate-beacon/id738709014) - iOS beacon scanner
- [Beacon Scanner](https://play.google.com/store/apps/details?id=com.bridou_n.beaconscanner) - Android app
- [BeaconToy](https://www.beacontoy.com/) - Beacon simulator for testing
- [Wireshark](https://www.wireshark.org/) - BLE packet analyzer

### Development Resources
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) - Arduino ESP32 support
- [BLE Beacon Library](https://github.com/nkolban/ESP32_BLE_Arduino) - ESP32 BLE examples
- [Beacon Reference Apps](https://github.com/RadiusNetworks) - Open-source beacon apps
- [Indoor Positioning Systems](https://github.com/IndoorLBS) - IPS implementations

### Online Calculators
- [UUID Generator](https://www.uuidgenerator.net/) - Generate unique UUIDs
- [RSSI Distance Calculator](https://iotandelectronics.wordpress.com/2016/10/07/how-to-calculate-distance-from-the-rssi-value-of-the-ble-beacon/)
- [Battery Life Estimator](https://www.digikey.com/en/resources/conversion-calculators/conversion-calculator-battery-life)
- [Path Loss Calculator](https://www.pasternack.com/t-calculator-fspl.aspx)

### Community & Forums
- [ESP32 Forum - BLE Section](https://esp32.com/viewforum.php?f=19)
- [Bluetooth SIG Community](https://www.bluetooth.com/community/)
- [Reddit r/esp32](https://reddit.com/r/esp32)
- [Stack Overflow - BLE Tag](https://stackoverflow.com/questions/tagged/bluetooth-lowenergy)

### Research & Papers
- [Indoor Positioning Using BLE Beacons](https://ieeexplore.ieee.org/document/7562959)
- [RSSI-Based Positioning Algorithms](https://www.mdpi.com/1424-8220/20/13/3611)
- [BLE Security and Privacy](https://www.usenix.org/system/files/conference/woot16/woot16-paper-sivakumaran.pdf)

### Commercial Beacon Providers
- [Estimote](https://estimote.com/) - Hardware beacons and SDK
- [Kontakt.io](https://kontakt.io/) - Enterprise beacon solutions
- [Gimbal](https://gimbal.com/) - Location and proximity platform
- [Blue Sense Networks](https://bluesensenetworks.com/) - Industrial beacons

## 💡 Tips for Success

1. **UUID Generation**: Generate a unique UUID for your application - never use examples in production
2. **Calibrate TX Power**: Measure RSSI at exactly 1 meter in open space for accurate distance estimation
3. **Test Range**: Walk different paths to understand how obstacles affect signal
4. **Battery Optimization**: Longer advertising intervals dramatically improve battery life
5. **Multiple Apps**: Test with 2-3 different scanner apps to ensure compatibility
6. **Privacy Awareness**: Beacons broadcast continuously - consider privacy implications
7. **Documentation**: Keep a spreadsheet of Major/Minor assignments for multiple beacons
8. **Physical Placement**: Mount beacons 1.5-2 meters high for optimal coverage
9. **Interference Testing**: Test near WiFi routers and other BLE devices
10. **Backup Configuration**: Save your UUID and settings - they're hard to recover if lost

## ⚠️ Important Notes

**Privacy Considerations**: BLE beacons continuously broadcast their identifier. While they don't transmit personal information, apps can track user proximity over time. Consider:
- Informing users of beacon deployment
- GDPR compliance for EU deployments
- Opt-in tracking policies
- Data retention limits
- Anonymization strategies

**Not GPS**: Beacons provide proximity detection, not absolute positioning. They work best:
- Indoors where GPS doesn't work
- Combined with multiple beacons for triangulation
- For zone-based detection (entrance, room, area)
- With smartphone app to process beacon data

**Range Limitations**: Advertised range (up to 100m) is theoretical:
- Indoors: 10-30 meters typical
- Obstacles reduce range significantly
- Human bodies absorb 2.4 GHz signals
- Metal and water block signals

**Battery Life**: Actual battery life depends on many factors:
- Advertising interval (major factor)
- Transmission power
- Battery capacity and quality
- Temperature (cold reduces capacity)
- Self-discharge rate
- ESP32 power management efficiency

**iOS Requirements**: Apple restricts beacon detection:
- Must use CoreLocation framework
- Requires app with beacon monitoring capability
- Background monitoring has limitations
- "Always" location permission needed for background detection
- Won't appear in standard Bluetooth settings

---

**Project Complete!** 🎉

You've created a powerful BLE beacon that can serve as the foundation for indoor positioning, proximity marketing, asset tracking, and countless other location-based applications. This simple project opens the door to sophisticated location-aware systems!

**Share Your Deployment**: Post your beacon network with #ESP32Beacon #iBeacon #IoT

Next up: **Project 11 - Bluetooth Gamepad** where we'll create a wireless game controller! 🎮
