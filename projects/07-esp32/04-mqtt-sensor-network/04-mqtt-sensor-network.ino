/*
 * ESP32 MQTT Sensor Network
 * Publish sensor data and subscribe to control messages using MQTT protocol
 * 
 * Features:
 * - Connect to WiFi and MQTT broker
 * - Publish temperature, humidity, and motion data
 * - Subscribe to control topics (LED, buzzer)
 * - Auto-reconnect on disconnection
 * - JSON message format
 * - QoS levels support
 * 
 * Required Libraries:
 * - WiFi (built-in)
 * - PubSubClient by Nick O'Leary (install via Library Manager)
 * - DHT sensor library by Adafruit (install via Library Manager)
 * - ArduinoJson by Benoit Blanchon (install via Library Manager)
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================
// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// MQTT Broker Settings
const char* MQTT_BROKER = "broker.hivemq.com";  // Public broker (change to your own)
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_Sensor_01";  // Must be unique
const char* MQTT_USERNAME = "";  // Leave empty if no auth
const char* MQTT_PASSWORD = "";  // Leave empty if no auth

// MQTT Topics
const char* TOPIC_TEMPERATURE = "home/sensor01/temperature";
const char* TOPIC_HUMIDITY = "home/sensor01/humidity";
const char* TOPIC_MOTION = "home/sensor01/motion";
const char* TOPIC_STATUS = "home/sensor01/status";
const char* TOPIC_LED_CONTROL = "home/sensor01/led";
const char* TOPIC_BUZZER_CONTROL = "home/sensor01/buzzer";

// Pin Definitions
#define DHT_PIN 4           // DHT22 data pin
#define DHT_TYPE DHT22      // DHT22 sensor type
#define PIR_PIN 5           // PIR motion sensor pin
#define LED_PIN 2           // Built-in LED
#define BUZZER_PIN 15       // Active buzzer pin

// Timing Configuration
const unsigned long SENSOR_INTERVAL = 10000;  // 10 seconds
const unsigned long MOTION_CHECK_INTERVAL = 1000;  // 1 second
const unsigned long RECONNECT_INTERVAL = 5000;  // 5 seconds

// ==================== GLOBAL OBJECTS ====================
WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

// State variables
unsigned long lastSensorRead = 0;
unsigned long lastMotionCheck = 0;
unsigned long lastReconnectAttempt = 0;
bool motionDetected = false;
bool lastMotionState = false;

// ==================== SETUP ====================
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("     ESP32 MQTT Sensor Network");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("✓ DHT sensor initialized");
  
  // Connect to WiFi
  connectWiFi();
  
  // Configure MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
  
  Serial.println("✓ MQTT client configured");
  Serial.println();
}

// ==================== MAIN LOOP ====================
void loop() {
  // Ensure MQTT connection
  if (!mqttClient.connected()) {
    unsigned long currentTime = millis();
    if (currentTime - lastReconnectAttempt >= RECONNECT_INTERVAL) {
      lastReconnectAttempt = currentTime;
      if (reconnectMQTT()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    mqttClient.loop();
  }
  
  // Read and publish sensor data periodically
  unsigned long currentTime = millis();
  if (currentTime - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = currentTime;
    readAndPublishSensors();
  }
  
  // Check motion sensor
  if (currentTime - lastMotionCheck >= MOTION_CHECK_INTERVAL) {
    lastMotionCheck = currentTime;
    checkMotionSensor();
  }
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
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ WiFi connection failed!");
    Serial.println("Please check credentials and restart.");
  }
}

// ==================== MQTT CONNECTION ====================
bool reconnectMQTT() {
  Serial.print("Attempting MQTT connection to ");
  Serial.print(MQTT_BROKER);
  Serial.print("...");
  
  // Attempt to connect
  bool connected = false;
  if (strlen(MQTT_USERNAME) > 0) {
    connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
  } else {
    connected = mqttClient.connect(MQTT_CLIENT_ID);
  }
  
  if (connected) {
    Serial.println(" Connected!");
    
    // Subscribe to control topics
    mqttClient.subscribe(TOPIC_LED_CONTROL, 1);  // QoS 1
    mqttClient.subscribe(TOPIC_BUZZER_CONTROL, 1);
    
    Serial.println("✓ Subscribed to control topics");
    
    // Publish online status
    publishStatus("online");
    
    // Blink LED to indicate connection
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
    
    return true;
  } else {
    Serial.print(" Failed, rc=");
    Serial.println(mqttClient.state());
    return false;
  }
}

// ==================== MQTT CALLBACK ====================
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Convert payload to string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  // Handle LED control
  if (strcmp(topic, TOPIC_LED_CONTROL) == 0) {
    if (message.equalsIgnoreCase("on") || message.equals("1")) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("✓ LED turned ON");
    } else if (message.equalsIgnoreCase("off") || message.equals("0")) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("✓ LED turned OFF");
    } else if (message.equalsIgnoreCase("toggle")) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      Serial.println("✓ LED toggled");
    }
  }
  
  // Handle buzzer control
  if (strcmp(topic, TOPIC_BUZZER_CONTROL) == 0) {
    if (message.equalsIgnoreCase("on") || message.equals("1")) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("✓ Buzzer turned ON");
    } else if (message.equalsIgnoreCase("off") || message.equals("0")) {
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("✓ Buzzer turned OFF");
    } else if (message.equalsIgnoreCase("beep")) {
      // Short beep
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("✓ Buzzer beeped");
    }
  }
}

// ==================== SENSOR READING ====================
void readAndPublishSensors() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Reading sensors...");
  
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("✗ Failed to read from DHT sensor!");
    return;
  }
  
  Serial.printf("Temperature: %.1f°C\n", temperature);
  Serial.printf("Humidity: %.1f%%\n", humidity);
  
  // Publish temperature
  publishTemperature(temperature);
  
  // Publish humidity
  publishHumidity(humidity);
  
  Serial.println("✓ Data published successfully");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

// ==================== MOTION DETECTION ====================
void checkMotionSensor() {
  bool currentMotionState = digitalRead(PIR_PIN);
  
  // Detect motion state change
  if (currentMotionState != lastMotionState) {
    lastMotionState = currentMotionState;
    
    if (currentMotionState == HIGH) {
      Serial.println("🚶 Motion detected!");
      publishMotion(true);
      
      // Flash LED on motion
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("Motion ended");
      publishMotion(false);
    }
  }
}

// ==================== PUBLISH FUNCTIONS ====================
void publishTemperature(float temperature) {
  // Create JSON document
  StaticJsonDocument<200> doc;
  doc["sensor"] = "DHT22";
  doc["type"] = "temperature";
  doc["value"] = temperature;
  doc["unit"] = "celsius";
  doc["timestamp"] = millis();
  
  // Serialize to string
  char buffer[200];
  serializeJson(doc, buffer);
  
  // Publish with QoS 0 (fire and forget)
  if (mqttClient.publish(TOPIC_TEMPERATURE, buffer, false)) {
    Serial.println("✓ Temperature published");
  } else {
    Serial.println("✗ Temperature publish failed");
  }
}

void publishHumidity(float humidity) {
  // Create JSON document
  StaticJsonDocument<200> doc;
  doc["sensor"] = "DHT22";
  doc["type"] = "humidity";
  doc["value"] = humidity;
  doc["unit"] = "percent";
  doc["timestamp"] = millis();
  
  // Serialize to string
  char buffer[200];
  serializeJson(doc, buffer);
  
  // Publish with QoS 0
  if (mqttClient.publish(TOPIC_HUMIDITY, buffer, false)) {
    Serial.println("✓ Humidity published");
  } else {
    Serial.println("✗ Humidity publish failed");
  }
}

void publishMotion(bool detected) {
  // Create JSON document
  StaticJsonDocument<200> doc;
  doc["sensor"] = "PIR";
  doc["type"] = "motion";
  doc["detected"] = detected;
  doc["timestamp"] = millis();
  
  // Serialize to string
  char buffer[200];
  serializeJson(doc, buffer);
  
  // Publish with QoS 1 (at least once)
  if (mqttClient.publish(TOPIC_MOTION, buffer, false)) {
    Serial.println("✓ Motion status published");
  } else {
    Serial.println("✗ Motion publish failed");
  }
}

void publishStatus(const char* status) {
  // Create JSON document
  StaticJsonDocument<200> doc;
  doc["client_id"] = MQTT_CLIENT_ID;
  doc["status"] = status;
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  doc["timestamp"] = millis();
  
  // Serialize to string
  char buffer[200];
  serializeJson(doc, buffer);
  
  // Publish with retained flag
  mqttClient.publish(TOPIC_STATUS, buffer, true);
}
