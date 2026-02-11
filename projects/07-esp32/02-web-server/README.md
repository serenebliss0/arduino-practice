# Project 2: Web Server - LED Control 🌐

A beginner-friendly web server that allows you to control LEDs through your web browser. Learn the basics of web programming with ESP32!

## 🎯 Project Overview

**Difficulty**: Beginner  
**Time to Complete**: 45 minutes  
**Real-World Applications**:
- Home automation control panels
- IoT device management interfaces
- Remote appliance control
- Smart lighting systems
- Educational IoT demonstrations
- Prototyping web-based controls

This project introduces you to creating web interfaces for ESP32 devices. You'll build a responsive web page hosted directly on your ESP32 that can toggle LEDs on and off from any device connected to your network.

## 📦 Components Needed

### Hardware
- ESP32 Development Board
- 2x LEDs (any color, 5mm recommended)
- 2x 220Ω Resistors (red-red-brown)
- Breadboard
- Jumper wires
- USB cable for programming

### Software
- Arduino IDE with ESP32 board support
- WiFi library (built-in)
- WebServer library (built-in)

**Total Cost**: ~$10-15

## 🔌 Wiring Diagram

### Pin Connections

```
ESP32          Component
GPIO 25  ────→  LED 1 Anode (+)
                LED 1 Cathode (-) ──→ 220Ω Resistor ──→ GND

GPIO 26  ────→  LED 2 Anode (+)
                LED 2 Cathode (-) ──→ 220Ω Resistor ──→ GND
```

### Breadboard Layout

```
         ESP32 Board
    ┌─────────────────────┐
    │                     │
    │        GPIO25 ●─────┼────→ LED1 → 220Ω → GND
    │        GPIO26 ●─────┼────→ LED2 → 220Ω → GND
    │           GND ●─────┼────→ Common GND
    │                     │
    └─────────────────────┘
```

### Detailed Circuit

```
           3.3V
            │
            
ESP32 Pin 25 ──→ ──┐
                   │
                  LED1 (Anode to GPIO, Cathode down)
                   │
                  220Ω
                   │
                  GND

ESP32 Pin 26 ──→ ──┐
                   │
                  LED2
                   │
                  220Ω
                   │
                  GND
```

**Important Notes**:
- **LED Polarity**: Long leg (anode) connects to GPIO pin, short leg (cathode) to resistor
- **Resistor Value**: 220Ω limits current to safe levels (~10-15mA)
- **Common Ground**: All GND connections share the same ESP32 GND pin

## 💻 Code

### Main Sketch: `02-web-server.ino`

