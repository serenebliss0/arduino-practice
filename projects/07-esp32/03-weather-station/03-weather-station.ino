/*
 * ESP32 Weather Station with API Integration
 * 
 * Fetches weather data from OpenWeatherMap API and reads
 * local environmental data from BME280 sensor.
 * Displays data on web interface and optional OLED.
 * 
 * Features:
 * - OpenWeatherMap API integration
 * - BME280 sensor reading (temp, humidity, pressure)
 * - Web server dashboard
 * - JSON data parsing
 * - Automatic periodic updates
 * - Optional OLED display
 * 
 * Circuit:
 * - BME280: SDA→GPIO21, SCL→GPIO22
 * - OLED (optional): Shares same I2C bus
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WebServer.h>

// Optional OLED display support
#define USE_OLED true  // Set to false if not using OLED
#if USE_OLED
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 64
  #define OLED_RESET -1
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// OpenWeatherMap API configuration
const char* apiKey = "YOUR_API_KEY";  // Get from openweathermap.org
const char* city = "London";           // Your city
const char* countryCode = "UK";        // Your country code
const char* units = "metric";          // "metric" or "imperial"

// Update intervals (milliseconds)
const unsigned long API_UPDATE_INTERVAL = 600000;  // 10 minutes (API limit)
const unsigned long SENSOR_UPDATE_INTERVAL = 10000; // 10 seconds
const unsigned long DISPLAY_UPDATE_INTERVAL = 2000; // 2 seconds

unsigned long lastApiUpdate = 0;
unsigned long lastSensorUpdate = 0;
unsigned long lastDisplayUpdate = 0;

// BME280 sensor
Adafruit_BME280 bme;
bool bmeFound = false;

// Weather data structures
struct LocalWeather {
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  bool valid;
} localWeather;

struct OnlineWeather {
  String city;
  String description;
  String icon;
  float temperature;
  float feelsLike;
  int humidity;
  float pressure;
  float windSpeed;
  int cloudiness;
  long sunrise;
  long sunset;
  bool valid;
} onlineWeather;

// Web server
WebServer server(80);

// Function prototypes
void connectWiFi();
void initBME280();
void updateLocalWeather();
void updateOnlineWeather();
void displayWeather();
void handleRoot();
void handleAPI();
String generateHTML();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n═══════════════════════════════════════");
  Serial.println("   ESP32 Weather Station");
  Serial.println("═══════════════════════════════════════\n");
  
  // Initialize I2C
  Wire.begin(21, 22); // SDA, SCL
  
  // Initialize BME280 sensor
  initBME280();
  
  // Initialize OLED display
  #if USE_OLED
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("✓ OLED display initialized");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Weather Station");
    display.println("Initializing...");
    display.display();
  } else {
    Serial.println("⚠ OLED display not found");
  }
  #endif
  
  // Connect to WiFi
  connectWiFi();
  
  // Initial weather updates
  if (bmeFound) {
    updateLocalWeather();
  }
  updateOnlineWeather();
  
  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  server.begin();
  
  Serial.println("\n✓ Weather station ready!");
  Serial.print("Web interface: http://");
  Serial.println(WiFi.localIP());
  Serial.println("═══════════════════════════════════════\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Update local sensor data
  if (bmeFound && currentTime - lastSensorUpdate >= SENSOR_UPDATE_INTERVAL) {
    lastSensorUpdate = currentTime;
    updateLocalWeather();
  }
  
  // Update online weather data
  if (currentTime - lastApiUpdate >= API_UPDATE_INTERVAL) {
    lastApiUpdate = currentTime;
    updateOnlineWeather();
  }
  
  // Update display
  #if USE_OLED
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    lastDisplayUpdate = currentTime;
    displayWeather();
  }
  #endif
  
  // Handle web requests
  server.handleClient();
  
  delay(10);
}

void connectWiFi() {
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
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ WiFi connection failed!");
  }
}

void initBME280() {
  Serial.print("Initializing BME280 sensor... ");
  
  // Try address 0x76 first, then 0x77
  if (bme.begin(0x76) || bme.begin(0x77)) {
    bmeFound = true;
    Serial.println("✓ Found!");
    
    // Configure sensor settings
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X2,  // Temperature
                    Adafruit_BME280::SAMPLING_X16, // Pressure
                    Adafruit_BME280::SAMPLING_X1,  // Humidity
                    Adafruit_BME280::FILTER_X16,
                    Adafruit_BME280::STANDBY_MS_500);
  } else {
    bmeFound = false;
    Serial.println("✗ Not found!");
    Serial.println("  Check wiring and I2C address (0x76 or 0x77)");
  }
}

void updateLocalWeather() {
  localWeather.temperature = bme.readTemperature();
  localWeather.humidity = bme.readHumidity();
  localWeather.pressure = bme.readPressure() / 100.0F; // Convert Pa to hPa
  localWeather.altitude = bme.readAltitude(1013.25);   // Sea level pressure
  localWeather.valid = true;
  
  Serial.println("📊 Local Weather Updated:");
  Serial.printf("  Temperature: %.1f°C\n", localWeather.temperature);
  Serial.printf("  Humidity: %.1f%%\n", localWeather.humidity);
  Serial.printf("  Pressure: %.1f hPa\n", localWeather.pressure);
}

void updateOnlineWeather() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠ WiFi not connected, skipping API update");
    return;
  }
  
  Serial.println("🌐 Fetching online weather data...");
  
  // Build API URL
  String url = "http://api.openweathermap.org/data/2.5/weather?q=";
  url += city;
  url += ",";
  url += countryCode;
  url += "&units=";
  url += units;
  url += "&appid=";
  url += apiKey;
  
  HTTPClient http;
  http.begin(url);
  
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String payload = http.getString();
    
    // Parse JSON response
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      // Extract weather data
      onlineWeather.city = doc["name"].as<String>();
      onlineWeather.description = doc["weather"][0]["description"].as<String>();
      onlineWeather.icon = doc["weather"][0]["icon"].as<String>();
      onlineWeather.temperature = doc["main"]["temp"];
      onlineWeather.feelsLike = doc["main"]["feels_like"];
      onlineWeather.humidity = doc["main"]["humidity"];
      onlineWeather.pressure = doc["main"]["pressure"];
      onlineWeather.windSpeed = doc["wind"]["speed"];
      onlineWeather.cloudiness = doc["clouds"]["all"];
      onlineWeather.sunrise = doc["sys"]["sunrise"];
      onlineWeather.sunset = doc["sys"]["sunset"];
      onlineWeather.valid = true;
      
      Serial.println("✓ Weather data received:");
      Serial.printf("  Location: %s\n", onlineWeather.city.c_str());
      Serial.printf("  Conditions: %s\n", onlineWeather.description.c_str());
      Serial.printf("  Temperature: %.1f°C\n", onlineWeather.temperature);
      Serial.printf("  Humidity: %d%%\n", onlineWeather.humidity);
    } else {
      Serial.println("✗ JSON parsing failed!");
      onlineWeather.valid = false;
    }
  } else {
    Serial.printf("✗ HTTP request failed, code: %d\n", httpCode);
    onlineWeather.valid = false;
  }
  
  http.end();
}

#if USE_OLED
void displayWeather() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Display online weather
  if (onlineWeather.valid) {
    display.print(onlineWeather.city);
    display.print(" ");
    display.println(onlineWeather.description);
    display.setTextSize(2);
    display.print(onlineWeather.temperature, 1);
    display.println("C");
  }
  
  display.setTextSize(1);
  display.println();
  
  // Display local sensor data
  if (localWeather.valid) {
    display.print("Local: ");
    display.print(localWeather.temperature, 1);
    display.println("C");
    display.print("Humid: ");
    display.print(localWeather.humidity, 0);
    display.println("%");
    display.print("Press: ");
    display.print(localWeather.pressure, 0);
    display.println("hPa");
  }
  
  display.display();
}
#endif

void handleRoot() {
  String html = generateHTML();
  server.send(200, "text/html", html);
}

void handleAPI() {
  // Return JSON data for API access
  StaticJsonDocument<512> doc;
  
  if (localWeather.valid) {
    JsonObject local = doc.createNestedObject("local");
    local["temperature"] = localWeather.temperature;
    local["humidity"] = localWeather.humidity;
    local["pressure"] = localWeather.pressure;
  }
  
  if (onlineWeather.valid) {
    JsonObject online = doc.createNestedObject("online");
    online["city"] = onlineWeather.city;
    online["description"] = onlineWeather.description;
    online["temperature"] = onlineWeather.temperature;
    online["humidity"] = onlineWeather.humidity;
    online["pressure"] = onlineWeather.pressure;
    online["windSpeed"] = onlineWeather.windSpeed;
  }
  
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

String generateHTML() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='60'>";  // Auto-refresh every minute
  html += "<title>ESP32 Weather Station</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);margin:0;padding:20px}";
  html += ".container{max-width:800px;margin:0 auto;background:white;border-radius:20px;padding:30px;box-shadow:0 10px 40px rgba(0,0,0,0.3)}";
  html += "h1{text-align:center;color:#333;margin-bottom:30px}";
  html += ".weather-card{background:#f8f9fa;border-radius:15px;padding:20px;margin-bottom:20px}";
  html += ".weather-card h2{color:#667eea;margin-bottom:15px}";
  html += ".data-row{display:flex;justify-content:space-between;padding:10px 0;border-bottom:1px solid #ddd}";
  html += ".data-row:last-child{border-bottom:none}";
  html += ".label{font-weight:bold;color:#666}";
  html += ".value{color:#333;font-size:18px}";
  html += ".temp{font-size:48px;font-weight:bold;color:#667eea;text-align:center;margin:20px 0}";
  html += ".footer{text-align:center;color:#666;margin-top:20px;font-size:12px}";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>🌤 Weather Station</h1>";
  
  // Online Weather
  if (onlineWeather.valid) {
    html += "<div class='weather-card'>";
    html += "<h2>🌐 " + onlineWeather.city + "</h2>";
    html += "<div class='temp'>" + String(onlineWeather.temperature, 1) + "°C</div>";
    html += "<p style='text-align:center;color:#666;font-size:18px'>" + onlineWeather.description + "</p>";
    html += "<div class='data-row'><span class='label'>Feels Like:</span><span class='value'>" + String(onlineWeather.feelsLike, 1) + "°C</span></div>";
    html += "<div class='data-row'><span class='label'>Humidity:</span><span class='value'>" + String(onlineWeather.humidity) + "%</span></div>";
    html += "<div class='data-row'><span class='label'>Pressure:</span><span class='value'>" + String(onlineWeather.pressure, 1) + " hPa</span></div>";
    html += "<div class='data-row'><span class='label'>Wind Speed:</span><span class='value'>" + String(onlineWeather.windSpeed, 1) + " m/s</span></div>";
    html += "<div class='data-row'><span class='label'>Cloudiness:</span><span class='value'>" + String(onlineWeather.cloudiness) + "%</span></div>";
    html += "</div>";
  }
  
  // Local Weather
  if (localWeather.valid) {
    html += "<div class='weather-card'>";
    html += "<h2>📍 Local Sensor</h2>";
    html += "<div class='data-row'><span class='label'>Temperature:</span><span class='value'>" + String(localWeather.temperature, 1) + "°C</span></div>";
    html += "<div class='data-row'><span class='label'>Humidity:</span><span class='value'>" + String(localWeather.humidity, 1) + "%</span></div>";
    html += "<div class='data-row'><span class='label'>Pressure:</span><span class='value'>" + String(localWeather.pressure, 1) + " hPa</span></div>";
    html += "<div class='data-row'><span class='label'>Altitude:</span><span class='value'>" + String(localWeather.altitude, 0) + " m</span></div>";
    html += "</div>";
  }
  
  html += "<div class='footer'>Auto-refreshes every minute | <a href='/api'>View JSON API</a></div>";
  html += "</div></body></html>";
  
  return html;
}
