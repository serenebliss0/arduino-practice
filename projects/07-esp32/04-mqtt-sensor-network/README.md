# Project 4: MQTT Sensor Network 📡🌐

A complete IoT sensor network that publishes sensor data and receives control commands using the MQTT protocol, enabling real-time communication between devices and creating the foundation for smart home automation.

## 🎯 Project Overview

**Difficulty**: Intermediate  
**Time to Complete**: 2-3 hours  
**Real-World Applications**:
- Smart home sensor networks
- Industrial IoT monitoring systems
- Environmental monitoring stations
- Building automation systems
- Remote device control and monitoring
- Real-time data dashboards
- Agricultural monitoring systems
- Energy management systems

MQTT (Message Queuing Telemetry Transport) is a lightweight publish-subscribe messaging protocol ideal for IoT applications. This project demonstrates how to create a bidirectional IoT device that can both send sensor data and receive commands, forming the backbone of modern smart home systems.

## 📦 Components Needed

### Hardware
| Component | Quantity | Estimated Cost |
|-----------|----------|----------------|
| ESP32 Development Board | 1 | $8-12 |
| DHT22 Temperature/Humidity Sensor | 1 | $3-5 |
| PIR Motion Sensor (HC-SR501) | 1 | $2-4 |
| Active Buzzer (5V) | 1 | $1-2 |
| LED (any color) | 1 | $0.10 |
| 220Ω Resistor | 1 | $0.05 |
| 10kΩ Resistor (pull-up for DHT22) | 1 | $0.05 |
| Breadboard | 1 | $3-5 |
| Jumper Wires | 10-15 | $2-3 |
| USB Cable (Micro USB or USB-C) | 1 | $2-3 |

### Software
- Arduino IDE with ESP32 board support
- **PubSubClient** library by Nick O'Leary (MQTT client)
- **DHT sensor library** by Adafruit
- **Adafruit Unified Sensor** library (dependency)
- **ArduinoJson** library by Benoit Blanchon
- MQTT broker access (HiveMQ, Mosquitto, or cloud service)
- MQTT client for testing (MQTT Explorer, MQTT.fx, or mosquitto_sub/pub)

**Total Cost**: ~$25-40

### Optional MQTT Broker Options
1. **Public Brokers** (for testing):
   - broker.hivemq.com (no auth required)
   - test.mosquitto.org (no auth required)

2. **Self-Hosted**:
   - Mosquitto (free, open-source)
   - EMQX (feature-rich)

3. **Cloud Services**:
   - HiveMQ Cloud (free tier available)
   - CloudMQTT (managed service)
   - AWS IoT Core
   - Azure IoT Hub

## 🔌 Wiring Diagram

### Pin Connections

**DHT22 Temperature/Humidity Sensor:**
```
DHT22          ESP32
  VCC    →     3.3V
  DATA   →     GPIO 4
  NC     →     (not connected)
  GND    →     GND

Note: Add 10kΩ pull-up resistor between DATA and VCC
```

**PIR Motion Sensor:**
```
PIR Sensor     ESP32
  VCC    →     5V (or 3.3V depending on sensor)
  OUT    →     GPIO 5
  GND    →     GND
```

**LED Indicator:**
```
LED            ESP32
  Anode  (+)   →  GPIO 2 (through 220Ω resistor)
  Cathode (-)  →  GND
```

**Active Buzzer:**
```
Buzzer         ESP32
  +      →     GPIO 15
  -      →     GND
```

## 📋 Circuit Diagram

```
                                  ESP32
                    ┌──────────────────────────┐
                    │                          │
    DHT22           │                          │
   ┌─────┐          │                          │
   │ VCC ├──────────┤ 3.3V              GPIO 2 ├─────[220Ω]─────┐
   │     │   10kΩ   │                          │                │
   │DATA ├────/\/\──┤ GPIO 4                   │              [LED]
   │     │     └────┤ 3.3V                     │                │
   │ GND ├──────────┤ GND                      │              GND
   └─────┘          │                          │
                    │                          │
   PIR Sensor       │                          │
   ┌─────┐          │                          │
   │ VCC ├──────────┤ 5V               GPIO 15 ├────┐
   │ OUT ├──────────┤ GPIO 5                   │    │
   │ GND ├──────────┤ GND                      │  ┌───┐
   └─────┘          │                          │  │ ♪ │ Active Buzzer
                    │                          │  └─┬─┘
                    │                      USB │    │
                    │                          │  GND
                    └──────────────────────────┘
                              │
                              ▼
                         Computer
```