```cpp
/*
 * ESP32 Web Server - LED Control
 * 
 * A simple web server that provides a user interface to control
 * two LEDs connected to the ESP32. Access via browser at the
 * displayed IP address.
 * 
 * Features:
 * - Responsive web interface
 * - Real-time LED status display
 * - Individual LED control buttons
 * - WiFi connection with status feedback
 * - Auto-refresh capability
 * 
 * Circuit:
 * - LED 1: GPIO 25 → LED → 220Ω → GND
 * - LED 2: GPIO 26 → LED → 220Ω → GND
 */

#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials - CHANGE THESE!
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// LED Pin definitions
const int LED1_PIN = 25;
const int LED2_PIN = 26;

// LED states (false = OFF, true = ON)
bool led1State = false;
bool led2State = false;

// Create web server on port 80
WebServer server(80);

// Function prototypes
void handleRoot();
void handleLED1On();
void handleLED1Off();
void handleLED2On();
void handleLED2Off();
void handleNotFound();
String generateHTML();

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n═══════════════════════════════════════");
  Serial.println("     ESP32 Web Server - LED Control");
  Serial.println("═══════════════════════════════════════\n");
  
  // Configure LED pins as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  // Ensure LEDs start in OFF state
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  
  Serial.println("✓ GPIO pins configured");
  
  // Connect to WiFi
  connectToWiFi();
  
  // Configure web server routes
  server.on("/", handleRoot);                    // Main page
  server.on("/led1/on", handleLED1On);          // LED 1 ON
  server.on("/led1/off", handleLED1Off);        // LED 1 OFF
  server.on("/led2/on", handleLED2On);          // LED 2 ON
  server.on("/led2/off", handleLED2Off);        // LED 2 OFF
  server.onNotFound(handleNotFound);            // 404 handler
  
  // Start web server
  server.begin();
  Serial.println("✓ Web server started");
  Serial.println("\n═══════════════════════════════════════");
  Serial.println("Server is ready! Open browser and go to:");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("═══════════════════════════════════════\n");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
  
  // Add small delay to prevent WDT issues
  delay(2);
}

// Connect to WiFi network
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
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
    Serial.println("Please check your credentials and try again");
  }
}

// Handle root page request
void handleRoot() {
  String html = generateHTML();
  server.send(200, "text/html", html);
  Serial.println("📄 Page served to client: " + server.client().remoteIP().toString());
}

// Handle LED 1 ON request
void handleLED1On() {
  led1State = true;
  digitalWrite(LED1_PIN, HIGH);
  Serial.println("💡 LED 1 turned ON");
  
  String html = generateHTML();
  server.send(200, "text/html", html);
}

// Handle LED 1 OFF request
void handleLED1Off() {
  led1State = false;
  digitalWrite(LED1_PIN, LOW);
  Serial.println("💡 LED 1 turned OFF");
  
  String html = generateHTML();
  server.send(200, "text/html", html);
}

// Handle LED 2 ON request
void handleLED2On() {
  led2State = true;
  digitalWrite(LED2_PIN, HIGH);
  Serial.println("💡 LED 2 turned ON");
  
  String html = generateHTML();
  server.send(200, "text/html", html);
}

// Handle LED 2 OFF request
void handleLED2Off() {
  led2State = false;
  digitalWrite(LED2_PIN, LOW);
  Serial.println("💡 LED 2 turned OFF");
  
  String html = generateHTML();
  server.send(200, "text/html", html);
}

// Handle 404 Not Found
void handleNotFound() {
  String message = "404: Page Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  
  server.send(404, "text/plain", message);
  Serial.println("⚠ 404 Error: " + server.uri());
}

// Generate HTML page
String generateHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 LED Control</title>";
  html += "<style>";
  
  // CSS Styling
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: 'Segoe UI', Arial, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
  html += ".container { max-width: 600px; margin: 0 auto; background: white; border-radius: 20px; box-shadow: 0 20px 60px rgba(0,0,0,0.3); padding: 40px; }";
  html += "h1 { color: #333; text-align: center; margin-bottom: 10px; font-size: 28px; }";
  html += ".subtitle { text-align: center; color: #666; margin-bottom: 30px; }";
  html += ".led-control { background: #f8f9fa; border-radius: 15px; padding: 25px; margin-bottom: 20px; }";
  html += ".led-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 15px; }";
  html += ".led-name { font-size: 20px; font-weight: bold; color: #333; }";
  html += ".led-status { padding: 5px 15px; border-radius: 20px; font-size: 14px; font-weight: bold; }";
  html += ".status-on { background: #d4edda; color: #155724; }";
  html += ".status-off { background: #f8d7da; color: #721c24; }";
  html += ".button-group { display: flex; gap: 10px; }";
  html += ".btn { flex: 1; padding: 12px; border: none; border-radius: 8px; font-size: 16px; font-weight: bold; cursor: pointer; transition: all 0.3s; text-decoration: none; display: block; text-align: center; }";
  html += ".btn-on { background: #28a745; color: white; }";
  html += ".btn-on:hover { background: #218838; transform: translateY(-2px); box-shadow: 0 5px 15px rgba(40,167,69,0.3); }";
  html += ".btn-off { background: #dc3545; color: white; }";
  html += ".btn-off:hover { background: #c82333; transform: translateY(-2px); box-shadow: 0 5px 15px rgba(220,53,69,0.3); }";
  html += ".btn:active { transform: translateY(0); }";
  html += ".info { background: #e7f3ff; border-left: 4px solid #2196F3; padding: 15px; border-radius: 5px; margin-top: 20px; }";
  html += ".info p { color: #333; margin: 5px 0; font-size: 14px; }";
  html += ".footer { text-align: center; margin-top: 20px; color: #666; font-size: 12px; }";
  html += "@media (max-width: 480px) { .container { padding: 20px; } h1 { font-size: 24px; } }";
  
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  
  // Header
  html += "<h1>🌐 ESP32 LED Control</h1>";
  html += "<p class='subtitle'>Control Panel</p>";
  
  // LED 1 Control
  html += "<div class='led-control'>";
  html += "<div class='led-header'>";
  html += "<span class='led-name'>💡 LED 1</span>";
  html += "<span class='led-status " + String(led1State ? "status-on" : "status-off") + "'>";
  html += led1State ? "ON" : "OFF";
  html += "</span>";
  html += "</div>";
  html += "<div class='button-group'>";
  html += "<a href='/led1/on' class='btn btn-on'>Turn ON</a>";
  html += "<a href='/led1/off' class='btn btn-off'>Turn OFF</a>";
  html += "</div>";
  html += "</div>";
  
  // LED 2 Control
  html += "<div class='led-control'>";
  html += "<div class='led-header'>";
  html += "<span class='led-name'>💡 LED 2</span>";
  html += "<span class='led-status " + String(led2State ? "status-on" : "status-off") + "'>";
  html += led2State ? "ON" : "OFF";
  html += "</span>";
  html += "</div>";
  html += "<div class='button-group'>";
  html += "<a href='/led2/on' class='btn btn-on'>Turn ON</a>";
  html += "<a href='/led2/off' class='btn btn-off'>Turn OFF</a>";
  html += "</div>";
  html += "</div>";
  
  // System Info
  html += "<div class='info'>";
  html += "<p><strong>📡 Device:</strong> ESP32</p>";
  html += "<p><strong>🌐 IP Address:</strong> " + WiFi.localIP().toString() + "</p>";
  html += "<p><strong>📶 Signal:</strong> " + String(WiFi.RSSI()) + " dBm</p>";
  html += "<p><strong>⏱ Uptime:</strong> " + String(millis() / 1000) + " seconds</p>";
  html += "</div>";
  
  // Footer
  html += "<div class='footer'>";
  html += "<p>ESP32 Web Server Project | Page auto-refreshes on button click</p>";
  html += "</div>";
  
  html += "</div>";
  html += "</body>";
  html += "</html>";
  
  return html;
}
```

