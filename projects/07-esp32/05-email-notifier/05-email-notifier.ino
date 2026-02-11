/*
 * ESP32 Email Notifier
 * Send email notifications when sensors are triggered
 * 
 * Features:
 * - Send emails via SMTP
 * - Multiple trigger conditions
 * - Motion detection alerts
 * - Temperature threshold monitoring
 * - Customizable email content with HTML
 * - Cooldown period to prevent spam
 * - Email attachments support
 * 
 * Required Libraries:
 * - WiFi (built-in)
 * - ESP_Mail_Client by Mobizt (install via Library Manager)
 * - DHT sensor library by Adafruit (install via Library Manager)
 */

#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <DHT.h>

// ==================== CONFIGURATION ====================
// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Email SMTP Settings (Gmail example)
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465  // SSL port (use 587 for TLS)

// Sender Email Account
#define SENDER_EMAIL "your_email@gmail.com"
#define SENDER_PASSWORD "your_app_password"  // Use App Password for Gmail
#define SENDER_NAME "ESP32 Security System"

// Recipient Email
#define RECIPIENT_EMAIL "recipient@example.com"
#define RECIPIENT_NAME "Homeowner"

// Pin Definitions
#define DHT_PIN 4           // DHT22 data pin
#define DHT_TYPE DHT22      // DHT22 sensor type
#define PIR_PIN 5           // PIR motion sensor pin
#define BUTTON_PIN 18       // Manual trigger button
#define LED_PIN 2           // Status LED

// Threshold Configuration
const float TEMP_HIGH_THRESHOLD = 30.0;  // °C
const float TEMP_LOW_THRESHOLD = 15.0;   // °C
const float HUMIDITY_HIGH_THRESHOLD = 80.0;  // %

// Timing Configuration
const unsigned long SENSOR_CHECK_INTERVAL = 30000;  // 30 seconds
const unsigned long EMAIL_COOLDOWN = 300000;  // 5 minutes between emails
const unsigned long MOTION_TIMEOUT = 5000;  // 5 seconds motion timeout

// ==================== GLOBAL OBJECTS ====================
DHT dht(DHT_PIN, DHT_TYPE);
SMTPSession smtp;

// Email session configuration
Session_Config config;

// State variables
unsigned long lastSensorCheck = 0;
unsigned long lastEmailSent = 0;
unsigned long lastMotionTime = 0;
bool motionAlertSent = false;
bool lastButtonState = HIGH;
float lastTemperature = 0;
float lastHumidity = 0;

// ==================== SETUP ====================
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("       ESP32 Email Notifier");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("✓ DHT sensor initialized");
  
  // Connect to WiFi
  connectWiFi();
  
  // Configure SMTP session
  configureSMTP();
  
  Serial.println("✓ Email system ready!");
  Serial.println();
  
  // Send startup notification
  sendStartupEmail();
  
  // Flash LED to indicate ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// ==================== MAIN LOOP ====================
