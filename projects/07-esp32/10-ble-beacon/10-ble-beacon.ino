/*
 * ESP32 BLE Beacon - iBeacon Implementation
 * 
 * This sketch turns your ESP32 into an iBeacon transmitter using Apple's
 * Bluetooth Low Energy beacon protocol. Perfect for proximity marketing,
 * indoor navigation, attendance tracking, and asset management.
 * 
 * iBeacon broadcasts a unique identifier consisting of:
 * - UUID (16 bytes): Organization/application identifier
 * - Major (2 bytes): Location identifier (e.g., store number)
 * - Minor (2 bytes): Specific beacon identifier (e.g., department)
 * - TX Power (1 byte): Calibrated signal strength at 1 meter
 * 
 * Features:
 * - Standard iBeacon protocol implementation
 * - Configurable UUID, Major, Minor values
 * - Adjustable transmission power and advertising interval
 * - Ultra-low power consumption
 * - No external hardware required
 * - Compatible with iOS and Android beacon scanner apps
 * 
 * Hardware Required:
 * - ESP32 Development Board (any variant with BLE support)
 * - USB cable or battery for power
 * 
 * Testing:
 * - iOS: Locate Beacon, BeaconScanner, or nRF Connect app
 * - Android: Beacon Scanner, nRF Connect, or Beacon Simulator app
 * 
 * How to Use:
 * 1. Upload this sketch to your ESP32
 * 2. Open Serial Monitor at 115200 baud
 * 3. Install a beacon scanner app on your smartphone
 * 4. Scan for beacons and look for your UUID
 * 5. Observe RSSI values and distance estimation
 * 
 * Customization:
 * - Generate your own UUID at https://www.uuidgenerator.net/
 * - Set Major/Minor values to identify your beacon network
 * - Calibrate TX_POWER by measuring RSSI at exactly 1 meter
 * - Adjust advertising interval for battery life vs responsiveness
 * 
 * Power Consumption:
 * - Active: ~15-30mA (depending on TX power and interval)
 * - Battery life with 2000mAh: 60-130 hours
 * - Increase advertising interval for longer battery life
 * 
 * Author: Arduino Practice Project
 * License: MIT
 * Version: 1.0
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>
#include <esp_bt.h>

// ===== CONFIGURATION =====
// Customize these values for your beacon deployment

// Proximity UUID - Generate your own at https://www.uuidgenerator.net/
// All beacons in your network typically share the same UUID
#define BEACON_UUID "E2C56DB5-DFFB-48D2-B060-D0F5A71096E0"

// Major value (0-65535) - Identifies a location or group
// Example: Store #1 = 1, Store #2 = 2
#define MAJOR_VALUE 1

// Minor value (0-65535) - Identifies specific beacon within Major group
// Example: Entrance = 1, Electronics = 2, Checkout = 3
#define MINOR_VALUE 100

// TX Power - Calibrated signal strength at 1 meter (typically -59 to -65)
// Measure RSSI at exactly 1 meter to calibrate this value
#define TX_POWER -59

// Advertising interval in milliseconds
// Shorter interval = more responsive but higher power consumption
// 100ms  = very responsive, ~30-40mA (good for real-time tracking)
// 1000ms = balanced, ~15-20mA (default, good for most uses)
// 10000ms = battery saver, ~8-12mA (good for long deployments)
#define ADVERTISING_INTERVAL_MS 1000

// Transmission power level
// Options: ESP_PWR_LVL_N12 (-12dBm, lowest), ESP_PWR_LVL_N0 (0dBm, default),
//          ESP_PWR_LVL_P3 (+3dBm, maximum range)
#define TX_POWER_LEVEL ESP_PWR_LVL_N0

// Device name (optional, for identification in some scanners)
#define DEVICE_NAME "ESP32_Beacon"

// ===== GLOBAL VARIABLES =====
BLEAdvertising *pAdvertising;
bool beaconActive = false;

// ===== HELPER FUNCTIONS =====

/**
 * Convert UUID string to byte array
 * Input: "E2C56DB5-DFFB-48D2-B060-D0F5A71096E0"
 * Output: [0xE2, 0xC5, 0x6D, 0xB5, ...]
 */