## 🚀 How to Use

### 1. Hardware Setup
1. Insert ESP32 into breadboard
2. Connect LED 1:
   - GPIO 25 → LED anode (long leg)
   - LED cathode → 220Ω resistor → GND
3. Connect LED 2:
   - GPIO 26 → LED anode
   - LED cathode → 220Ω resistor → GND
4. Double-check connections and polarity

### 2. Software Setup
1. Open Arduino IDE
2. Install ESP32 board support (if not already installed):
   - File → Preferences
   - Add to Additional Board Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Board Manager → Search "ESP32" → Install
3. Select board: Tools → Board → ESP32 Dev Module
4. Select port: Tools → Port → (your ESP32 port)

### 3. Configure WiFi
**IMPORTANT**: Edit these lines in the code:
```cpp
const char* ssid = "YOUR_WIFI_SSID";        // Your WiFi network name
const char* password = "YOUR_WIFI_PASSWORD"; // Your WiFi password
```

### 4. Upload Code
1. Click Verify (✓) to compile
2. Click Upload (→) to program ESP32
3. Open Serial Monitor (Ctrl+Shift+M)
4. Set baud rate to 115200
5. Note the IP address displayed

### 5. Access Web Interface
1. Open any web browser on a device connected to same WiFi
2. Enter the IP address shown in Serial Monitor
3. You'll see the LED control panel
4. Click buttons to control LEDs
5. Watch LEDs respond in real-time!

## 📊 Example Output

### Serial Monitor
```
═══════════════════════════════════════
     ESP32 Web Server - LED Control
═══════════════════════════════════════

✓ GPIO pins configured
Connecting to WiFi: MyHomeNetwork
.....
✓ WiFi connected!
IP Address: 192.168.1.100
Signal Strength: -45 dBm
✓ Web server started

═══════════════════════════════════════
Server is ready! Open browser and go to:
http://192.168.1.100
═══════════════════════════════════════

📄 Page served to client: 192.168.1.50
💡 LED 1 turned ON
📄 Page served to client: 192.168.1.50
💡 LED 2 turned ON
💡 LED 1 turned OFF
```

### Web Interface
The browser will display a beautiful, responsive control panel with:
- Clean, modern design with gradient background
- Two LED control sections
- Real-time status indicators (ON/OFF)
- Color-coded buttons (green for ON, red for OFF)
- System information (IP, signal strength, uptime)
- Mobile-friendly responsive layout

## 🎓 Understanding the Code

### Web Server Basics

**1. Including Libraries**
```cpp
#include <WiFi.h>      // WiFi connectivity
#include <WebServer.h>  // HTTP server functionality
```

**2. Creating Server Instance**
```cpp
WebServer server(80);  // Port 80 is standard for HTTP
```

**3. Defining Routes**
```cpp
server.on("/", handleRoot);           // Main page at http://ip/
server.on("/led1/on", handleLED1On); // LED control at http://ip/led1/on
```

