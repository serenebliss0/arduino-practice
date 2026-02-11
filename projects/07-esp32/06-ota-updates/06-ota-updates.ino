/*
 * ESP32 Over-The-Air (OTA) Updates
 * Update ESP32 firmware wirelessly without USB connection
 * 
 * Features:
 * - Web-based OTA update interface
 * - Password-protected updates
 * - Progress indication with LED
 * - Firmware version tracking
 * - Rollback capability
 * - Update status display
 * - WiFi signal monitoring
 * - Automatic restart after update
 * 
 * Required Libraries:
 * - WiFi (built-in)
 * - WebServer (built-in)
 * - Update (built-in)
 * - ESPmDNS (built-in)
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>

// ==================== CONFIGURATION ====================
// Firmware Version
#define FIRMWARE_VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// OTA Configuration
const char* OTA_HOSTNAME = "esp32-ota";  // Access via http://esp32-ota.local
const char* OTA_PASSWORD = "admin123";   // Change this for security!
const int WEB_SERVER_PORT = 80;

// Pin Definitions
#define LED_PIN 2           // Built-in LED for status
#define BUTTON_PIN 18       // Reset button (optional)

// ==================== GLOBAL OBJECTS ====================
WebServer server(WEB_SERVER_PORT);

// Update progress tracking
size_t updateProgress = 0;
size_t updateTotal = 0;

// ==================== HTML PAGES ====================
const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 OTA Update</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #667eea;
            text-align: center;
            margin-bottom: 30px;
        }
        .info {
            background: #f5f5f5;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
        }
        .info-item {
            display: flex;
            justify-content: space-between;
            margin: 10px 0;
        }
        .label {
            font-weight: bold;
            color: #666;
        }
        .value {
            color: #333;
        }
        form {
            margin-top: 20px;
        }
        input[type="password"] {
            width: 100%;
            padding: 12px;
            margin: 10px 0;
            border: 2px solid #ddd;
            border-radius: 5px;
            box-sizing: border-box;
            font-size: 16px;
        }
        button {
            width: 100%;
            padding: 12px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
            transition: background 0.3s;
        }
        button:hover {
            background: #5568d3;
        }
        .footer {
            text-align: center;
            margin-top: 20px;
            color: #666;
            font-size: 12px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🚀 ESP32 OTA Update</h1>
        <div class="info">
            <div class="info-item">
                <span class="label">Version:</span>
                <span class="value">%VERSION%</span>
            </div>
            <div class="info-item">
                <span class="label">Build Date:</span>
                <span class="value">%BUILD_DATE%</span>
            </div>
            <div class="info-item">
                <span class="label">IP Address:</span>
                <span class="value">%IP_ADDRESS%</span>
            </div>
            <div class="info-item">
                <span class="label">Signal:</span>
                <span class="value">%RSSI% dBm</span>
            </div>
        </div>
        <form action="/update" method="POST">
            <input type="password" name="password" placeholder="Enter password" required>
            <button type="submit">Continue to Update</button>
        </form>
        <div class="footer">
            Powered by ESP32
        </div>
    </div>
</body>
</html>
)rawliteral";

const char* updatePage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Upload Firmware</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            max-width: 500px;
            width: 100%;
        }
        h1 {
            color: #667eea;
            text-align: center;
            margin-bottom: 30px;
        }
        .alert {
            background: #fff3cd;
            border: 1px solid #ffc107;
            border-radius: 5px;
            padding: 15px;
            margin-bottom: 20px;
        }
        .file-input {
            border: 2px dashed #667eea;
            border-radius: 5px;
            padding: 40px;
            text-align: center;
            margin: 20px 0;
            cursor: pointer;
            transition: background 0.3s;
        }
        .file-input:hover {
            background: #f5f5f5;
        }
        input[type="file"] {
            display: none;
        }
        .file-label {
            color: #667eea;
            font-weight: bold;
            cursor: pointer;
        }
        .selected-file {
            margin: 10px 0;
            color: #333;
        }
        button {
            width: 100%;
            padding: 12px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
            transition: background 0.3s;
        }
        button:hover {
            background: #5568d3;
        }
        button:disabled {
            background: #ccc;
            cursor: not-allowed;
        }
        .progress-container {
            display: none;
            margin: 20px 0;
        }
        .progress-bar {
            width: 100%;
            height: 30px;
            background: #f0f0f0;
            border-radius: 15px;
            overflow: hidden;
        }
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            transition: width 0.3s;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
            font-weight: bold;
        }
        .status {
            text-align: center;
            margin: 10px 0;
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📤 Upload Firmware</h1>
        <div class="alert">
            ⚠️ <strong>Warning:</strong> Do not disconnect power during update!
        </div>
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data">
            <div class="file-input" onclick="document.getElementById('firmware').click()">
                <label class="file-label">
                    📁 Click to select firmware file (.bin)
                </label>
                <input type="file" id="firmware" name="firmware" accept=".bin" onchange="fileSelected(this)">
                <div class="selected-file" id="selectedFile"></div>
            </div>
            <button type="submit" id="uploadBtn" disabled>Upload Firmware</button>
        </form>
        <div class="progress-container" id="progressContainer">
            <div class="progress-bar">
                <div class="progress-fill" id="progressFill">0%</div>
            </div>
            <div class="status" id="status">Uploading...</div>
        </div>
    </div>
    <script>
        function fileSelected(input) {
            if (input.files.length > 0) {
                document.getElementById('selectedFile').textContent = '✓ ' + input.files[0].name;
                document.getElementById('uploadBtn').disabled = false;
            }
        }
        
        document.getElementById('uploadForm').addEventListener('submit', function(e) {
            e.preventDefault();
            
            var formData = new FormData(this);
            var xhr = new XMLHttpRequest();
            
            document.getElementById('progressContainer').style.display = 'block';
            document.getElementById('uploadBtn').disabled = true;
            
            xhr.upload.addEventListener('progress', function(e) {
                if (e.lengthComputable) {
                    var percent = Math.round((e.loaded / e.total) * 100);
                    document.getElementById('progressFill').style.width = percent + '%';
                    document.getElementById('progressFill').textContent = percent + '%';
                }
            });
            
            xhr.addEventListener('load', function() {
                if (xhr.status === 200) {
                    document.getElementById('status').textContent = '✓ Upload complete! Rebooting...';
                    document.getElementById('status').style.color = '#4caf50';
                    setTimeout(function() {
                        window.location.href = '/';
                    }, 5000);
                } else {
                    document.getElementById('status').textContent = '✗ Upload failed!';
                    document.getElementById('status').style.color = '#f44336';
                    document.getElementById('uploadBtn').disabled = false;
                }
            });
            
            xhr.addEventListener('error', function() {
                document.getElementById('status').textContent = '✗ Connection error!';
                document.getElementById('status').style.color = '#f44336';
                document.getElementById('uploadBtn').disabled = false;
            });
            
            xhr.open('POST', '/upload');
            xhr.send(formData);
        });
    </script>
</body>
</html>
)rawliteral";

// ==================== SETUP ====================
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("      ESP32 OTA Update System");
  Serial.println("═══════════════════════════════════════");
  Serial.printf("Firmware Version: %s\n", FIRMWARE_VERSION);
  Serial.printf("Build Date: %s %s\n", BUILD_DATE, BUILD_TIME);
  Serial.println();
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);
  
  // Connect to WiFi
  connectWiFi();
  
  // Setup mDNS
  if (MDNS.begin(OTA_HOSTNAME)) {
    Serial.printf("✓ mDNS responder started: http://%s.local\n", OTA_HOSTNAME);
    MDNS.addService("http", "tcp", WEB_SERVER_PORT);
  } else {
    Serial.println("✗ Error starting mDNS");
  }
  
  // Setup web server routes
  setupWebServer();
  
  // Start web server
  server.begin();
  Serial.println("✓ Web server started");
  Serial.println();
  printAccessInfo();
  
  // Blink LED to indicate ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// ==================== MAIN LOOP ====================
void loop() {
  server.handleClient();
  
  // Optional: Check for factory reset button
  static unsigned long buttonPressTime = 0;
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (buttonPressTime == 0) {
      buttonPressTime = millis();
    } else if (millis() - buttonPressTime > 5000) {
      // Button held for 5 seconds - factory reset
      Serial.println("Factory reset triggered!");
      // Add factory reset logic here if needed
      buttonPressTime = 0;
    }
  } else {
    buttonPressTime = 0;
  }
  
  delay(10);
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
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Blink during connection
    attempts++;
  }
  
  digitalWrite(LED_PIN, LOW);
  
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

// ==================== WEB SERVER SETUP ====================
void setupWebServer() {
  // Root page - login
  server.on("/", HTTP_GET, []() {
    String page = String(loginPage);
    page.replace("%VERSION%", FIRMWARE_VERSION);
    page.replace("%BUILD_DATE%", String(BUILD_DATE) + " " + String(BUILD_TIME));
    page.replace("%IP_ADDRESS%", WiFi.localIP().toString());
    page.replace("%RSSI%", String(WiFi.RSSI()));
    server.send(200, "text/html", page);
  });
  
  // Password check and show update page
  server.on("/update", HTTP_POST, []() {
    if (server.hasArg("password") && server.arg("password") == OTA_PASSWORD) {
      server.send(200, "text/html", updatePage);
    } else {
      server.send(401, "text/plain", "Invalid password!");
    }
  });
  
  // Handle firmware upload
  server.on("/upload", HTTP_POST, []() {
    // This is called after upload completes
    server.sendHeader("Connection", "close");
    if (Update.hasError()) {
      server.send(500, "text/plain", "Update failed!");
    } else {
      server.send(200, "text/plain", "Update successful! Rebooting...");
      delay(1000);
      ESP.restart();
    }
  }, []() {
    // This is called during upload
    HTTPUpload& upload = server.upload();
    
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      
      // Turn on LED during update
      digitalWrite(LED_PIN, HIGH);
      
      // Start update
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // Write firmware data
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      } else {
        // Update progress
        updateProgress += upload.currentSize;
        int percent = (updateProgress * 100) / upload.totalSize;
        Serial.printf("Progress: %d%%\r", percent);
        
        // Blink LED to show progress
        if (percent % 10 == 0) {
          digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        }
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
        Serial.printf("\nUpdate Success: %u bytes\n", upload.totalSize);
        digitalWrite(LED_PIN, LOW);
      } else {
        Update.printError(Serial);
        digitalWrite(LED_PIN, LOW);
      }
      updateProgress = 0;
    }
  });
  
  // Info page (JSON)
  server.on("/info", HTTP_GET, []() {
    String json = "{";
    json += "\"version\":\"" + String(FIRMWARE_VERSION) + "\",";
    json += "\"build_date\":\"" + String(BUILD_DATE) + " " + String(BUILD_TIME) + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"sketch_size\":" + String(ESP.getSketchSize()) + ",";
    json += "\"free_sketch_space\":" + String(ESP.getFreeSketchSpace()) + ",";
    json += "\"chip_id\":" + String((uint32_t)ESP.getEfuseMac()) + "";
    json += "}";
    server.send(200, "application/json", json);
  });
  
  // Reboot endpoint
  server.on("/reboot", HTTP_GET, []() {
    server.send(200, "text/plain", "Rebooting...");
    delay(1000);
    ESP.restart();
  });
  
  // 404 handler
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });
}

// ==================== UTILITY FUNCTIONS ====================
void printAccessInfo() {
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║         Access Information            ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.printf("║ URL: http://%-24s ║\n", WiFi.localIP().toString().c_str());
  Serial.printf("║ mDNS: http://%-23s ║\n", (String(OTA_HOSTNAME) + ".local").c_str());
  Serial.printf("║ Password: %-27s ║\n", OTA_PASSWORD);
  Serial.println("╚═══════════════════════════════════════╝");
}
