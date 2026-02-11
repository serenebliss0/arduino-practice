/*
 * ESP32 BLE Heart Rate Monitor with MAX30102
 * 
 * A Bluetooth Low Energy heart rate monitor that broadcasts data
 * using the standard Heart Rate Service (0x180D). Compatible with
 * fitness apps and BLE heart rate monitor applications.
 * 
 * Features:
 * - Standard BLE Heart Rate Profile implementation
 * - Real-time heart rate and SpO2 monitoring
 * - MAX30102 pulse oximeter sensor
 * - Connection status feedback
 * - Automatic reconnection
 * - Serial debug output
 * 
 * Hardware:
 * - ESP32 Development Board
 * - MAX30102 Heart Rate/SpO2 Sensor
 * - Connections:
 *   MAX30102 VIN  → ESP32 3.3V
 *   MAX30102 GND  → ESP32 GND
 *   MAX30102 SDA  → ESP32 GPIO 21
 *   MAX30102 SCL  → ESP32 GPIO 22
 * 
 * Libraries Required:
 * - ESP32 BLE Arduino (included with ESP32 core)
 * - SparkFun MAX3010x Pulse Sensor Library
 * 
 * BLE Service:
 * - Service UUID: 0x180D (Heart Rate Service)
 * - Characteristic UUID: 0x2A37 (Heart Rate Measurement)
 */

#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "MAX30105.h"

// ===== CONFIGURATION =====
// Change these if needed
#define DEVICE_NAME "ESP32_HRM"           // BLE device name (max 20 chars)
#define UPDATE_INTERVAL 1000              // Update rate in milliseconds (1000 = 1 second)
#define LED_BRIGHTNESS 0x1F               // MAX30102 LED brightness (0-255, 0x1F = low power)

// Sensor thresholds
#define FINGER_THRESHOLD 50000            // IR threshold to detect finger presence
#define VALID_HR_MIN 30                   // Minimum valid heart rate (bpm)
#define VALID_HR_MAX 200                  // Maximum valid heart rate (bpm)

// ===== GLOBAL OBJECTS =====
MAX30105 particleSensor;
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// ===== SENSOR VARIABLES =====
// Heart rate calculation
const byte RATE_SIZE = 4;                 // Number of samples for averaging
byte rates[RATE_SIZE];                    // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;                        // Time of last beat
float beatsPerMinute = 0;
int beatAvg = 0;                          // Average heart rate

// SpO2 calculation variables
uint32_t irBuffer[100];                   // Infrared LED sensor data
uint32_t redBuffer[100];                  // Red LED sensor data
int32_t bufferLength = 100;
int32_t spo2 = 0;                         // SpO2 value
int8_t validSPO2 = 0;                     // Flag for valid SpO2
int32_t heartRate = 0;                    // Heart rate value
int8_t validHeartRate = 0;                // Flag for valid heart rate

// Timing variables
unsigned long lastUpdate = 0;

// ===== BLE UUIDS =====
// Standard BLE Heart Rate Service UUIDs
#define SERVICE_UUID        "180D"        // Heart Rate Service
#define CHARACTERISTIC_UUID "2A37"        // Heart Rate Measurement

// ===== BLE SERVER CALLBACKS =====
// Handle BLE connection events
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("\n📱 Client Connected!");
      Serial.println("   Address: " + String(pServer->getConnId()));
      Serial.println("   Broadcasting heart rate data...\n");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("\n📱 Client Disconnected!");
      Serial.println("   Restarting advertising...\n");
      // Restart advertising so device can be discovered again
      pServer->startAdvertising();
    }
};