**4. Starting Server**
```cpp
server.begin();  // Start listening for connections
```

**5. Handling Requests**
```cpp
void loop() {
  server.handleClient();  // Process incoming requests
}
```

### HTTP Request/Response Flow

```
Browser                 ESP32
  │                      │
  ├──── GET / ──────────→│  User loads page
  │←─── HTML ───────────┤  Server sends HTML
  │                      │
  ├─ GET /led1/on ──────→│  User clicks "Turn ON"
  │                      │  digitalWrite(LED1_PIN, HIGH)
  │←─── HTML ───────────┤  Server sends updated page
  │                      │
```

### HTML Generation

The `generateHTML()` function creates a complete HTML page with:
- **Structure**: HTML5 document structure
- **Styling**: Embedded CSS for beautiful appearance
- **Dynamic Content**: LED states update in real-time
- **Interactivity**: Buttons as links to different routes

### GPIO Control

```cpp
pinMode(LED1_PIN, OUTPUT);      // Configure pin as output
digitalWrite(LED1_PIN, HIGH);   // Turn LED on (3.3V)
digitalWrite(LED1_PIN, LOW);    // Turn LED off (0V)
```

### State Management

```cpp
bool led1State = false;  // Track LED state in software
// When state changes:
led1State = true;              // Update software state
digitalWrite(LED1_PIN, HIGH);  // Update hardware state
```

## 🔧 Customization Ideas

### 1. Add More LEDs
```cpp
const int LED3_PIN = 27;
// Add routes and handlers for LED 3
```

### 2. PWM Dimming Control
```cpp
// Use sliders to control LED brightness
analogWrite(LED1_PIN, brightness); // 0-255
```

### 3. Color LED (RGB)
```cpp
// Control RGB LED with color picker
digitalWrite(RED_PIN, r);
digitalWrite(GREEN_PIN, g);
digitalWrite(BLUE_PIN, b);
```

### 4. Schedule/Timer
```cpp
// Turn LED on/off at specific times
if (hour == 18) digitalWrite(LED1_PIN, HIGH);
```

### 5. API Endpoints (JSON)
```cpp
// Return JSON instead of HTML
server.send(200, "application/json", "{\"led1\":\"on\"}");
```

### 6. AJAX Updates
Add JavaScript to update status without page reload:
```javascript
fetch('/led1/on').then(() => updateStatus());
```

### 7. Authentication
```cpp
// Add password protection
if (!server.authenticate("admin", "password")) {
  return server.requestAuthentication();
}
```

### 8. Mobile App Integration
- Create REST API
- Build companion mobile app
- Use HTTP requests from app

### 9. Voice Control
- Integrate with Google Home/Alexa
- Use webhooks (IFTTT, etc.)

### 10. Data Logging
- Log LED state changes with timestamps
- Save to SD card or cloud

## 🐛 Troubleshooting

### WiFi Connection Issues

**Problem**: Can't connect to WiFi
- **Check credentials**: SSID and password must be exact (case-sensitive!)
- **Check band**: ESP32 only supports 2.4GHz, not 5GHz
- **Signal strength**: Move ESP32 closer to router
- **Router settings**: Some routers block new devices by default

**Problem**: Connection drops frequently
- **Power supply**: Use quality USB cable and power source
- **Add capacitor**: 100µF across VIN and GND
- **WiFi sleep**: Disable with `WiFi.setSleep(false)`

### Web Page Issues

**Problem**: Can't access web page
- **Check IP address**: Make sure you're using correct IP from Serial Monitor
- **Same network**: Computer/phone must be on same WiFi network
- **Firewall**: Some networks block device-to-device communication
- **Browser cache**: Try incognito/private mode
- **Try different browser**: Some browsers cache aggressively

**Problem**: Page loads but buttons don't work
- **Check routes**: Ensure all routes are defined correctly
- **Serial Monitor**: Check for error messages
- **LED connections**: Verify physical connections

### LED Problems

**Problem**: LEDs don't light up
- **Polarity**: Long leg (anode) to GPIO, short leg to resistor
- **Pin numbers**: Verify you're using GPIO 25 and 26
- **Resistor**: Make sure resistor is present (LED will burn out without it!)
- **Test LED**: Connect LED+resistor directly to 3.3V to test

**Problem**: LEDs are very dim
- **Current limiting**: Try lower value resistor (150Ω)
- **Power supply**: ESP32 may not provide enough current
- **Use transistor**: For high-power LEDs