void uuidStringToBytes(const char* uuidStr, uint8_t* uuidBytes) {
    int byteIndex = 0;
    for (int i = 0; i < strlen(uuidStr); i++) {
        if (uuidStr[i] == '-') continue;  // Skip dashes
        
        // Convert two hex characters to one byte
        char hexByte[3] = {uuidStr[i], uuidStr[i+1], '\0'};
        uuidBytes[byteIndex++] = (uint8_t)strtol(hexByte, NULL, 16);
        i++;  // Skip next character (already processed)
    }
}

/**
 * Print UUID in formatted string
 */
void printUUID(uint8_t* uuid) {
    for (int i = 0; i < 16; i++) {
        Serial.printf("%02X", uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) Serial.print("-");
    }
}

/**
 * Setup and start iBeacon advertising
 */
void setupBeacon() {
    Serial.println("\n════════════════════════════════════════");
    Serial.println("   ESP32 BLE Beacon (iBeacon)");
    Serial.println("════════════════════════════════════════\n");
    
    // Initialize BLE
    Serial.println("Initializing BLE...");
    BLEDevice::init(DEVICE_NAME);
    
    // Get advertising instance
    pAdvertising = BLEDevice::getAdvertising();
    
    // Create iBeacon data packet
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    
    // Parse UUID string to bytes
    uint8_t uuidBytes[16];
    uuidStringToBytes(BEACON_UUID, uuidBytes);
    
    // Build iBeacon manufacturer data (25 bytes total)
    uint8_t beaconData[25];
    int idx = 0;
    
    // Apple Company Identifier (2 bytes) - Little Endian
    beaconData[idx++] = 0x4C;
    beaconData[idx++] = 0x00;
    
    // iBeacon Type (1 byte)
    beaconData[idx++] = 0x02;
    
    // iBeacon Data Length (1 byte)
    beaconData[idx++] = 0x15;  // 21 bytes of data follows
    
    // Proximity UUID (16 bytes)
    for (int i = 0; i < 16; i++) {
        beaconData[idx++] = uuidBytes[i];
    }
    
    // Major (2 bytes) - Big Endian
    beaconData[idx++] = (MAJOR_VALUE >> 8) & 0xFF;  // MSB
    beaconData[idx++] = MAJOR_VALUE & 0xFF;         // LSB
    
    // Minor (2 bytes) - Big Endian
    beaconData[idx++] = (MINOR_VALUE >> 8) & 0xFF;  // MSB
    beaconData[idx++] = MINOR_VALUE & 0xFF;         // LSB
    
    // TX Power (1 byte) - Signed integer in two's complement
    beaconData[idx++] = (uint8_t)TX_POWER;
    
    // Set manufacturer data
    std::string strBeaconData((char*)beaconData, sizeof(beaconData));
    oAdvertisementData.addData(strBeaconData);
    
    // Configure advertising parameters
    pAdvertising->setAdvertisementData(oAdvertisementData);
    
    // Set advertising interval
    // BLE units: 0.625ms per unit
    // Convert milliseconds to BLE units
    uint16_t intervalUnits = ADVERTISING_INTERVAL_MS * 1000 / 625;
    pAdvertising->setMinInterval(intervalUnits);
    pAdvertising->setMaxInterval(intervalUnits);
    
    // Set transmission power
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, TX_POWER_LEVEL);
    
    // Disable scan response (we're only advertising, not connectable)
    pAdvertising->setScanResponse(false);
    
    // Print configuration
    Serial.println("Configuration:");
    Serial.print("  UUID:  ");
    printUUID(uuidBytes);
    Serial.println();
    Serial.printf("  Major: %d\n", MAJOR_VALUE);
    Serial.printf("  Minor: %d\n", MINOR_VALUE);
    Serial.printf("  TX Power: %d dBm\n", TX_POWER);
    Serial.println();
    
    // Start advertising
    Serial.println("✓ BLE initialized");
    pAdvertising->start();
    Serial.println("✓ Beacon advertising started\n");
    
    beaconActive = true;
    
    // Print usage information
    Serial.println("Beacon is broadcasting...");
    Serial.println("Use a beacon scanner app to detect!\n");
    
    Serial.printf("Advertising Interval: %d ms\n", ADVERTISING_INTERVAL_MS);
    
    // Print TX power level description
    Serial.print("Transmission Power: ");
    switch(TX_POWER_LEVEL) {
        case ESP_PWR_LVL_N12: Serial.println("-12 dBm (Low power, ~10m range)"); break;
        case ESP_PWR_LVL_N9:  Serial.println("-9 dBm"); break;
        case ESP_PWR_LVL_N6:  Serial.println("-6 dBm"); break;
        case ESP_PWR_LVL_N3:  Serial.println("-3 dBm"); break;
        case ESP_PWR_LVL_N0:  Serial.println("0 dBm (Standard, ~50m range)"); break;
        case ESP_PWR_LVL_P3:  Serial.println("+3 dBm (Maximum, ~100m range)"); break;
        default: Serial.println("Unknown");
    }
    
    // Print power consumption estimate
    Serial.println("\nEstimated Power Consumption:");
    if (ADVERTISING_INTERVAL_MS <= 200) {
        Serial.println("  ~30-40mA (High responsiveness mode)");
        Serial.println("  Battery life: ~50-65 hours (2000mAh)");
    } else if (ADVERTISING_INTERVAL_MS <= 2000) {
        Serial.println("  ~15-20mA (Balanced mode)");
        Serial.println("  Battery life: ~100-130 hours (2000mAh)");
    } else {
        Serial.println("  ~8-12mA (Battery saver mode)");
        Serial.println("  Battery life: ~160-250 hours (2000mAh)");
    }
    
    Serial.println("\n════════════════════════════════════════");
    Serial.println("Beacon is active and broadcasting!");
    Serial.println("════════════════════════════════════════\n");
    
    Serial.println("📱 How to Test:");
    Serial.println("  1. Install beacon scanner app on phone");
    Serial.println("     iOS: Locate Beacon, nRF Connect");
    Serial.println("     Android: Beacon Scanner, nRF Connect");
    Serial.println("  2. Open app and start scanning");
    Serial.println("  3. Look for UUID: " + String(BEACON_UUID));
    Serial.println("  4. Verify Major: " + String(MAJOR_VALUE) + 
                   ", Minor: " + String(MINOR_VALUE));
    Serial.println("  5. Move closer/farther to see RSSI change\n");
    
    Serial.println("💡 Tips:");
    Serial.println("  - RSSI (signal strength) decreases with distance");
    Serial.println("  - Typical values: -50 dBm (close) to -90 dBm (far)");
    Serial.println("  - Walls and obstacles reduce signal strength");
    Serial.println("  - Multiple readings average for better accuracy\n");
}