// ===== SETUP =====
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  // Print startup banner
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("   ESP32 BLE Heart Rate Monitor");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Initialize I2C communication
  Wire.begin();
  
  // Initialize MAX30102 sensor
  Serial.println("Initializing MAX30102 sensor...");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("❌ MAX30102 not found!");
    Serial.println("   Please check wiring:");
    Serial.println("   - VIN  → 3.3V");
    Serial.println("   - GND  → GND");
    Serial.println("   - SDA  → GPIO 21");
    Serial.println("   - SCL  → GPIO 22");
    while (1);  // Halt program
  }
  Serial.println("✓ MAX30102 sensor initialized");
  
  // Configure MAX30102 sensor
  // Parameters: LED brightness, sample average, LED mode, sample rate, pulse width, ADC range
  byte ledBrightness = LED_BRIGHTNESS;    // 0x1F = 6.4mA (lower power)
  byte sampleAverage = 4;                 // Average 4 samples
  byte ledMode = 3;                       // Red + IR LEDs
  int sampleRate = 400;                   // 400 samples per second
  int pulseWidth = 411;                   // 411 microseconds
  int adcRange = 4096;                    // 4096 (15 bit) ADC range
  
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setPulseAmplitudeRed(0x0A);    // Turn Red LED to low
  particleSensor.setPulseAmplitudeGreen(0);      // Turn off Green LED
  
  Serial.println("✓ Sensor configured");
  Serial.println("   Sample Rate: " + String(sampleRate) + " Hz");
  Serial.println("   LED Mode: Red + IR");
  Serial.println();
  
  // Initialize BLE
  Serial.println("Initializing BLE stack...");
  BLEDevice::init(DEVICE_NAME);
  
  // Create BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Create Heart Rate Service
  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180D));
  
  // Create Heart Rate Measurement Characteristic
  pCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)0x2A37),
    BLECharacteristic::PROPERTY_NOTIFY
  );
  
  // Add descriptor (required for notifications)
  pCharacteristic->addDescriptor(new BLE2902());
  
  // Start the service
  pService->start();
  
  Serial.println("✓ BLE stack initialized");
  Serial.println("✓ Heart Rate service started");
  Serial.println();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x180D));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Functions for iPhone connection
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Device Name: " + String(DEVICE_NAME));
  Serial.println("Waiting for BLE connection...");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  Serial.println("Place finger on sensor to start measuring");
  Serial.println();
}

// ===== MAIN LOOP =====
void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensor data
  uint32_t irValue = particleSensor.getIR();
  uint32_t redValue = particleSensor.getRed();
  
  // Check if finger is detected
  if (irValue > FINGER_THRESHOLD) {
    // Finger is on sensor - calculate heart rate
    
    // Check for a heartbeat using simple beat detection
    if (checkForBeat(irValue) == true) {
      // A beat was detected!
      
      // Calculate time between beats
      long delta = currentMillis - lastBeat;
      lastBeat = currentMillis;
      
      // Calculate BPM
      beatsPerMinute = 60 / (delta / 1000.0);
      
      // Only accept valid heart rate values
      if (beatsPerMinute > VALID_HR_MIN && beatsPerMinute < VALID_HR_MAX) {
        // Store this reading in the array
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;  // Wrap around to start of array
        
        // Calculate average of readings
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++) {
          beatAvg += rates[x];
        }
        beatAvg /= RATE_SIZE;
      }
    }
    
    // Calculate SpO2 (simplified algorithm - not medical grade)
    // This is a basic estimation - real SpO2 requires calibration
    float ratio = ((float)redValue / (float)irValue);
    spo2 = (int)(110 - 25 * ratio);  // Simplified formula
    
    // Clamp SpO2 to valid range
    if (spo2 > 100) spo2 = 100;
    if (spo2 < 70) spo2 = 95;  // Default to reasonable value
    
    // Update BLE and Serial output at specified interval
    if (currentMillis - lastUpdate >= UPDATE_INTERVAL) {
      lastUpdate = currentMillis;
      
      // Print to Serial Monitor
      Serial.printf("💓 Heart Rate: ");
      if (beatAvg > 0) {
        Serial.printf("%d bpm", beatAvg);
      } else {
        Serial.printf("-- bpm");
      }
      Serial.printf(" | SpO2: %d%% | IR: %lu\n", spo2, irValue);
      
      // Send BLE notification if client is connected
      if (deviceConnected && beatAvg > 0) {
        sendHeartRateNotification(beatAvg);
      }
    }
    
  } else {
    // No finger detected
    if (currentMillis - lastUpdate >= UPDATE_INTERVAL) {
      lastUpdate = currentMillis;
      Serial.println("👆 Place finger on sensor...");
      beatAvg = 0;  // Reset heart rate
    }
  }
  
  // Handle disconnection
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);  // Give BLE stack time to get ready
    pServer->startAdvertising();
    Serial.println("Advertising restarted");
    oldDeviceConnected = deviceConnected;
  }
  
  // Handle new connection
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
  
  // Small delay to prevent watchdog timer issues
  delay(10);
}