### Physical Layout on Breadboard

```
     Power Rails              ESP32                Components
   ─────────────────────────────────────────────────────────
   + (Red)                                        DHT22
   │                        ┌────┐               ┌─────┐
   ├────────────────────────┤3.3V│               │  1  │ VCC → +
   │                        │    │    10kΩ       │  2  │ DATA → GPIO4
   │              ┌─────────┤GPIO│────/\/\───────│     │  └─→ + (via 10kΩ)
   │              │         │ 2  ├─[220Ω]─LED+   │  3  │ NC
   │              │         │ 4  │               │  4  │ GND → -
   │              │         │ 5  ├───────────────┘     └
   │              │         │ 15 ├────────┐
   │              │         │    │      Buzzer
   │              │         │ 5V │        + -
   │              │         │GND │        │ │
   ├──────────────┼─────────┤    ├────────┘ │
   │              │         └────┘           │
   │              │                          │
   - (Black)      │                          │
   └──────────────┴──────────────────────────┘
   GND
   
   PIR Sensor
   ┌─────────┐
   │  VCC    ├─→ + (5V)
   │  OUT    ├─→ GPIO 5
   │  GND    ├─→ -
   └─────────┘
```

## 💻 Code

### Complete Sketch: `04-mqtt-sensor-network.ino`

The complete code is provided in the `.ino` file. Key sections include:

#### Configuration Section
```cpp
// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// MQTT Broker Settings
const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_Sensor_01";
```

#### MQTT Topics Structure
```cpp
// Publishing topics (sensor data)
home/sensor01/temperature
home/sensor01/humidity
home/sensor01/motion
home/sensor01/status

// Subscribing topics (control commands)
home/sensor01/led
home/sensor01/buzzer
```

## 🚀 How to Use

### 1. Library Installation

Open Arduino IDE and install required libraries:
1. Go to **Sketch → Include Library → Manage Libraries**
2. Search and install:
   - "PubSubClient" by Nick O'Leary
   - "DHT sensor library" by Adafruit
   - "Adafruit Unified Sensor"
   - "ArduinoJson" by Benoit Blanchon

### 2. Hardware Setup

1. Assemble the circuit according to the wiring diagram
2. Double-check all connections, especially power (3.3V vs 5V)
3. Ensure the DHT22 has a 10kΩ pull-up resistor on the DATA line
4. Connect ESP32 to computer via USB

### 3. Configure the Code

Edit these lines in the sketch:
```cpp
const char* WIFI_SSID = "YourNetworkName";
const char* WIFI_PASSWORD = "YourPassword";
const char* MQTT_BROKER = "broker.hivemq.com";  // Or your broker
const char* MQTT_CLIENT_ID = "ESP32_Sensor_01";  // Make it unique!
```

**Important**: Each device needs a unique `MQTT_CLIENT_ID` to avoid conflicts!

### 4. Upload and Monitor

1. Select **Tools → Board → ESP32 Dev Module**
2. Select correct **Port**
3. Click **Upload** (→)
4. Open **Serial Monitor** (set to 115200 baud)
5. Wait for "MQTT connected" message

### 5. Testing with MQTT Client