// ===== ARDUINO SETUP =====
void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);  // Wait for serial to initialize
    
    // Setup and start beacon
    setupBeacon();
    
    // Optional: Disable WiFi to save power
    // Uncomment the line below to disable WiFi radio
    // WiFi.mode(WIFI_OFF);
}

// ===== ARDUINO LOOP =====
void loop() {
    // Beacon runs autonomously - nothing to do in loop!
    // BLE advertising is handled by ESP32 BLE stack in background
    
    // Optional: Add LED heartbeat to show beacon is alive
    // digitalWrite(LED_BUILTIN, HIGH);
    // delay(100);
    // digitalWrite(LED_BUILTIN, LOW);
    
    // Simple delay to reduce unnecessary CPU cycles
    delay(1000);
    
    // The beacon will continue broadcasting indefinitely
    // Reset ESP32 to restart beacon with new configuration
}

// ===== OPTIONAL: ALTERNATIVE IMPLEMENTATIONS =====

/*
 * EDDYSTONE-UID IMPLEMENTATION
 * Google's open-source beacon format with namespace and instance
 */
#ifdef USE_EDDYSTONE_UID
void setupEddystoneUID() {
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    
    // Eddystone-UID frame
    uint8_t eddystoneData[] = {
        0xAA, 0xFE,              // Eddystone Service UUID (0xFEAA)
        0x00,                    // Frame Type: UID
        0xF0,                    // TX Power at 0m: -16 dBm
        
        // 10-byte Namespace ID (your organization/app identifier)
        0x01, 0x02, 0x03, 0x04, 0x05,
        0x06, 0x07, 0x08, 0x09, 0x0A,
        
        // 6-byte Instance ID (specific beacon identifier)
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        
        0x00, 0x00               // Reserved for future use
    };
    
    std::string strEddystoneData((char*)eddystoneData, sizeof(eddystoneData));
    oAdvertisementData.addData(strEddystoneData);
    pAdvertising->setAdvertisementData(oAdvertisementData);
}
#endif

