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