void loop() {
  unsigned long currentTime = millis();
  
  // Check button for manual email trigger
  checkButton();
  
  // Check motion sensor
  checkMotion(currentTime);
  
  // Check temperature and humidity periodically
  if (currentTime - lastSensorCheck >= SENSOR_CHECK_INTERVAL) {
    lastSensorCheck = currentTime;
    checkSensors();
  }
  
  delay(100);  // Small delay to prevent excessive CPU usage
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

// ==================== SMTP CONFIGURATION ====================
void configureSMTP() {
  Serial.println("Configuring SMTP...");
  
  // Set the session config
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = SENDER_EMAIL;
  config.login.password = SENDER_PASSWORD;
  config.login.user_domain = "";
  
  // Set the NTP config for time
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 0;  // Adjust for your timezone
  config.time.day_light_offset = 0;
  
  Serial.println("✓ SMTP configured");
}

// ==================== SENSOR CHECKING ====================
void checkSensors() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Checking sensors...");
  
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
  
  lastTemperature = temperature;
  lastHumidity = humidity;
  
  // Check thresholds
  bool shouldSendEmail = false;
  String alertReason = "";
  
  if (temperature > TEMP_HIGH_THRESHOLD) {
    alertReason = "High Temperature Alert";
    shouldSendEmail = true;
    Serial.println("⚠️  Temperature too high!");
  } else if (temperature < TEMP_LOW_THRESHOLD) {
    alertReason = "Low Temperature Alert";
    shouldSendEmail = true;
    Serial.println("⚠️  Temperature too low!");
  }
  
  if (humidity > HUMIDITY_HIGH_THRESHOLD) {
    if (shouldSendEmail) {
      alertReason += " & High Humidity";
    } else {
      alertReason = "High Humidity Alert";
    }
    shouldSendEmail = true;
    Serial.println("⚠️  Humidity too high!");
  }
  
  // Send email if threshold exceeded and cooldown passed
  if (shouldSendEmail && canSendEmail()) {
    sendSensorAlert(alertReason, temperature, humidity);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

// ==================== MOTION DETECTION ====================
void checkMotion(unsigned long currentTime) {
  bool motionDetected = digitalRead(PIR_PIN);
  
  if (motionDetected) {
    lastMotionTime = currentTime;
    
    // Send alert if not already sent and cooldown passed
    if (!motionAlertSent && canSendEmail()) {
      Serial.println("🚶 Motion detected! Sending alert...");
      sendMotionAlert();
      motionAlertSent = true;
      
      // Flash LED rapidly
      for (int i = 0; i < 5; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
      }
    }
  } else {
    // Reset motion alert flag after timeout
    if (currentTime - lastMotionTime > MOTION_TIMEOUT) {
      motionAlertSent = false;
    }
  }
}

// ==================== BUTTON CHECK ====================
void checkButton() {
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // Detect button press (LOW because of INPUT_PULLUP)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Serial.println("🔘 Manual trigger button pressed!");
    delay(50);  // Debounce
    
    if (canSendEmail()) {
      sendManualAlert();
    } else {
      Serial.println("⏳ Email cooldown active, please wait...");
      // Flash LED to indicate cooldown
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
  }
  
  lastButtonState = currentButtonState;
}

// ==================== EMAIL COOLDOWN CHECK ====================
bool canSendEmail() {
  unsigned long currentTime = millis();
  if (currentTime - lastEmailSent >= EMAIL_COOLDOWN) {
    return true;
  }
  
  unsigned long remainingCooldown = (EMAIL_COOLDOWN - (currentTime - lastEmailSent)) / 1000;
  Serial.printf("⏳ Cooldown active: %lu seconds remaining\n", remainingCooldown);
  return false;
}

// ==================== EMAIL SENDING FUNCTIONS ====================
void sendStartupEmail() {
  Serial.println("Sending startup notification...");
  
  // Prepare email message
  SMTP_Message message;
  message.sender.name = SENDER_NAME;
  message.sender.email = SENDER_EMAIL;
  message.subject = "ESP32 Security System - Started";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  
  // Create HTML content
  String htmlMsg = "<div style='font-family: Arial, sans-serif;'>";
  htmlMsg += "<h2 style='color: #2196F3;'>🔔 System Startup</h2>";
  htmlMsg += "<p>Your ESP32 security system has been started successfully.</p>";
  htmlMsg += "<h3>System Information:</h3>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><strong>Device:</strong> ESP32 Security System</li>";
  htmlMsg += "<li><strong>IP Address:</strong> " + WiFi.localIP().toString() + "</li>";
  htmlMsg += "<li><strong>WiFi Signal:</strong> " + String(WiFi.RSSI()) + " dBm</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<p style='color: #666;'>This is an automated message from your ESP32 device.</p>";
  htmlMsg += "</div>";
  
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  // Send email
  if (!smtp.connect(&config)) {
    Serial.printf("✗ Connection error: %s\n", smtp.errorReason().c_str());
    return;
  }
  
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("✗ Error sending email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("✓ Startup email sent successfully!");
    lastEmailSent = millis();
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
  
  smtp.closeSession();
}

void sendMotionAlert() {
  Serial.println("Sending motion alert email...");
  
  SMTP_Message message;
  message.sender.name = SENDER_NAME;
  message.sender.email = SENDER_EMAIL;
  message.subject = "⚠️ MOTION DETECTED - ESP32 Alert";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  
  // Create HTML content
  String htmlMsg = "<div style='font-family: Arial, sans-serif;'>";
  htmlMsg += "<h2 style='color: #F44336;'>🚨 Motion Detected!</h2>";
  htmlMsg += "<p style='font-size: 16px;'>Motion has been detected by your ESP32 security system.</p>";
  htmlMsg += "<h3>Details:</h3>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><strong>Time:</strong> " + getFormattedTime() + "</li>";
  htmlMsg += "<li><strong>Sensor:</strong> PIR Motion Detector</li>";
  htmlMsg += "<li><strong>Location:</strong> Main Area</li>";
  htmlMsg += "<li><strong>Temperature:</strong> " + String(lastTemperature, 1) + "°C</li>";
  htmlMsg += "<li><strong>Humidity:</strong> " + String(lastHumidity, 1) + "%</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<p style='color: #666;'>Please check your security system immediately.</p>";
  htmlMsg += "</div>";
  
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;
  
  // Send email
  if (!smtp.connect(&config)) {
    Serial.printf("✗ Connection error: %s\n", smtp.errorReason().c_str());
    return;
  }
  
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("✗ Error sending email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("✓ Motion alert email sent successfully!");
    lastEmailSent = millis();
  }
  
  smtp.closeSession();
}

void sendSensorAlert(String reason, float temperature, float humidity) {
  Serial.printf("Sending sensor alert email: %s\n", reason.c_str());
  
  SMTP_Message message;
  message.sender.name = SENDER_NAME;
  message.sender.email = SENDER_EMAIL;
  message.subject = "⚠️ " + reason + " - ESP32 Alert";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  
  // Create HTML content
  String htmlMsg = "<div style='font-family: Arial, sans-serif;'>";
  htmlMsg += "<h2 style='color: #FF9800;'>⚠️ Sensor Alert</h2>";
  htmlMsg += "<p style='font-size: 16px;'><strong>Alert Type:</strong> " + reason + "</p>";
  htmlMsg += "<h3>Current Readings:</h3>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><strong>Temperature:</strong> " + String(temperature, 1) + "°C";
  if (temperature > TEMP_HIGH_THRESHOLD) {
    htmlMsg += " <span style='color: #F44336;'>(Above threshold: " + String(TEMP_HIGH_THRESHOLD, 1) + "°C)</span>";
  } else if (temperature < TEMP_LOW_THRESHOLD) {
    htmlMsg += " <span style='color: #2196F3;'>(Below threshold: " + String(TEMP_LOW_THRESHOLD, 1) + "°C)</span>";
  }
  htmlMsg += "</li>";
  htmlMsg += "<li><strong>Humidity:</strong> " + String(humidity, 1) + "%";
  if (humidity > HUMIDITY_HIGH_THRESHOLD) {
    htmlMsg += " <span style='color: #F44336;'>(Above threshold: " + String(HUMIDITY_HIGH_THRESHOLD, 1) + "%)</span>";
  }
  htmlMsg += "</li>";
  htmlMsg += "<li><strong>Time:</strong> " + getFormattedTime() + "</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<p style='color: #666;'>Please check your system and take appropriate action.</p>";
  htmlMsg += "</div>";
  
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;
  
  // Send email
  if (!smtp.connect(&config)) {
    Serial.printf("✗ Connection error: %s\n", smtp.errorReason().c_str());
    return;
  }
  
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("✗ Error sending email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("✓ Sensor alert email sent successfully!");
    lastEmailSent = millis();
  }
  
  smtp.closeSession();
}

void sendManualAlert() {
  Serial.println("Sending manual alert email...");
  
  SMTP_Message message;
  message.sender.name = SENDER_NAME;
  message.sender.email = SENDER_EMAIL;
  message.subject = "🔔 Manual Alert - ESP32 Notification";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  
  // Create HTML content
  String htmlMsg = "<div style='font-family: Arial, sans-serif;'>";
  htmlMsg += "<h2 style='color: #4CAF50;'>🔔 Manual Alert</h2>";
  htmlMsg += "<p style='font-size: 16px;'>A manual alert has been triggered from your ESP32 system.</p>";
  htmlMsg += "<h3>Current Status:</h3>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><strong>Temperature:</strong> " + String(lastTemperature, 1) + "°C</li>";
  htmlMsg += "<li><strong>Humidity:</strong> " + String(lastHumidity, 1) + "%</li>";
  htmlMsg += "<li><strong>Motion:</strong> " + String(digitalRead(PIR_PIN) ? "Detected" : "None") + "</li>";
  htmlMsg += "<li><strong>Time:</strong> " + getFormattedTime() + "</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<p style='color: #666;'>This alert was manually triggered by pressing the button.</p>";
  htmlMsg += "</div>";
  
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  // Send email
  if (!smtp.connect(&config)) {
    Serial.printf("✗ Connection error: %s\n", smtp.errorReason().c_str());
    return;
  }
  
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("✗ Error sending email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("✓ Manual alert email sent successfully!");
    lastEmailSent = millis();
    
    // Success indication
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
  
  smtp.closeSession();
}

// ==================== UTILITY FUNCTIONS ====================
String getFormattedTime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;
  
  char timeBuffer[50];
  sprintf(timeBuffer, "%lu days, %02lu:%02lu:%02lu", days, hours, minutes, seconds);
  return String(timeBuffer);
}

// ==================== SMTP CALLBACK ====================
void smtpCallback(SMTP_Status status) {
  // Print SMTP debug info
  Serial.println(status.info());
  
  if (status.success()) {
    Serial.println("✓ Message sent successfully!");
  }
}