/*
 * EDDYSTONE-URL IMPLEMENTATION
 * Broadcast a web URL that appears in Chrome's Physical Web
 */
#ifdef USE_EDDYSTONE_URL
void setupEddystoneURL() {
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    
    // Eddystone-URL frame
    uint8_t eddystoneData[] = {
        0xAA, 0xFE,              // Eddystone Service UUID
        0x10,                    // Frame Type: URL
        0xF0,                    // TX Power at 0m: -16 dBm
        0x03,                    // URL Scheme: https://
        
        // URL: "esp32.com" (encoded)
        'e', 's', 'p', '3', '2', '.', 'c', 'o', 'm'
    };
    
    // URL Scheme codes:
    // 0x00 = http://www.
    // 0x01 = https://www.
    // 0x02 = http://
    // 0x03 = https://
    
    std::string strEddystoneData((char*)eddystoneData, sizeof(eddystoneData));
    oAdvertisementData.addData(strEddystoneData);
    pAdvertising->setAdvertisementData(oAdvertisementData);
}
#endif

/*
 * ALTBEACON IMPLEMENTATION
 * Open-source alternative to iBeacon with similar functionality
 */
#ifdef USE_ALTBEACON
void setupAltBeacon() {
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    
    uint8_t uuidBytes[16];
    uuidStringToBytes(BEACON_UUID, uuidBytes);
    
    // AltBeacon frame
    uint8_t altBeaconData[28];
    int idx = 0;
    
    // AltBeacon manufacturer ID (0x0118)
    altBeaconData[idx++] = 0x18;
    altBeaconData[idx++] = 0x01;
    
    // AltBeacon advertisement code
    altBeaconData[idx++] = 0xBE;
    altBeaconData[idx++] = 0xAC;
    
    // 20-byte Beacon ID (16-byte UUID + 2-byte Major + 2-byte Minor)
    for (int i = 0; i < 16; i++) {
        altBeaconData[idx++] = uuidBytes[i];
    }
    
    // Major (2 bytes)
    altBeaconData[idx++] = (MAJOR_VALUE >> 8) & 0xFF;
    altBeaconData[idx++] = MAJOR_VALUE & 0xFF;
    
    // Minor (2 bytes)
    altBeaconData[idx++] = (MINOR_VALUE >> 8) & 0xFF;
    altBeaconData[idx++] = MINOR_VALUE & 0xFF;
    
    // Reference RSSI at 1m
    altBeaconData[idx++] = (uint8_t)TX_POWER;
    
    // Reserved
    altBeaconData[idx++] = 0x00;
    
    std::string strAltBeaconData((char*)altBeaconData, sizeof(altBeaconData));
    oAdvertisementData.addData(strAltBeaconData);
    pAdvertising->setAdvertisementData(oAdvertisementData);
}
#endif