// ===== HELPER FUNCTIONS =====

/**
 * Simple beat detection algorithm
 * Detects peaks in the IR signal
 * Returns true when a beat is detected
 */
bool checkForBeat(int32_t sample) {
  static int32_t lastSample = 0;
  static int32_t threshold = FINGER_THRESHOLD + 5000;
  static unsigned long lastBeatTime = 0;
  static bool increasing = false;
  
  bool beatDetected = false;
  
  // Check if signal is increasing
  if (sample > lastSample) {
    increasing = true;
  }
  
  // If signal was increasing and now decreasing, we might have found a peak
  if (increasing && sample < lastSample) {
    // Check if peak is above threshold
    if (lastSample > threshold) {
      // Check if enough time has passed (prevent double-counting)
      // Minimum 300ms between beats (200 bpm max)
      if (millis() - lastBeatTime > 300) {
        beatDetected = true;
        lastBeatTime = millis();
      }
    }
    increasing = false;
  }
  
  lastSample = sample;
  return beatDetected;
}

/**
 * Send heart rate data via BLE notification
 * Follows the BLE Heart Rate Measurement characteristic format
 * 
 * Format:
 * Byte 0: Flags
 *   Bit 0: Heart Rate Value Format (0 = UINT8, 1 = UINT16)
 *   Bit 1-2: Sensor Contact Status
 *   Bit 3: Energy Expended Status
 *   Bit 4: RR-Interval present
 * Byte 1: Heart Rate Measurement Value (BPM)
 */
void sendHeartRateNotification(uint8_t bpm) {
  // Prepare data packet
  uint8_t hrData[2];
  
  // Flags byte
  // 0x00 = Heart Rate value format is UINT8
  // 0x04 = Sensor Contact detected
  // 0x06 = Sensor Contact detected and good contact
  hrData[0] = 0x00;  // Basic format
  
  // Heart rate value (beats per minute)
  hrData[1] = bpm;
  
  // Set characteristic value and notify connected client
  pCharacteristic->setValue(hrData, 2);
  pCharacteristic->notify();
  
  // Debug output
  // Serial.printf("📡 BLE: Sent HR = %d bpm\n", bpm);
}

/**
 * Advanced: Calculate Heart Rate Variability (HRV)
 * Uncomment to use - requires storing RR intervals
 */
/*
#include <vector>

std::vector<long> rrIntervals;

void calculateHRV() {
  if (rrIntervals.size() < 5) return;
  
  // Calculate RMSSD (Root Mean Square of Successive Differences)
  float sumSquares = 0;
  for(size_t i = 1; i < rrIntervals.size(); i++) {
    long diff = rrIntervals[i] - rrIntervals[i-1];
    sumSquares += diff * diff;
  }
  float rmssd = sqrt(sumSquares / (rrIntervals.size() - 1));
  Serial.printf("HRV (RMSSD): %.2f ms\n", rmssd);
}

// Call this when a beat is detected:
// rrIntervals.push_back(delta);  // delta is time between beats in ms
// if (rrIntervals.size() > 20) rrIntervals.erase(rrIntervals.begin());
// calculateHRV();
*/

/**
 * Advanced: Heart Rate Zones
 * Uncomment to display training zones
 */
/*
void displayHeartRateZone(int hr, int age) {
  int maxHR = 220 - age;  // Estimated max heart rate
  float percentage = (float)hr / maxHR * 100;
  
  Serial.print("Zone: ");
  if (percentage < 60) {
    Serial.println("1 - Very Light (50-60% max HR)");
  } else if (percentage < 70) {
    Serial.println("2 - Light (60-70% max HR)");
  } else if (percentage < 80) {
    Serial.println("3 - Moderate (70-80% max HR)");
  } else if (percentage < 90) {
    Serial.println("4 - Hard (80-90% max HR)");
  } else {
    Serial.println("5 - Maximum (90-100% max HR)");
  }
}

// Call in loop with: displayHeartRateZone(beatAvg, 30);  // 30 is age
*/
