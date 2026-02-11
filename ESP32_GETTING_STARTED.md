# Getting Started with ESP32 🚀

Welcome to ESP32 development! The ESP32 is a powerful, low-cost microcontroller with built-in **WiFi**, **Bluetooth**, and excellent processing power. It's perfect for IoT projects, smart home devices, and wireless applications.

## 📋 Table of Contents
- [What is ESP32?](#what-is-esp32)
- [ESP32 vs Arduino](#esp32-vs-arduino)
- [Hardware Setup](#hardware-setup)
- [Software Installation](#software-installation)
- [First Program](#first-program)
- [WiFi Setup](#wifi-setup)
- [Bluetooth Setup](#bluetooth-setup)
- [Pinout and Specifications](#pinout-and-specifications)
- [Troubleshooting](#troubleshooting)

## 🎯 What is ESP32?

The ESP32 is a series of low-cost, low-power system-on-chip microcontrollers with integrated WiFi and Bluetooth capabilities.

### Key Features
- **Dual-core** 240 MHz processor (Xtensa LX6)
- **Built-in WiFi** (802.11 b/g/n)
- **Bluetooth** v4.2 BR/EDR and BLE
- **34 GPIO pins** with various functions
- **12-bit ADC** (18 channels)
- **2 8-bit DAC** channels
- **PWM** on all pins
- **SPI, I2C, I2S, UART** support
- **Touch sensors** (10 capacitive touch pins)
- **512 KB RAM** + 4 MB Flash (typical)
- **Low power consumption** with deep sleep mode

### Why Choose ESP32?
- ✅ **Wireless Connectivity**: Built-in WiFi and Bluetooth
- ✅ **Powerful**: Much faster than Arduino Uno
- ✅ **IoT Ready**: Perfect for Internet of Things projects
- ✅ **Cost-Effective**: $5-15 for dev boards
- ✅ **Arduino Compatible**: Use Arduino IDE and libraries
- ✅ **Large Community**: Extensive documentation and support

## 🆚 ESP32 vs Arduino

| Feature | Arduino Uno | ESP32 |
|---------|-------------|-------|
| Processor | 8-bit 16 MHz | 32-bit dual-core 240 MHz |
| RAM | 2 KB | 512 KB |
| Flash | 32 KB | 4 MB (typical) |
| WiFi | ❌ (requires module) | ✅ Built-in |
| Bluetooth | ❌ (requires module) | ✅ Built-in |
| GPIO Pins | 14 digital, 6 analog | 34 GPIO (multi-function) |
| Operating Voltage | 5V | 3.3V |
| Best For | Basic projects, learning | IoT, wireless projects |

**Important**: ESP32 operates at **3.3V** logic level. Use level shifters for 5V components!

## 🔧 Hardware Setup

### What You Need

#### Essential Hardware
1. **ESP32 Development Board**
   - Recommended: ESP32 DevKit v1 or DOIT DevKit
   - Other options: ESP32-CAM, ESP32-WROOM, NodeMCU-32S
2. **USB Cable**: Micro USB or USB-C (depending on board)
3. **Computer**: Windows, Mac, or Linux
4. **Breadboard and Jumper Wires**: For prototyping

#### Optional but Useful
- **External Antenna**: For better WiFi range (some boards)
- **Battery/Power Supply**: 3.3V or 5V (via USB)
- **Level Shifters**: If using 5V sensors/devices
- **LEDs and Resistors**: For testing

### Popular ESP32 Boards

#### 1. ESP32 DevKit v1
- Most common and beginner-friendly
- 30 GPIO pins exposed
- Built-in USB-to-serial converter
- Reset and boot buttons

#### 2. ESP32-WROOM-32
- Standard ESP32 module
- Requires external USB-serial converter
- Compact design

#### 3. ESP32-CAM
- Includes camera module
- MicroSD card slot
- Ideal for vision projects

#### 4. NodeMCU-32S
- NodeMCU form factor
- Easy breadboard compatibility
- Built-in programmer

## 💻 Software Installation

### Step 1: Install Arduino IDE

If you haven't already:
1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install version 1.8.x or 2.x
3. Launch Arduino IDE

### Step 2: Add ESP32 Board Support

#### Method 1: Using Board Manager (Recommended)

1. **Open Preferences**
   - File → Preferences (Windows/Linux)
   - Arduino IDE → Preferences (Mac)

2. **Add ESP32 Board Manager URL**
   - Find "Additional Board Manager URLs" field
   - Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - If you have other URLs, separate with commas or use the window icon to add on new line

3. **Install ESP32 Board Package**
   - Go to Tools → Board → Boards Manager
   - Search for "esp32"
   - Click **Install** on "esp32 by Espressif Systems"
   - Wait for installation (may take several minutes)

4. **Verify Installation**
   - Go to Tools → Board
   - You should see ESP32 boards under "ESP32 Arduino"

#### Method 2: Manual Installation (Advanced)

```bash
# Clone ESP32 Arduino repository
cd ~/Documents/Arduino/hardware
mkdir -p espressif
cd espressif
git clone https://github.com/espressif/arduino-esp32.git esp32
cd esp32
git submodule update --init --recursive
cd tools
python3 get.py
```

### Step 3: Select Your Board

1. **Connect ESP32** via USB cable
2. **Select Board Type**:
   - Tools → Board → ESP32 Arduino → Select your board
   - Common: "ESP32 Dev Module" or "DOIT ESP32 DEVKIT V1"
3. **Select Upload Speed**: 115200 (default) or 921600 (faster)
4. **Select Port**: 
   - Windows: COMx
   - Mac: /dev/cu.usbserial-xxx
   - Linux: /dev/ttyUSB0 or /dev/ttyACM0

### Step 4: Install USB Drivers (If Needed)

Most ESP32 boards use one of these USB-to-serial chips:

#### CP2102 Driver
- Download from [Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- Most common on ESP32 DevKit boards

#### CH340 Driver
- Download from [manufacturer](http://www.wch-ic.com/downloads/CH341SER_EXE.html)
- Common on cheaper boards

**How to check**: Look at the small chip near the USB port on your board.

## 💡 First Program: Blink LED

Let's test your ESP32 setup!

### Step 1: Create New Sketch

```cpp
// ESP32 Blink Example
// Built-in LED is usually on GPIO 2

#define LED_PIN 2  // Built-in LED pin

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("ESP32 Blink Test");
  
  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn LED on
  Serial.println("LED ON");
  delay(1000);                   // Wait 1 second
  
  digitalWrite(LED_PIN, LOW);    // Turn LED off
  Serial.println("LED OFF");
  delay(1000);                   // Wait 1 second
}
```

### Step 2: Upload Code

1. Click **Verify** (✓) to compile
2. Click **Upload** (→) to upload to ESP32
3. Watch for "Connecting..." message
4. **Hold BOOT button** if upload fails to start
5. Wait for "Done uploading"
6. The built-in LED should blink!

### Step 3: View Serial Output

1. Open Serial Monitor (Tools → Serial Monitor)
2. Set baud rate to **115200**
3. You should see "LED ON" and "LED OFF" messages

**Note**: If nothing happens, try GPIO 2, GPIO 5, or check your board documentation for built-in LED pin.

## 📶 WiFi Setup

ESP32's WiFi capabilities are one of its strongest features!

### Basic WiFi Connection

```cpp
#include <WiFi.h>

const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Your code here
  delay(1000);
}
```

### WiFi Modes

#### 1. Station Mode (STA)
- ESP32 connects to existing WiFi network
- Gets IP from router
- **Use case**: Connect to internet, home network

```cpp
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
```

#### 2. Access Point Mode (AP)
- ESP32 creates its own WiFi network
- Other devices connect to ESP32
- **Use case**: Direct device control, setup mode

```cpp
WiFi.mode(WIFI_AP);
WiFi.softAP("ESP32-AP", "password123");
IPAddress IP = WiFi.softAPIP();
Serial.println(IP);  // Usually 192.168.4.1
```

#### 3. Both Modes (AP+STA)
- ESP32 creates AP while connected to network
- **Use case**: WiFi bridge, repeater

```cpp
WiFi.mode(WIFI_AP_STA);
WiFi.begin(ssid, password);
WiFi.softAP("ESP32-AP", "password123");
```

### WiFi Functions Reference

```cpp
// Connection
WiFi.begin(ssid, password);           // Connect to network
WiFi.disconnect();                     // Disconnect
WiFi.status();                         // Get connection status

// Info
WiFi.localIP();                        // Get IP address
WiFi.macAddress();                     // Get MAC address
WiFi.SSID();                           // Get connected network name
WiFi.RSSI();                           // Get signal strength

// Scanning
int n = WiFi.scanNetworks();           // Scan for networks
String ssid = WiFi.SSID(i);           // Get network name
int rssi = WiFi.RSSI(i);              // Get signal strength
```

## 📱 Bluetooth Setup

ESP32 supports both Classic Bluetooth and BLE (Bluetooth Low Energy).

### Bluetooth Classic (Serial)

Simple serial communication like HC-05:

```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-BT");  // Bluetooth device name
  Serial.println("Bluetooth Started! Pair with 'ESP32-BT'");
}

void loop() {
  // Read from Bluetooth
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c);
  }
  
  // Send to Bluetooth
  if (Serial.available()) {
    char c = Serial.read();
    SerialBT.write(c);
  }
}
```

### BLE (Bluetooth Low Energy)

More power-efficient, better for battery projects:

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  
  // Initialize BLE
  BLEDevice::init("ESP32-BLE");
  
  // Create server
  BLEServer *pServer = BLEDevice::createServer();
  
  // Create service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create characteristic
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  
  pCharacteristic->setValue("Hello BLE");
  pService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  
  Serial.println("BLE Server Started!");
}

void loop() {
  delay(2000);
}
```

### Bluetooth Apps for Testing

- **Android**: 
  - "Serial Bluetooth Terminal" (Classic)
  - "nRF Connect" (BLE)
- **iOS**: 
  - "LightBlue" (BLE)
  - "BLE Scanner" (BLE)

## 📐 Pinout and Specifications

### ESP32 DevKit v1 Pinout

```
                     ┌─────────┐
                     │  USB    │
                     └─────────┘
    EN          │1              38│  GND
    VP(GPIO36)  │2              37│  D23(MOSI)
    VN(GPIO39)  │3              36│  D22(SCL)
    D34         │4              35│  TXD0(GPIO1)
    D35         │5              34│  RXD0(GPIO3)
    D32         │6              33│  D21(SDA)
    D33         │7              32│  NC
    D25         │8              31│  D19(MISO)
    D26         │9              30│  D18(SCK)
    D27         │10             29│  D5
    D14         │11             28│  D17
    D12         │12             27│  D16
    GND         │13             26│  D4
    D13         │14             25│  D0
    SD2(D9)     │15             24│  D2
    SD3(D10)    │16             23│  D15
    CMD(D11)    │17             22│  GND
    5V          │18             21│  VIN(5V)
    3V3         │19             20│  3V3
```

### Pin Capabilities

#### ADC (Analog to Digital)
- **ADC1**: GPIO 32-39 (8 channels)
- **ADC2**: GPIO 0, 2, 4, 12-15, 25-27 (10 channels)
- **Note**: ADC2 cannot be used when WiFi is active!
- Resolution: 12-bit (0-4095)

#### DAC (Digital to Analog)
- **GPIO 25**: DAC1
- **GPIO 26**: DAC2
- Resolution: 8-bit (0-255)

#### Touch Sensors
- GPIO 0, 2, 4, 12, 13, 14, 15, 27, 32, 33
- 10 capacitive touch pins
- No external components needed!

#### PWM
- All GPIO pins support PWM
- 16 channels available
- Configurable frequency and resolution

#### Communication Pins
- **I2C**: Any GPIO (typically GPIO 21=SDA, 22=SCL)
- **SPI**: 
  - MOSI: GPIO 23
  - MISO: GPIO 19
  - SCK: GPIO 18
  - CS: Any GPIO
- **UART**: 3 hardware serial ports

### Input-Only Pins ⚠️
These pins are **INPUT ONLY** (no output):
- GPIO 34, 35, 36 (VP), 39 (VN)

### Strapping Pins ⚠️
These pins affect boot mode - use carefully:
- **GPIO 0**: Must be HIGH during boot
- **GPIO 2**: Must be LOW during boot (connected to LED on many boards)
- **GPIO 12**: Controls flash voltage
- **GPIO 15**: Debug output

### Power Specifications
- **Operating Voltage**: 3.3V
- **Input Voltage**: 5V via USB or VIN pin
- **Max Current per Pin**: 40 mA
- **Max Total Current**: 200 mA
- **Deep Sleep Current**: ~10 µA
- **Active WiFi Current**: ~160 mA

## 🔍 Troubleshooting

### Upload Issues

#### "Failed to connect to ESP32"
**Solutions**:
1. Hold **BOOT** button while clicking upload
2. Release after "Connecting..." appears
3. Try lower upload speed (115200 instead of 921600)
4. Check USB cable (must support data, not charge-only)
5. Try different USB port

#### "Serial port not found"
**Solutions**:
1. Install USB drivers (CP2102 or CH340)
2. Check Device Manager (Windows) or `ls /dev/tty*` (Mac/Linux)
3. Try different USB cable
4. Check if board is powered (LED should be on)

#### "A fatal error occurred: Timed out waiting for packet header"
**Solutions**:
1. Hold BOOT button during upload
2. Press EN/Reset button then try upload
3. Lower upload speed
4. Check if another program is using serial port

### Runtime Issues

#### WiFi Won't Connect
**Check**:
1. SSID and password are correct
2. WiFi network is 2.4 GHz (ESP32 doesn't support 5 GHz)
3. Check signal strength (`WiFi.RSSI()`)
4. Try connecting to phone hotspot to test
5. Check if router has MAC filtering enabled

#### ESP32 Keeps Rebooting
**Causes**:
1. Power supply insufficient (use 5V 1A minimum)
2. Brownout detector triggered
3. Watchdog timer reset (code stuck in loop)
4. Faulty USB cable

**Solutions**:
1. Use powered USB hub or external power supply
2. Add `delay()` in tight loops
3. Check serial monitor for crash dump
4. Disable brownout in code: `WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);`

#### Bluetooth Not Working
**Check**:
1. Classic BT and BLE cannot run simultaneously
2. Enough memory available (Bluetooth uses ~100KB RAM)
3. Correct library included
4. Device name not too long

#### ADC Readings Incorrect
**Solutions**:
1. Don't use ADC2 pins when WiFi is active
2. Calibrate ADC readings
3. Use averaging for stable readings
4. Check voltage is within 0-3.3V range
5. Add small capacitor to ADC pin for noise reduction

### Common Error Messages

```
Brownout detector was triggered
```
**Fix**: Insufficient power. Use better power supply or add capacitor.

```
Guru Meditation Error: Core 0 panic'ed (LoadProhibited)
```
**Fix**: Memory access error. Check array bounds, pointer validity.

```
rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
```
**Fix**: Watchdog reset. Add delays, avoid infinite loops, check code logic.

## 📚 Essential Libraries

### Pre-installed with ESP32 Core
- **WiFi.h**: WiFi connectivity
- **BluetoothSerial.h**: Classic Bluetooth
- **BLE*.h**: Bluetooth Low Energy
- **WebServer.h**: Simple web server
- **HTTPClient.h**: HTTP requests
- **SPIFFS.h**: File system
- **Preferences.h**: Store settings in flash

### Popular Third-Party Libraries
- **PubSubClient**: MQTT communication
- **AsyncTCP**: Asynchronous TCP
- **ESPAsyncWebServer**: Better web server
- **ArduinoJson**: JSON parsing
- **NTPClient**: Network time
- **Firebase-ESP32**: Google Firebase

## 🎓 Next Steps

### Beginner Projects
1. ✅ Blink LED (completed above)
2. WiFi Scanner - See available networks
3. Simple Web Server - Control LED from browser
4. Bluetooth Serial - Communicate with phone

### Intermediate Projects
1. Weather Station - Get data from API
2. MQTT Home Automation - Smart home control
3. Web Dashboard - Real-time data display
4. OTA Updates - Update firmware wirelessly

### Advanced Projects
1. Voice Assistant Integration
2. Security Camera System
3. Mesh Network
4. GPS Tracker

Check out the `projects/07-esp32/` folder for complete project tutorials!

## 🔗 Resources

### Official Documentation
- [ESP32 Arduino Core Docs](https://docs.espressif.com/projects/arduino-esp32/)
- [Espressif ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

### Community Resources
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [ESP32.com Forum](https://www.esp32.com/)
- [r/esp32 on Reddit](https://www.reddit.com/r/esp32/)

### Tools
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [ESPHome](https://esphome.io/) - Configuration-based development
- [PlatformIO](https://platformio.org/) - Advanced IDE

## 💡 Tips for Success

1. **Start with WiFi scanner** - Verify WiFi works before complex projects
2. **Use Serial Monitor** - Essential for debugging
3. **Mind the voltage** - ESP32 is 3.3V, not 5V!
4. **Power matters** - Use good USB cable and power supply
5. **Check memory usage** - ESP32 has limited RAM
6. **Deep sleep for battery** - Can run weeks on battery with proper sleep
7. **OTA is your friend** - Update code without USB cable
8. **Read datasheets** - Know your sensor/module voltage requirements

## 🎉 You're Ready!

Now that you have:
- ✅ ESP32 board set up
- ✅ Arduino IDE configured
- ✅ First program uploaded
- ✅ Understanding of WiFi and Bluetooth basics
- ✅ Knowledge of pinout and capabilities

You're ready to build amazing IoT projects! Head to `projects/07-esp32/` to start building!

Happy coding! 🚀📶