/*
 * BATTERY-OPTIMIZED DEEP SLEEP MODE
 * Beacon wakes up, advertises briefly, then sleeps
 * Dramatically extends battery life at cost of responsiveness
 */
#ifdef USE_DEEP_SLEEP
#define ACTIVE_TIME_MS 5000      // Advertise for 5 seconds
#define SLEEP_TIME_SEC 55        // Sleep for 55 seconds

void enterDeepSleep() {
    Serial.println("\n⏳ Entering deep sleep mode...");
    Serial.printf("   Sleeping for %d seconds\n", SLEEP_TIME_SEC);
    Serial.flush();
    
    // Stop BLE advertising
    pAdvertising->stop();
    BLEDevice::deinit();
    
    // Configure wake-up timer
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_SEC * 1000000ULL);
    
    // Enter deep sleep
    esp_deep_sleep_start();
    
    // Execution continues from setup() after wake-up
}

void setupDeepSleepMode() {
    // If waking from deep sleep, show wake-up message
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
        Serial.println("\n⏰ Woke from deep sleep!");
    }
    
    // Setup beacon normally
    setupBeacon();
    
    // Wait for active period, then sleep
    delay(ACTIVE_TIME_MS);
    enterDeepSleep();
}

// Use setupDeepSleepMode() instead of setupBeacon() in setup()
#endif

/*
 * TX POWER CALIBRATION HELPER
 * Helps you measure and calibrate the TX_POWER value
 * Place phone exactly 1 meter from beacon and record RSSI
 */
#ifdef CALIBRATE_TX_POWER
void calibrateTXPower() {
    Serial.println("\n╔══════════════════════════════════════╗");
    Serial.println("║   TX POWER CALIBRATION MODE          ║");
    Serial.println("╚══════════════════════════════════════╝\n");
    Serial.println("Instructions:");
    Serial.println("1. Place beacon and phone on flat surface");
    Serial.println("2. Measure exactly 1.0 meter distance");
    Serial.println("3. Open beacon scanner app on phone");
    Serial.println("4. Record RSSI value (average of 10+ readings)");
    Serial.println("5. Update TX_POWER constant with this value\n");
    Serial.println("Tips:");
    Serial.println("- Remove obstacles between beacon and phone");
    Serial.println("- Ensure line of sight");
    Serial.println("- Take multiple readings and average");
    Serial.println("- Typical values: -55 to -65 dBm\n");
    Serial.println("Beacon is now advertising for calibration...\n");
}

// Call calibrateTXPower() at start of setup() for calibration mode
#endif

/*
 * CUSTOM BEACON DATA FORMAT
 * Example: Include sensor data in beacon advertisement
 */
#ifdef USE_CUSTOM_BEACON
#include <DHT.h>

#define DHT_PIN 15
DHT dht(DHT_PIN, DHT22);

void setupCustomBeacon() {
    // Read sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Convert to integers for compact transmission
    int16_t temp = (int16_t)(temperature * 10);  // 1 decimal place
    uint8_t hum = (uint8_t)humidity;
    
    // Custom manufacturer data format
    uint8_t customData[] = {
        0xFF, 0xFF,              // Custom manufacturer ID
        0x01,                    // Data type: Sensor data
        
        // Temperature (2 bytes, signed)
        (temp >> 8) & 0xFF,
        temp & 0xFF,
        
        // Humidity (1 byte)
        hum,
        
        // Battery level (1 byte, 0-100%)
        100,  // TODO: Read actual battery level
        
        // Reserved for future use
        0x00, 0x00
    };
    
    BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
    std::string strCustomData((char*)customData, sizeof(customData));
    oAdvertisementData.addData(strCustomData);
    pAdvertising->setAdvertisementData(oAdvertisementData);
    
    Serial.printf("Broadcasting: Temp=%.1f°C, Humidity=%d%%\n", 
                  temperature, (int)humidity);
}

// Update beacon data periodically in loop()
#endif