**Problem**: ESP32 resets when turning on LEDs
- **Power issue**: LEDs drawing too much current
- **Add capacitor**: 100µF-1000µF near ESP32 power pins
- **External power**: Use separate power supply for LEDs

### Code Compilation Errors

**Problem**: `WebServer.h: No such file or directory`
- **Solution**: Update ESP32 core to latest version
- **Alternative**: Use `#include <ESP32WebServer.h>` instead

**Problem**: Board not found
- **Install ESP32**: Tools → Board Manager → Install ESP32
- **Select correct board**: Tools → Board → ESP32 Dev Module

## 📚 Learning Outcomes

After completing this project, you'll understand:

✅ **Web Server Concepts**
- HTTP protocol basics (GET requests)
- URL routing and handlers
- HTML generation and serving
- Client-server architecture

✅ **ESP32 Capabilities**
- WiFi station mode
- Simultaneous WiFi and GPIO operations
- Web server hosting on microcontroller
- Memory constraints and optimization

✅ **Web Development**
- HTML5 structure
- CSS styling and responsive design
- Building user interfaces
- Mobile-first design principles

✅ **GPIO Control**
- Digital output control
- LED operation and current limiting
- State management in software

✅ **Network Programming**
- IP addresses and ports
- Local network communication
- Handling multiple client connections

✅ **Real-World Skills**
- Debugging network issues
- Reading datasheets
- Circuit design and testing
- User interface design

## 🚀 Next Steps

Ready to advance? Try these challenges:

### Beginner
1. **Add button press counter** - Track how many times each button is clicked
2. **Add timestamp** - Show when LED was last changed
3. **Change colors** - Customize the web page appearance
4. **Add sound** - Play audio when button is clicked

### Intermediate
5. **Password protection** - Add authentication to web server
6. **Multiple pages** - Create separate pages for different functions
7. **Analog sensor** - Display temperature sensor reading
8. **Chart/graph** - Visualize sensor data over time
9. **Control relay** - Switch high-power devices

### Advanced
10. **WebSocket** - Real-time updates without page refresh
11. **REST API** - Build JSON API for mobile app
12. **MQTT integration** - Publish LED states to MQTT broker
13. **Database logging** - Store states in SQLite/InfluxDB
14. **SSL/HTTPS** - Add encryption for security

### Project Ideas
- **Smart home dashboard** - Control lights, fans, etc.
- **Garage door opener** - Remote access with status
- **Plant watering system** - Schedule-based control
- **Security system** - LED indicators for zones
- **Aquarium controller** - Lights, pump, heater control

## 📖 References

### Documentation
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [ESP32 WebServer Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer)
- [HTML5 Documentation](https://developer.mozilla.org/en-US/docs/Web/HTML)
- [CSS Basics](https://developer.mozilla.org/en-US/docs/Web/CSS)

### Tutorials
- [HTTP Protocol Explained](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)
- [RESTful API Design](https://restfulapi.net/)
- [Responsive Web Design](https://web.dev/responsive-web-design-basics/)

### Tools
- [HTML Color Picker](https://htmlcolorcodes.com/)
- [CSS Gradient Generator](https://cssgradient.io/)
- [Responsive Design Tester](https://responsivedesignchecker.com/)

### Community
- [ESP32 Forum](https://esp32.com/)
- [Arduino Forum - ESP32](https://forum.arduino.cc/c/hardware/esp32/173)
- [Reddit r/esp32](https://reddit.com/r/esp32)

## 💡 Tips for Success

1. **Start Simple**: Get basic LED control working first, then enhance UI
2. **Test Incrementally**: Test each feature as you add it
3. **Use Serial Monitor**: Debug with Serial.println() statements
4. **Save Power**: Disable WiFi sleep if having stability issues
5. **Organize Code**: Keep HTML in separate function for clarity
6. **Comment Well**: Explain complex logic for future reference
7. **Version Control**: Use Git to track changes
8. **Mobile First**: Test on phone - most users will use mobile
9. **Error Handling**: Always check WiFi.status() before serving
10. **Security**: Don't expose to public internet without authentication

---

**Project Complete!** 🎉

You've built a fully functional IoT device with a web interface! This is the foundation for countless home automation and IoT projects. The skills you've learned here will apply to almost every ESP32 web-based project.

**Share Your Build**: Post your project with #ESP32WebServer

Next up: **Project 3 - Weather Station** where we'll fetch real internet data! 🌦️