**Option A: Using MQTT Explorer** (Recommended for beginners)
1. Download MQTT Explorer from [mqtt-explorer.com](http://mqtt-explorer.com/)
2. Connect to your broker (e.g., broker.hivemq.com, port 1883)
3. You should see messages appearing under `home/sensor01/`
4. To control devices:
   - Publish "on" or "off" to `home/sensor01/led`
   - Publish "beep" to `home/sensor01/buzzer`

**Option B: Using Mosquitto Command Line**
```bash
# Subscribe to all sensor topics
mosquitto_sub -h broker.hivemq.com -t "home/sensor01/#" -v

# Control LED (in another terminal)
mosquitto_pub -h broker.hivemq.com -t "home/sensor01/led" -m "on"
mosquitto_pub -h broker.hivemq.com -t "home/sensor01/led" -m "off"

# Trigger buzzer
mosquitto_pub -h broker.hivemq.com -t "home/sensor01/buzzer" -m "beep"
```

**Option C: Using Node-RED** (For visual dashboards)
1. Install Node-RED
2. Add MQTT input/output nodes
3. Create dashboard with gauges and switches

## 📊 Example Output

### Serial Monitor Output
```
═══════════════════════════════════════
     ESP32 MQTT Sensor Network
═══════════════════════════════════════

✓ DHT sensor initialized
Connecting to WiFi: MyHomeNetwork
......
✓ WiFi connected!
IP Address: 192.168.1.45
Signal Strength: -52 dBm
✓ MQTT client configured

Attempting MQTT connection to broker.hivemq.com... Connected!
✓ Subscribed to control topics

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Reading sensors...
Temperature: 23.4°C
Humidity: 45.2%
✓ Temperature published
✓ Humidity published
✓ Data published successfully
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚶 Motion detected!
✓ Motion status published

Message arrived [home/sensor01/led]: on
✓ LED turned ON

Message arrived [home/sensor01/buzzer]: beep
✓ Buzzer beeped
```

### MQTT Message Examples

**Temperature Message:**
```json
{
  "sensor": "DHT22",
  "type": "temperature",
  "value": 23.4,
  "unit": "celsius",
  "timestamp": 125430
}
```

**Motion Detection Message:**
```json
{
  "sensor": "PIR",
  "type": "motion",
  "detected": true,
  "timestamp": 125430
}
```

**Status Message:**
```json
{
  "client_id": "ESP32_Sensor_01",
  "status": "online",
  "ip": "192.168.1.45",
  "rssi": -52,
  "timestamp": 125430
}
```

## 🎓 Understanding the Code

### MQTT Basics

**What is MQTT?**

MQTT (Message Queuing Telemetry Transport) is a lightweight, publish-subscribe network protocol designed for IoT devices with limited resources. Created by IBM in 1999, it has become the de facto standard for IoT communication due to its efficiency, reliability, and simplicity.

**Why MQTT for IoT?**
- **Lightweight**: Minimal packet overhead (as small as 2 bytes)
- **Efficient**: Low bandwidth consumption, ideal for mobile networks
- **Reliable**: Three levels of Quality of Service (QoS)
- **Bi-directional**: Both publish and subscribe from the same device
- **Persistent sessions**: Resume communication after disconnection
- **Last Will and Testament**: Automatic notification when device goes offline

**Publish-Subscribe Pattern:**
- **Publishers** send messages to topics
- **Subscribers** receive messages from topics they're interested in
- **Broker** routes messages between publishers and subscribers
- **Topics** organize messages hierarchically (e.g., `home/livingroom/temperature`)

```
Publisher (ESP32)  →  [Topic: home/temp]  →  Broker  →  Subscriber (App)
```

**MQTT vs HTTP:**
- MQTT uses persistent connections (HTTP creates new connection per request)
- MQTT is event-driven (HTTP is request/response)
- MQTT has smaller message overhead (2-20 bytes vs 100+ bytes for HTTP)
- MQTT supports QoS levels (HTTP doesn't)
- MQTT is ideal for many-to-many communication

**Topic Hierarchy Best Practices:**
```
home/                          # Root level
  ├── livingroom/              # Location
  │   ├── sensor01/            # Device
  │   │   ├── temperature      # Sensor type
  │   │   ├── humidity
  │   │   └── motion
  │   └── lights/              # Another device
  │       ├── power
  │       └── brightness
  └── bedroom/
      └── sensor02/
          ├── temperature
          └── humidity
```

### Quality of Service (QoS) Levels

MQTT provides three levels of Quality of Service to balance between reliability and performance:

**QoS 0 - At Most Once (Fire and Forget):**
- Message sent once, no acknowledgment required
- Fastest but least reliable
- Message may be lost if connection drops
- Best for: Non-critical data, high-frequency updates (temperature every second)
```cpp
mqttClient.publish(topic, message, false);  // No guarantee
```

**QoS 1 - At Least Once (Acknowledged Delivery):**
- Message sent and acknowledgment (PUBACK) required
- May receive duplicates if ACK is lost
- Guaranteed delivery but not guaranteed uniqueness
- Best for: Important data that can tolerate duplicates (motion alerts)
```cpp
mqttClient.publish(topic, message, true);  // Guaranteed delivery
```

**QoS 2 - Exactly Once (Four-Way Handshake):**
- Most reliable but slowest
- Four-step handshake: PUBLISH → PUBREC → PUBREL → PUBCOMP
- No duplicates, guaranteed delivery
- Best for: Critical commands (door unlock, payment transactions)
- **Note**: Not supported by PubSubClient library

**Choosing the Right QoS:**
- Temperature/humidity readings: QoS 0 (frequent, non-critical)
- Motion detection alerts: QoS 1 (important but duplicates OK)
- Control commands: QoS 1 or higher (must be received)
- Financial transactions: QoS 2 (if supported)

### Key Functions Explained

**1. MQTT Connection:**
```cpp
bool reconnectMQTT() {
  // Try to connect to broker
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    // Subscribe to topics after connection
    mqttClient.subscribe(TOPIC_LED_CONTROL, 1);  // QoS 1
    return true;
  }
  return false;
}
```

**2. Message Callback:**
```cpp
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Called when message arrives on subscribed topic
  // Process command and take action
}
```

**3. Publishing with JSON:**
```cpp
StaticJsonDocument<200> doc;
doc["temperature"] = temp;
char buffer[200];
serializeJson(doc, buffer);
mqttClient.publish(TOPIC_TEMPERATURE, buffer);
```

**4. Auto-Reconnect Logic:**
```cpp
if (!mqttClient.connected()) {
  if (currentTime - lastReconnectAttempt >= RECONNECT_INTERVAL) {
    reconnectMQTT();
  }
} else {
  mqttClient.loop();  // Must call regularly to process messages
}
```

**Why `mqttClient.loop()` is Critical:**
- Processes incoming messages
- Maintains connection with broker
- Sends keep-alive packets
- Must be called frequently (at least every few seconds)
- Should be called in every iteration of `loop()`

**Connection State Management:**
The ESP32 maintains a state machine for MQTT connection:
1. **Disconnected**: Initial state, no connection attempt
2. **Connecting**: Attempting to connect to broker
3. **Connected**: Active connection, can publish/subscribe
4. **Disconnecting**: Gracefully closing connection

**Reconnection Strategy:**
- Exponential backoff: Increase delay between retries
- Avoid rapid reconnection attempts (can overload broker)
- Resubscribe to all topics after reconnection
- Publish "online" status message upon reconnection

### DHT22 Sensor Reading

```cpp
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();

if (isnan(temperature) || isnan(humidity)) {
  // Reading failed - sensor error
}
```

### Motion Detection

```cpp
bool currentMotionState = digitalRead(PIR_PIN);
if (currentMotionState != lastMotionState) {
  // State changed - motion detected or ended
  publishMotion(currentMotionState);
}
```

## 🔧 Customization Ideas

### 1. Add More Sensors
```cpp
// Add soil moisture sensor
#define SOIL_PIN 34
const char* TOPIC_SOIL = "home/sensor01/soil";

void readSoilMoisture() {
  int moisture = analogRead(SOIL_PIN);
  int percentage = map(moisture, 0, 4095, 0, 100);
  publishSoilMoisture(percentage);
}
```

### 2. Implement Rules Engine
```cpp
// Auto-control based on sensor values
if (temperature > 28.0) {
  // Publish command to turn on fan
  mqttClient.publish("home/fan/control", "on");
}

if (motion && nightTime) {
  // Turn on lights automatically
  mqttClient.publish("home/lights/control", "on");
}
```

### 3. Add TLS/SSL Security
```cpp
#include <WiFiClientSecure.h>

WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

void setup() {
  // Load CA certificate
  secureClient.setCACert(root_ca);
  mqttClient.setServer(MQTT_BROKER, 8883);  // Secure port
}
```

### 4. Create Device Groups
```cpp
// Multi-level topic structure
// home/room/device/sensor
const char* TOPIC_PREFIX = "home/livingroom/sensor01/";

String getTopic(const char* suffix) {
  return String(TOPIC_PREFIX) + suffix;
}
```

### 5. Add Scheduling
```cpp
// Only publish during specific hours
if (hour >= 8 && hour <= 22) {
  readAndPublishSensors();
}
```

### 6. Implement Discovery Protocol
```cpp
// Home Assistant MQTT Discovery
void publishDiscovery() {
  const char* discoveryTopic = 
    "homeassistant/sensor/sensor01/config";
  
  StaticJsonDocument<512> doc;
  doc["name"] = "Living Room Temperature";
  doc["state_topic"] = "home/sensor01/temperature";
  doc["unit_of_measurement"] = "°C";
  doc["device_class"] = "temperature";
  
  char buffer[512];
  serializeJson(doc, buffer);
  mqttClient.publish(discoveryTopic, buffer, true);
}
```

### 7. Add Local Storage
```cpp
#include <SPIFFS.h>

// Store failed messages when offline
void storeMessage(const char* topic, const char* message) {
  File file = SPIFFS.open("/queue.txt", FILE_APPEND);
  file.println(topic);
  file.println(message);
  file.close();
}

// Resend when reconnected
void processQueue() {
  // Read and publish stored messages
}
```

### 8. Battery Monitoring
```cpp
void publishBatteryStatus() {
  int batteryLevel = analogRead(BATTERY_PIN);
  float voltage = batteryLevel * (3.3 / 4095.0) * 2;  // Voltage divider
  
  StaticJsonDocument<200> doc;
  doc["voltage"] = voltage;
  doc["percentage"] = map(voltage, 3.0, 4.2, 0, 100);
  
  char buffer[200];
  serializeJson(doc, buffer);
  mqttClient.publish("home/sensor01/battery", buffer);
}
```

## 🐛 Troubleshooting

### Connection Issues

**Cannot connect to WiFi:**
- Verify SSID and password are correct
- Check WiFi signal strength (should be > -70 dBm)
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Try moving ESP32 closer to router
- Check if MAC filtering is enabled on router

**Cannot connect to MQTT broker:**
- Verify broker address and port (usually 1883)
- Check if broker requires authentication
- Ensure client ID is unique
- Test broker connectivity with MQTT Explorer
- Check firewall settings
- Verify broker is running (if self-hosted)

**Connection drops frequently:**
- Increase keep-alive interval:
  ```cpp
  mqttClient.setKeepAlive(60);  // 60 seconds
  ```
- Check WiFi stability
- Increase socket timeout:
  ```cpp
  mqttClient.setSocketTimeout(30);
  ```
- Ensure adequate power supply

### Sensor Issues

**DHT22 returns NaN (Not a Number):**
- Check wiring, especially the data pin
- Verify 10kΩ pull-up resistor is present
- Add delay between readings (minimum 2 seconds)
- Check power supply (needs stable 3.3V)
- Try a different GPIO pin
- Sensor may be faulty - test with multimeter

**PIR sensor constantly triggers:**
- Adjust sensitivity potentiometer on sensor
- Adjust time delay potentiometer
- Move away from heat sources
- Check if trigger mode is set correctly (H or L)
- Add delay to debounce:
  ```cpp
  if (motionDetected) {
    delay(2000);  // Ignore for 2 seconds
  }
  ```

**No sensor readings appear:**
- Check Serial Monitor baud rate (115200)
- Verify sensors are powered
- Check all GPIO connections
- Add debug prints:
  ```cpp
  Serial.printf("Temperature: %.1f\n", temperature);
  ```

### MQTT Message Issues

**Messages not received by subscribers:**
- Verify topic names match exactly (case-sensitive)
- Check QoS level
- Ensure subscriber is connected
- Check broker logs
- Test with MQTT Explorer

**Messages not published:**
- Check mqttClient.connected() status
- Verify return value of publish():
  ```cpp
  bool success = mqttClient.publish(topic, message);
  Serial.printf("Publish %s\n", success ? "OK" : "FAILED");
  ```
- Check message buffer size
- Ensure mqttClient.loop() is called regularly

**JSON parsing errors:**
- Increase JSON document size:
  ```cpp
  StaticJsonDocument<512> doc;  // Increase if needed
  ```
- Validate JSON with online tool
- Check for special characters

### Memory Issues

**ESP32 crashes or resets:**
- Monitor free heap:
  ```cpp
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  ```
- Reduce JSON buffer sizes
- Call WiFi.scanDelete() after scans
- Avoid large String operations
- Use const char* instead of String where possible

**Out of memory errors:**
- Reduce message frequency
- Decrease MQTT buffer size
- Free unused objects
- Use PROGMEM for large strings:
  ```cpp
  const char msg[] PROGMEM = "Large message";
  ```

### Programming Issues

**Upload fails:**
- Press and hold BOOT button during upload
- Check correct port is selected
- Reduce upload speed (Tools → Upload Speed → 115200)
- Check USB cable (must support data)
- Try different USB port

**Code won't compile:**
- Install all required libraries
- Check library versions compatibility
- Close and reopen Arduino IDE
- Clear cache: Delete Arduino15/packages/esp32

## 📚 Learning Outcomes

After completing this project, you'll understand:
- ✅ MQTT protocol and publish-subscribe architecture
- ✅ IoT communication patterns and best practices
- ✅ Bidirectional device communication
- ✅ JSON message formatting and parsing
- ✅ Multiple sensor integration
- ✅ Asynchronous event handling
- ✅ WiFi connection management
- ✅ Quality of Service (QoS) levels
- ✅ Topic naming conventions
- ✅ Device state management
- ✅ Error handling and auto-reconnection
- ✅ Real-time data publishing

## 🚀 Next Steps

### Beginner Level
1. **Add More Sensors**: Integrate light sensor (LDR) or ultrasonic distance sensor
2. **Create Dashboard**: Use Node-RED to create visual dashboard
3. **Email Alerts**: Send email when motion detected (see Project 05)
4. **Data Logging**: Log sensor data to SD card

### Intermediate Level
5. **Home Assistant Integration**: Connect to Home Assistant for automation
6. **Multiple Devices**: Deploy several ESP32 sensors in different rooms
7. **Rules Engine**: Implement automated responses based on sensor data
8. **Voice Control**: Integrate with Google Assistant or Alexa
9. **Mobile App**: Create smartphone app to monitor sensors

### Advanced Level
10. **TLS Encryption**: Add secure MQTT communication
11. **OTA Updates**: Add over-the-air firmware updates (Project 06)
12. **Mesh Network**: Create ESP-NOW mesh for multiple devices
13. **Database Integration**: Store data in InfluxDB or MongoDB
14. **Machine Learning**: Add TensorFlow Lite for anomaly detection
15. **Custom Protocol**: Build your own IoT protocol on top of MQTT

### Real-World Projects
- **Smart Garden**: Automated watering based on soil moisture
- **Energy Monitor**: Track and optimize power consumption
- **Pet Feeder**: Automated feeding with portion control
- **Air Quality Monitor**: Track PM2.5, CO2, and VOCs
- **Smart Aquarium**: Monitor and control temperature, pH, lighting

## 📖 References

### MQTT Protocol
- [MQTT.org Official Specification](https://mqtt.org/)
- [HiveMQ MQTT Essentials](https://www.hivemq.com/mqtt-essentials/)
- [MQTT Quality of Service Levels](https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/)

### Libraries Documentation
- [PubSubClient Library](https://pubsubclient.knolleary.net/)
- [Adafruit DHT Library](https://github.com/adafruit/DHT-sensor-library)
- [ArduinoJson Documentation](https://arduinojson.org/)

### MQTT Brokers
- [Mosquitto Setup Guide](https://mosquitto.org/)
- [HiveMQ Cloud Free Tier](https://www.hivemq.com/mqtt-cloud-broker/)
- [EMQX Documentation](https://www.emqx.io/docs/en/latest/)

### Testing Tools
- [MQTT Explorer Download](http://mqtt-explorer.com/)
- [MQTT.fx](https://mqttfx.jensd.de/)
- [Mosquitto Command Line Tools](https://mosquitto.org/man/mosquitto_pub-1.html)

### Smart Home Integration
- [Home Assistant MQTT Integration](https://www.home-assistant.io/integrations/mqtt/)
- [Node-RED Getting Started](https://nodered.org/docs/getting-started/)
- [OpenHAB MQTT Binding](https://www.openhab.org/addons/bindings/mqtt/)

### ESP32 Resources
- [ESP32 MQTT Examples](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples)
- [ESP32 WiFi Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)

### IoT Best Practices
- [AWS IoT Best Practices](https://docs.aws.amazon.com/iot/latest/developerguide/iot-best-practices.html)
- [OWASP IoT Top 10](https://owasp.org/www-project-internet-of-things/)

---

**Project Complete!** 🎉

You've built a professional IoT sensor network using MQTT. This is the foundation for countless smart home and industrial IoT applications. The skills learned here - publish-subscribe messaging, JSON data exchange, and bidirectional device communication - are essential for modern IoT development!
