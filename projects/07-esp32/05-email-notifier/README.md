# Project 5: ESP32 Email Notifier 📧🔔

Send real-time email notifications from your ESP32 when sensors are triggered, creating an intelligent alert system that keeps you informed anywhere in the world via email and SMS.

## 🎯 Project Overview

**Difficulty**: Intermediate  
**Time to Complete**: 2-3 hours  
**Real-World Applications**:
- Security systems with email alerts
- Temperature monitoring with notifications
- Motion detection alerts for home security
- Equipment failure warnings
- Environmental monitoring systems
- Remote facility monitoring
- Elderly care monitoring systems
- Greenhouse/garden automation alerts
- Package delivery notifications
- Water leak detection systems
- Smart home event notifications
- Industrial equipment monitoring

Email notifications transform your ESP32 into a smart alerting system that can reach you anywhere. This project demonstrates how to send professional HTML emails via SMTP, monitor multiple sensors, and implement intelligent cooldown mechanisms to prevent notification spam.

## 📦 Components Needed

### Hardware
| Component | Quantity | Estimated Cost |
|-----------|----------|----------------|
| ESP32 Development Board | 1 | $8-12 |
| DHT22 Temperature/Humidity Sensor | 1 | $3-5 |
| PIR Motion Sensor (HC-SR501) | 1 | $2-4 |
| Pushbutton | 1 | $0.50-1 |
| LED (any color) | 1 | $0.10 |
| Active Buzzer (5V) - Optional | 1 | $1-2 |
| 220Ω Resistor (for LED) | 1 | $0.05 |
| 10kΩ Resistor (pull-up for DHT22) | 1 | $0.05 |
| 10kΩ Resistor (pull-down for button) - Optional | 1 | $0.05 |
| Breadboard | 1 | $3-5 |
| Jumper Wires | 10-15 | $2-3 |
| USB Cable (Micro USB or USB-C) | 1 | $2-3 |

### Software
- Arduino IDE with ESP32 board support
- **ESP_Mail_Client** library by Mobizt (v3.4.0 or later)
- **DHT sensor library** by Adafruit
- **Adafruit Unified Sensor** library (dependency)
- Gmail account with App Password (or other SMTP provider)
- Email client for testing (Gmail, Outlook, etc.)

**Total Cost**: ~$20-35

### Email Service Requirements
1. **Gmail** (Recommended for beginners):
   - Free account with SMTP access
   - Requires App Password (2FA must be enabled)
   - Supports HTML emails and attachments
   - Port: 465 (SSL) or 587 (TLS)

2. **Outlook/Hotmail**:
   - Free SMTP access
   - Server: smtp.office365.com
   - Port: 587 (TLS)

3. **Yahoo Mail**:
   - Requires App Password
   - Server: smtp.mail.yahoo.com
   - Port: 465 (SSL) or 587 (TLS)

4. **Custom SMTP** (Advanced):
   - Any email provider with SMTP access
   - Corporate email servers
   - Self-hosted mail servers

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

**Pushbutton (Manual Trigger):**
```
Button         ESP32
  Pin 1  →     GPIO 18
  Pin 2  →     GND

Note: Using INPUT_PULLUP, no external resistor needed
```

**LED Status Indicator:**
```
LED            ESP32
  Anode  (+)   →  GPIO 2 (through 220Ω resistor)
  Cathode (-)  →  GND
```

**Optional Buzzer:**
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
    │ VCC ├──────────┤ 5V              GPIO 18  ├────┐
    │ OUT ├──────────┤ GPIO 5                   │    │
    │ GND ├──────────┤ GND                      │  [BTN]
    └─────┘          │                          │    │
                     │                          │  GND
    ┌─────┐          │                          │
    │     │          │                          │
    │[BTN]├──────────┤ GPIO 18                  │
    │     │          │                          │
    └──┬──┘          │                      USB │
       │             │                          │
      GND            └──────────────────────────┘
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
    │              │         │ 18 ├────────┐
    │              │         │    │      Button
    │              │         │ 5V │        │
    │              │         │GND │        │
    ├──────────────┼─────────┤    ├────────┘
    │              │         └────┘
    │              │
    - (Black)      │
    └──────────────┴──────────────────────────────
    GND
    
    PIR Sensor
    ┌─────────┐
    │  VCC    ├─→ + (5V)
    │  OUT    ├─→ GPIO 5
    │  GND    ├─→ -
    └─────────┘
```

## 💻 Code

### Complete Sketch: `05-email-notifier.ino`

The complete code is provided in the `.ino` file. Key sections include:

#### Configuration Section
```cpp
// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Email SMTP Settings (Gmail example)
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465  // SSL port (use 587 for TLS)

// Sender Email Account
#define SENDER_EMAIL "your_email@gmail.com"
#define SENDER_PASSWORD "your_app_password"  // App Password!
#define SENDER_NAME "ESP32 Security System"

// Recipient Email
#define RECIPIENT_EMAIL "recipient@example.com"
#define RECIPIENT_NAME "Homeowner"
```

#### Email Trigger Conditions
```cpp
// Temperature thresholds
TEMP_HIGH_THRESHOLD = 30.0°C
TEMP_LOW_THRESHOLD = 15.0°C
HUMIDITY_HIGH_THRESHOLD = 80.0%

// Timing
SENSOR_CHECK_INTERVAL = 30 seconds
EMAIL_COOLDOWN = 5 minutes
MOTION_TIMEOUT = 5 seconds
```

## 🚀 How to Use

### 1. Setup Gmail App Password (REQUIRED)

Gmail requires App Passwords for third-party applications. Follow these steps:

#### Enable 2-Factor Authentication
1. Go to [Google Account Security](https://myaccount.google.com/security)
2. Under "Signing in to Google", click **2-Step Verification**
3. Follow the setup process if not already enabled

#### Generate App Password
1. Go to [App Passwords](https://myaccount.google.com/apppasswords)
2. You may need to sign in again
3. Select app: **Mail**
4. Select device: **Other (Custom name)**
5. Enter name: **ESP32 Notifier**
6. Click **Generate**
7. **Copy the 16-character password** (format: `xxxx xxxx xxxx xxxx`)
8. **Remove spaces** when entering in code: `xxxxxxxxxxxxxxxx`

**Important Notes:**
- ⚠️ **Never use your regular Gmail password** - it won't work!
- 🔐 The App Password can only be viewed once - save it securely
- 🗑️ You can revoke App Passwords anytime from the same page
- ✅ Each device should use its own unique App Password

#### Visual Guide
```
Google Account → Security → 2-Step Verification → Enable
                             ↓
Google Account → Security → App passwords → Generate
                             ↓
                    Select "Mail" and "Other"
                             ↓
                    Copy 16-character password
                             ↓
            Paste into SENDER_PASSWORD (no spaces!)
```

### 2. Alternative SMTP Providers

**Outlook/Hotmail Configuration:**
```cpp
#define SMTP_HOST "smtp.office365.com"
#define SMTP_PORT 587  // Use TLS
#define SENDER_EMAIL "your_email@outlook.com"
#define SENDER_PASSWORD "your_password"  // Regular password
```

**Yahoo Mail Configuration:**
```cpp
#define SMTP_HOST "smtp.mail.yahoo.com"
#define SMTP_PORT 465  // SSL port
#define SENDER_EMAIL "your_email@yahoo.com"
#define SENDER_PASSWORD "app_password"  // Generate from Yahoo security
```

**Generic SMTP Configuration:**
```cpp
#define SMTP_HOST "mail.yourdomain.com"
#define SMTP_PORT 587  // Or 465 for SSL
#define SENDER_EMAIL "alert@yourdomain.com"
#define SENDER_PASSWORD "your_password"
// May need to configure authentication method
```

### 3. Library Installation

Open Arduino IDE and install required libraries:
1. Go to **Sketch → Include Library → Manage Libraries**
2. Search and install:
   - **"ESP Mail Client"** by Mobizt (v3.4.0+)
   - **"DHT sensor library"** by Adafruit
   - **"Adafruit Unified Sensor"**

**Important**: Make sure to get the latest version of ESP_Mail_Client for best compatibility and security features.

### 4. Hardware Setup

1. Assemble the circuit according to the wiring diagram
2. Double-check all connections:
   - DHT22 to 3.3V (not 5V!)
   - PIR sensor to 5V
   - LED resistor (220Ω)
   - DHT22 pull-up resistor (10kΩ)
3. Connect button with INPUT_PULLUP (no external resistor needed)
4. Connect ESP32 to computer via USB

### 5. Configure the Code

Edit these lines in the sketch:

```cpp
// WiFi Settings
const char* WIFI_SSID = "YourNetworkName";
const char* WIFI_PASSWORD = "YourWiFiPassword";

// SMTP Settings
#define SMTP_HOST "smtp.gmail.com"  // Change if not Gmail
#define SMTP_PORT 465  // 465 for SSL, 587 for TLS

// Email Credentials
#define SENDER_EMAIL "youremail@gmail.com"
#define SENDER_PASSWORD "aaaa bbbb cccc dddd"  // App Password!
#define SENDER_NAME "ESP32 Security"  // Customize sender name

// Recipient
#define RECIPIENT_EMAIL "where-to-send@example.com"
#define RECIPIENT_NAME "Your Name"

// Optional: Adjust thresholds
const float TEMP_HIGH_THRESHOLD = 30.0;  // °C
const float TEMP_LOW_THRESHOLD = 15.0;   // °C
const float HUMIDITY_HIGH_THRESHOLD = 80.0;  // %

// Optional: Adjust timing
const unsigned long SENSOR_CHECK_INTERVAL = 30000;  // 30 sec
const unsigned long EMAIL_COOLDOWN = 300000;  // 5 min
```

**Timezone Configuration** (Optional):
```cpp
config.time.gmt_offset = -5;  // EST (UTC-5)
config.time.day_light_offset = 1;  // Add 1 hour for DST
// Common offsets: PST=-8, MST=-7, CST=-6, EST=-5
```

### 6. Upload and Monitor

1. Select **Tools → Board → ESP32 Dev Module**
2. Select correct **Port**
3. Click **Upload** (→)
4. Open **Serial Monitor** (set to 115200 baud)
5. Wait for WiFi connection
6. You should receive a startup email!

### 7. Testing the System

**Automatic Triggers:**
1. **Temperature Alert**: Heat DHT22 sensor with your hand (or use ice pack)
2. **Motion Alert**: Wave hand in front of PIR sensor
3. **Humidity Alert**: Breathe on DHT22 sensor (increases humidity)

**Manual Trigger:**
1. Press the button connected to GPIO 18
2. LED should light up briefly
3. Email should arrive within 10-30 seconds

**Cooldown Testing:**
1. Trigger an alert (any type)
2. Try triggering again immediately
3. System should show cooldown message in Serial Monitor
4. Wait 5 minutes, then trigger again successfully

## 📊 Example Output

### Serial Monitor Output
```
═══════════════════════════════════════
       ESP32 Email Notifier
═══════════════════════════════════════

✓ DHT sensor initialized
Connecting to WiFi: MyHomeNetwork
......
✓ WiFi connected!
IP Address: 192.168.1.50
Signal Strength: -48 dBm
Configuring SMTP...
✓ SMTP configured
✓ Email system ready!

Sending startup notification...
> C: ESP Mail Client v3.4.0
> C: Connecting to SMTP server...
> C: Host > smtp.gmail.com
> C: Port > 465
✓ Message sent successfully!
✓ Startup email sent successfully!

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Checking sensors...
Temperature: 23.5°C
Humidity: 45.2%
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚶 Motion detected! Sending alert...
Sending motion alert email...
> C: Connecting to SMTP server...
✓ Message sent successfully!
✓ Motion alert email sent successfully!

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Checking sensors...
Temperature: 31.2°C
Humidity: 48.5%
⚠️  Temperature too high!
Sending sensor alert email: High Temperature Alert
✓ Message sent successfully!
✓ Sensor alert email sent successfully!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🔘 Manual trigger button pressed!
Sending manual alert email...
✓ Message sent successfully!
✓ Manual alert email sent successfully!
```

### Example Email: Startup Notification

**Subject:** ESP32 Security System - Started

**From:** ESP32 Security System <youremail@gmail.com>

**Body (HTML):**
```
🔔 System Startup

Your ESP32 security system has been started successfully.

System Information:
• Device: ESP32 Security System
• IP Address: 192.168.1.50
• WiFi Signal: -48 dBm

This is an automated message from your ESP32 device.
```

### Example Email: Motion Alert

**Subject:** ⚠️ MOTION DETECTED - ESP32 Alert

**From:** ESP32 Security System <youremail@gmail.com>

**Priority:** High

**Body (HTML):**
```
🚨 Motion Detected!

Motion has been detected by your ESP32 security system.

Details:
• Time: 0 days, 01:23:45
• Sensor: PIR Motion Detector
• Location: Main Area
• Temperature: 23.5°C
• Humidity: 45.2%

Please check your security system immediately.
```

### Example Email: Temperature Alert

**Subject:** ⚠️ High Temperature Alert - ESP32 Alert

**From:** ESP32 Security System <youremail@gmail.com>

**Priority:** High

**Body (HTML):**
```
⚠️ Sensor Alert

Alert Type: High Temperature Alert

Current Readings:
• Temperature: 31.2°C (Above threshold: 30.0°C)
• Humidity: 48.5%
• Time: 0 days, 02:15:30

Please check your system and take appropriate action.
```

### Example Email: Manual Alert

**Subject:** 🔔 Manual Alert - ESP32 Notification

**From:** ESP32 Security System <youremail@gmail.com>

**Body (HTML):**
```
🔔 Manual Alert

A manual alert has been triggered from your ESP32 system.

Current Status:
• Temperature: 23.5°C
• Humidity: 45.2%
• Motion: None
• Time: 0 days, 03:45:12

This alert was manually triggered by pressing the button.
```

## 🎓 Understanding the Code

### SMTP Protocol Basics

**Simple Mail Transfer Protocol (SMTP)** is the standard protocol for sending emails. Here's how it works:

```
ESP32 → SMTP Client → SMTP Server (Gmail) → Recipient's Email Server → Inbox
```

**Connection Process:**
1. **TCP Connection**: ESP32 connects to SMTP server on port 465 (SSL) or 587 (TLS)
2. **Greeting**: Server sends greeting message
3. **Authentication**: ESP32 provides email and password
4. **Message Transfer**: ESP32 sends email headers and content
5. **Confirmation**: Server confirms receipt
6. **Close Connection**: Session ends

**Security Layers:**
- **SSL (Port 465)**: Encrypted from start (implicit TLS)
- **TLS (Port 587)**: Starts unencrypted, upgrades to encryption (explicit TLS/STARTTLS)
- **Authentication**: Username/password verification
- **App Passwords**: More secure than regular passwords

### ESP_Mail_Client Library

The ESP_Mail_Client library simplifies email operations:

**Key Components:**

```cpp
SMTPSession smtp;              // SMTP connection handler
Session_Config config;         // Connection configuration
SMTP_Message message;          // Email message object
```

**Configuration Setup:**
```cpp
config.server.host_name = SMTP_HOST;      // SMTP server address
config.server.port = SMTP_PORT;           // Port number
config.login.email = SENDER_EMAIL;        // Authentication email
config.login.password = SENDER_PASSWORD;  // App password
config.time.ntp_server = "pool.ntp.org";  // Time sync for headers
```

**Message Creation:**
```cpp
SMTP_Message message;
message.sender.name = "ESP32 System";
message.sender.email = "sender@gmail.com";
message.subject = "Alert!";
message.addRecipient("Recipient", "recipient@email.com");
message.html.content = "<h1>Alert</h1><p>Message</p>";
message.priority = esp_mail_smtp_priority_high;
```

**Sending Process:**
```cpp
// 1. Connect to SMTP server
if (!smtp.connect(&config)) {
  Serial.println("Connection failed!");
  return;
}

// 2. Send the email
if (!MailClient.sendMail(&smtp, &message)) {
  Serial.println("Send failed!");
} else {
  Serial.println("Email sent!");
}

// 3. Close the session
smtp.closeSession();
```

### HTML Email Formatting

HTML emails provide rich formatting and professional appearance:

**Basic Structure:**
```cpp
String htmlMsg = "<div style='font-family: Arial, sans-serif;'>";
htmlMsg += "<h2 style='color: #F44336;'>Alert Title</h2>";
htmlMsg += "<p>Alert description</p>";
htmlMsg += "<ul>";
htmlMsg += "<li><strong>Item:</strong> Value</li>";
htmlMsg += "</ul>";
htmlMsg += "</div>";
```

**Color Schemes Used:**
- `#F44336` - Red (critical alerts)
- `#FF9800` - Orange (warnings)
- `#4CAF50` - Green (normal status)
- `#2196F3` - Blue (information)
- `#666666` - Gray (secondary text)

**Email Priorities:**
```cpp
// High priority (shows as important in email clients)
message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;

// Normal priority (default)
message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

// Low priority
message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
```

### Intelligent Cooldown System

Prevents email spam and respects SMTP rate limits:

```cpp
const unsigned long EMAIL_COOLDOWN = 300000;  // 5 minutes
unsigned long lastEmailSent = 0;

bool canSendEmail() {
  unsigned long currentTime = millis();
  if (currentTime - lastEmailSent >= EMAIL_COOLDOWN) {
    return true;  // Cooldown period passed
  }
  
  // Calculate and show remaining time
  unsigned long remaining = (EMAIL_COOLDOWN - (currentTime - lastEmailSent)) / 1000;
  Serial.printf("Cooldown: %lu seconds remaining\n", remaining);
  return false;
}
```

**Why Cooldown is Important:**
- Prevents hundreds of emails from rapid sensor triggers
- Respects Gmail's sending limits (100-500 emails/day for free accounts)
- Reduces battery usage (for portable applications)
- Makes alerts more actionable (not overwhelming)

### Multi-Trigger System

Different events trigger different email types:

```cpp
// 1. Motion Detection - Immediate alert
if (motionDetected && !motionAlertSent && canSendEmail()) {
  sendMotionAlert();
  motionAlertSent = true;
}

// 2. Temperature/Humidity - Periodic checks
if (temperature > TEMP_HIGH_THRESHOLD && canSendEmail()) {
  sendSensorAlert("High Temperature", temp, humidity);
}

// 3. Manual Trigger - On-demand status
if (buttonPressed && canSendEmail()) {
  sendManualAlert();
}

// 4. Startup - System initialization
void setup() {
  // ... initialization code ...
  sendStartupEmail();
}
```

### Error Handling

Robust error handling ensures reliability:

```cpp
// Check sensor readings
float temp = dht.readTemperature();
if (isnan(temp)) {
  Serial.println("Sensor error!");
  return;  // Skip this reading
}

// Check SMTP connection
if (!smtp.connect(&config)) {
  Serial.printf("Connection error: %s\n", smtp.errorReason().c_str());
  return;  // Will retry on next trigger
}

// Check email send status
if (!MailClient.sendMail(&smtp, &message)) {
  Serial.printf("Send error: %s\n", smtp.errorReason().c_str());
} else {
  Serial.println("Email sent successfully!");
  lastEmailSent = millis();  // Only update on success
}

// Always close session
smtp.closeSession();
```

### NTP Time Synchronization

The library uses NTP (Network Time Protocol) for accurate timestamps:

```cpp
config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
config.time.gmt_offset = 0;  // UTC timezone
config.time.day_light_offset = 0;  // No DST

// Time is used for:
// - Email Date headers (required by SMTP)
// - SSL certificate validation
// - Accurate timestamps in email content
```

## 🔧 Customization Ideas

### 1. Send SMS via Email-to-SMS Gateways

Most carriers provide email-to-SMS gateways:

```cpp
// Add SMS recipient alongside email
void sendMotionAlert() {
  SMTP_Message message;
  // ... regular setup ...
  
  // Email recipient
  message.addRecipient("Homeowner", "user@gmail.com");
  
  // SMS recipients (replace with your number)
  message.addRecipient("Mobile", "5551234567@txt.att.net");  // AT&T
  // message.addRecipient("Mobile", "5551234567@tmomail.net");  // T-Mobile
  // message.addRecipient("Mobile", "5551234567@vtext.com");  // Verizon
  // message.addRecipient("Mobile", "5551234567@messaging.sprintpcs.com");  // Sprint
  
  // Use plain text for SMS (HTML not supported)
  String plainText = "MOTION DETECTED at " + getFormattedTime();
  message.text.content = plainText.c_str();
  message.text.charSet = "utf-8";
  
  // ... send email ...
}
```

**Major US Carrier Gateways:**
- AT&T: `phonenumber@txt.att.net`
- T-Mobile: `phonenumber@tmomail.net`
- Verizon: `phonenumber@vtext.com`
- Sprint: `phonenumber@messaging.sprintpcs.com`
- Google Fi: `phonenumber@msg.fi.google.com`

### 2. Multiple Recipients

Send to multiple people:

```cpp
// Define recipients array
const char* recipients[][2] = {
  {"John Doe", "john@example.com"},
  {"Jane Smith", "jane@example.com"},
  {"Security Team", "security@company.com"}
};
const int NUM_RECIPIENTS = 3;

void sendAlert() {
  SMTP_Message message;
  // ... setup ...
  
  // Add all recipients
  for (int i = 0; i < NUM_RECIPIENTS; i++) {
    message.addRecipient(recipients[i][0], recipients[i][1]);
  }
  
  // Add CC recipients
  message.addCc("Manager", "manager@company.com");
  
  // Add BCC recipients (hidden)
  message.addBcc("Archive", "archive@company.com");
  
  // ... send ...
}
```

### 3. Add Photo Attachments

Capture images with ESP32-CAM and send as attachments:

```cpp
#include <esp_camera.h>

void sendMotionAlertWithPhoto() {
  // Capture photo
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  
  SMTP_Message message;
  // ... setup message ...
  
  // Attach the photo
  SMTP_Attachment att;
  att.descr.filename = "motion.jpg";
  att.descr.mime = "image/jpeg";
  att.blob.data = fb->buf;
  att.blob.size = fb->len;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  
  message.addAttachment(att);
  
  // Send email
  MailClient.sendMail(&smtp, &message);
  
  // Return frame buffer
  esp_camera_fb_return(fb);
}
```

### 4. Data Logging Attachment

Send CSV log files:

```cpp
#include <SPIFFS.h>

void sendDailyReport() {
  // Create CSV data
  String csv = "Time,Temperature,Humidity,Motion\n";
  csv += "2024-01-15 10:00,23.5,45.2,0\n";
  csv += "2024-01-15 10:30,24.1,44.8,1\n";
  // ... more data ...
  
  SMTP_Message message;
  message.subject = "Daily Sensor Report";
  // ... setup ...
  
  // Attach CSV
  SMTP_Attachment att;
  att.descr.filename = "sensor_log.csv";
  att.descr.mime = "text/csv";
  att.blob.data = (uint8_t*)csv.c_str();
  att.blob.size = csv.length();
  
  message.addAttachment(att);
  MailClient.sendMail(&smtp, &message);
}
```

### 5. Scheduled Reports

Send periodic status reports:

```cpp
const unsigned long REPORT_INTERVAL = 86400000;  // 24 hours
unsigned long lastReport = 0;

void loop() {
  unsigned long currentTime = millis();
  
  // Send daily report
  if (currentTime - lastReport >= REPORT_INTERVAL) {
    sendDailyStatusReport();
    lastReport = currentTime;
  }
  
  // ... rest of loop ...
}

void sendDailyStatusReport() {
  SMTP_Message message;
  message.subject = "Daily Status Report - " + getDate();
  // ... setup ...
  
  String htmlMsg = "<h2>24-Hour Summary</h2>";
  htmlMsg += "<ul>";
  htmlMsg += "<li>Average Temperature: " + String(avgTemp, 1) + "°C</li>";
  htmlMsg += "<li>Average Humidity: " + String(avgHumidity, 1) + "%</li>";
  htmlMsg += "<li>Motion Events: " + String(motionCount) + "</li>";
  htmlMsg += "<li>Alerts Sent: " + String(alertCount) + "</li>";
  htmlMsg += "</ul>";
  
  message.html.content = htmlMsg.c_str();
  MailClient.sendMail(&smtp, &message);
}
```

### 6. Different Alert Levels

Implement severity-based alerting:

```cpp
enum AlertLevel {
  INFO,
  WARNING,
  CRITICAL
};

void sendAlert(AlertLevel level, String title, String details) {
  SMTP_Message message;
  
  // Configure based on level
  switch(level) {
    case CRITICAL:
      message.subject = "🚨 CRITICAL: " + title;
      message.priority = esp_mail_smtp_priority_high;
      // Send to all recipients including management
      message.addRecipient("Owner", "owner@example.com");
      message.addRecipient("Manager", "manager@example.com");
      break;
      
    case WARNING:
      message.subject = "⚠️ WARNING: " + title;
      message.priority = esp_mail_smtp_priority_normal;
      // Send to primary recipient only
      message.addRecipient("Owner", "owner@example.com");
      break;
      
    case INFO:
      message.subject = "ℹ️ INFO: " + title;
      message.priority = esp_mail_smtp_priority_low;
      // Send to monitoring address
      message.addRecipient("Monitor", "monitor@example.com");
      break;
  }
  
  // ... rest of message setup and send ...
}

// Usage:
sendAlert(CRITICAL, "Motion Detected", "Intruder alert!");
sendAlert(WARNING, "High Temperature", "Temp: 32°C");
sendAlert(INFO, "Daily Report", "All systems normal");
```

### 7. Dynamic Configuration via Web Interface

Create web interface to change settings:

```cpp
#include <WebServer.h>

WebServer server(80);
String alertEmail = "default@example.com";
float tempThreshold = 30.0;

void setup() {
  // ... other setup ...
  
  // Setup web server
  server.on("/", handleRoot);
  server.on("/config", HTTP_POST, handleConfig);
  server.begin();
}

void loop() {
  server.handleClient();
  // ... rest of loop ...
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 Email Config</h1>";
  html += "<form action='/config' method='POST'>";
  html += "Alert Email: <input name='email' value='" + alertEmail + "'><br>";
  html += "Temp Threshold: <input name='temp' value='" + String(tempThreshold) + "'><br>";
  html += "<input type='submit' value='Save'>";
  html += "</form></body></html>";
  server.send(200, "text/html", html);
}

void handleConfig() {
  alertEmail = server.arg("email");
  tempThreshold = server.arg("temp").toFloat();
  server.send(200, "text/html", "Settings saved!<br><a href='/'>Back</a>");
}
```

### 8. Email Templates System

Use templates for consistent formatting:

```cpp
class EmailTemplate {
  private:
    String getHeader(String title, String color) {
      String html = "<div style='font-family: Arial, sans-serif; max-width: 600px;'>";
      html += "<div style='background: " + color + "; color: white; padding: 20px;'>";
      html += "<h1>" + title + "</h1>";
      html += "</div>";
      return html;
    }
    
    String getFooter() {
      return "<div style='background: #f0f0f0; padding: 10px; margin-top: 20px;'>"
             "<small>ESP32 Security System | " + WiFi.localIP().toString() + "</small>"
             "</div></div>";
    }
    
  public:
    String createAlert(String title, String message, String color) {
      String html = getHeader(title, color);
      html += "<div style='padding: 20px;'>";
      html += "<p>" + message + "</p>";
      html += "</div>";
      html += getFooter();
      return html;
    }
    
    String createTable(String title, String headers[], String data[][10], int rows) {
      String html = getHeader(title, "#2196F3");
      html += "<table style='width: 100%; border-collapse: collapse;'>";
      html += "<tr style='background: #f0f0f0;'>";
      // ... add headers and data ...
      html += "</table>";
      html += getFooter();
      return html;
    }
};

// Usage:
EmailTemplate tmpl;
String alertHtml = tmpl.createAlert("Motion Detected", "Motion in living room", "#F44336");
```

### 9. Integrate with IFTTT

Trigger IFTTT applets for additional actions:

```cpp
#include <HTTPClient.h>

void triggerIFTTT(String event, String value1, String value2, String value3) {
  HTTPClient http;
  
  String url = "https://maker.ifttt.com/trigger/" + event + "/with/key/YOUR_IFTTT_KEY";
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  String json = "{\"value1\":\"" + value1 + "\",\"value2\":\"" + value2 + "\",\"value3\":\"" + value3 + "\"}";
  
  int httpCode = http.POST(json);
  
  if (httpCode > 0) {
    Serial.printf("IFTTT triggered: %d\n", httpCode);
  }
  
  http.end();
}

// Usage:
void checkMotion() {
  if (motionDetected) {
    sendMotionAlert();  // Email
    triggerIFTTT("motion_detected", "Living Room", "23.5C", "45%");  // IFTTT
  }
}
```

## 🐛 Troubleshooting

### SMTP Connection Issues

**Error: "Connection failed" or "Connection timeout":**
- ✅ Check WiFi connection is stable
- ✅ Verify SMTP server address (smtp.gmail.com)
- ✅ Confirm port number (465 for SSL, 587 for TLS)
- ✅ Check firewall settings - may block outgoing SMTP ports
- ✅ Some networks block SMTP ports (corporate, hotels, public WiFi)
- ✅ Try different port: if 465 fails, use 587 and vice versa
- ✅ Ping SMTP server to verify it's reachable:
  ```bash
  ping smtp.gmail.com
  telnet smtp.gmail.com 587
  ```

**Error: "Authentication failed":**
- ⚠️ **Most common issue**: Using regular Gmail password instead of App Password
- ✅ Generate new App Password from Google Account settings
- ✅ Remove all spaces from App Password: `aaaa bbbb cccc dddd` → `aaaabbbbccccdddd`
- ✅ Ensure 2FA is enabled on Gmail account (required for App Passwords)
- ✅ Check sender email matches the account that generated App Password
- ✅ Verify no typos in email address or password
- ✅ For Outlook: use regular password (App Passwords not needed)

**Error: "TLS/SSL handshake failed":**
- ✅ ESP32 time not synchronized - library uses NTP, wait 10-20 seconds after WiFi connects
- ✅ Try switching ports: 465 (SSL) ↔ 587 (TLS)
- ✅ Update ESP_Mail_Client library to latest version
- ✅ Check if ESP32 has enough free heap memory:
  ```cpp
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  ```
- ✅ For older ESP32 boards, may need to update Arduino core

### Gmail-Specific Issues

**Error: "Username and Password not accepted":**
- 🔐 App Password not generated correctly
- 🔐 2-Step Verification not enabled
- 🔐 Using regular password instead of App Password
- 🔐 Wrong Gmail account (using different account than intended)

**Emails not arriving:**
- ✅ Check Gmail Spam folder
- ✅ Check "All Mail" folder
- ✅ Verify recipient email is correct
- ✅ Gmail may block suspicious login attempts - check Gmail security alerts
- ✅ Try sending email to yourself first for testing
- ✅ Check Gmail sending limits (100-500 emails/day)

**"Less secure app access" message:**
- ℹ️ This is outdated - Gmail removed this option
- ✅ Must use App Passwords instead (2FA required)
- ✅ Never try to enable "less secure apps" - it won't work

### Email Content Issues

**HTML not rendering:**
- ✅ Some email clients show plain text by default
- ✅ Check message.html.content is set (not message.text.content)
- ✅ Ensure HTML is valid (closing tags, proper escaping)
- ✅ Test with simple HTML first:
  ```cpp
  message.html.content = "<h1>Test</h1><p>Works!</p>";
  ```

**Special characters appearing as "?":**
- ✅ Set character encoding:
  ```cpp
  message.html.charSet = "utf-8";
  message.text.charSet = "utf-8";
  ```
- ✅ Use HTML entities for special chars: `&deg;` for °, `&nbsp;` for space

**Email marked as spam:**
- ✅ Add recipients to contact list
- ✅ Avoid spam trigger words in subject ("free", "winner", "urgent", too many capitals)
- ✅ Include plain text version alongside HTML
- ✅ Don't send too many emails too quickly
- ✅ Use proper sender name (not just email address)

### Sensor Issues

**DHT22 returns NaN:**
- ✅ Check 10kΩ pull-up resistor between DATA and VCC
- ✅ Verify DHT22 is connected to 3.3V (not 5V!)
- ✅ Wait at least 2 seconds between readings
- ✅ Try different GPIO pin (avoid strapping pins)
- ✅ Add delays:
  ```cpp
  dht.begin();
  delay(2000);  // Wait for sensor to stabilize
  ```

**PIR sensor constantly triggers or never triggers:**
- ✅ Adjust sensitivity potentiometer (usually orange or blue screw)
- ✅ Adjust time delay potentiometer
- ✅ Check jumper setting (H=retriggering, L=single trigger)
- ✅ Keep away from heat sources, moving air, sunlight
- ✅ Allow 30-60 seconds warm-up time after power-on
- ✅ Test with Serial.println(digitalRead(PIR_PIN));

**Button not working:**
- ✅ Using INPUT_PULLUP, button press should read LOW
- ✅ Check button wiring: one side to GPIO, other to GND
- ✅ Add debounce delay (50-100ms)
- ✅ Test with Serial.println(digitalRead(BUTTON_PIN));

### Memory and Performance Issues

**ESP32 crashes or reboots:**
- ✅ Check power supply - ESP32 needs stable 500mA+ during WiFi
- ✅ Monitor heap memory:
  ```cpp
  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  // ... send email ...
  Serial.printf("Heap after: %d\n", ESP.getFreeHeap());
  ```
- ✅ Close SMTP session after each email:
  ```cpp
  smtp.closeSession();  // IMPORTANT!
  ```
- ✅ Avoid large String concatenations - use smaller HTML

**Emails sending slowly:**
- ℹ️ Normal behavior: SMTP takes 10-30 seconds per email
- ✅ Use async operation if needed (advanced):
  ```cpp
  // Send in background (requires callback handling)
  ```
- ✅ Check WiFi signal strength: Serial.println(WiFi.RSSI());
- ✅ Move ESP32 closer to router

**"Out of memory" or crashes during send:**
- ✅ Reduce HTML email size
- ✅ Don't send large attachments (>100KB)
- ✅ Increase ESP32 heap by reducing other memory usage
- ✅ Send plain text instead of HTML for low-memory situations

### Network Issues

**WiFi disconnects frequently:**
- ✅ Check router signal strength (should be > -70 dBm)
- ✅ Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- ✅ Add auto-reconnect logic:
  ```cpp
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.reconnect();
    delay(5000);
  }
  ```
- ✅ Reduce power saving:
  ```cpp
  WiFi.setSleep(false);
  ```

**Firewall blocking SMTP:**
- ✅ Some corporate/school networks block SMTP ports
- ✅ Try mobile hotspot for testing
- ✅ Check with network administrator
- ✅ Alternative: Use HTTP-based email API (SendGrid, Mailgun)

### Debugging Tips

**Enable detailed SMTP debugging:**
```cpp
// In setup()
smtp.debug(1);  // Basic debug info
// smtp.debug(2);  // Detailed debug info
```

**Test SMTP connection separately:**
```cpp
void testSMTP() {
  Serial.println("Testing SMTP connection...");
  
  if (!smtp.connect(&config)) {
    Serial.println("SMTP connection failed!");
    Serial.println(smtp.errorReason());
    return;
  }
  
  Serial.println("SMTP connected successfully!");
  smtp.closeSession();
}
```

**Monitor each step:**
```cpp
Serial.println("Step 1: Reading sensors");
float temp = dht.readTemperature();
Serial.printf("Temperature: %.1f\n", temp);

Serial.println("Step 2: Checking thresholds");
if (temp > TEMP_HIGH_THRESHOLD) {
  Serial.println("Threshold exceeded!");
}

Serial.println("Step 3: Checking cooldown");
if (canSendEmail()) {
  Serial.println("Step 4: Sending email");
  sendAlert();
}
```

## 📚 Learning Outcomes

After completing this project, you'll understand:
- ✅ SMTP protocol and email communication
- ✅ HTML email formatting and styling
- ✅ Email authentication and security (App Passwords)
- ✅ TLS/SSL encrypted connections
- ✅ Multi-sensor monitoring systems
- ✅ Event-driven programming patterns
- ✅ Intelligent cooldown/rate limiting
- ✅ Error handling and retry logic
- ✅ Time synchronization with NTP
- ✅ Email priority and urgency handling
- ✅ Professional notification systems
- ✅ Real-world IoT alerting strategies

## 🚀 Next Steps

### Beginner Level
1. **Customize Thresholds**: Adjust temperature/humidity limits for your environment
2. **Add More Sensors**: Light sensor, soil moisture, gas sensor
3. **Test Email-to-SMS**: Send alerts to your phone via carrier gateway
4. **Multiple Recipients**: Send to family members or housemates
5. **Change Email Style**: Customize HTML colors and formatting

### Intermediate Level
6. **Add ESP32-CAM**: Send photos with motion alerts
7. **Daily Reports**: Schedule periodic status emails
8. **Web Dashboard**: Create web interface to view sensor data
9. **Database Logging**: Store sensor data in cloud database
10. **IFTTT Integration**: Trigger other smart home actions
11. **Telegram Bot**: Add Telegram notifications alongside email
12. **Battery Operation**: Optimize for battery-powered operation with deep sleep

### Advanced Level
13. **OTA Updates**: Update firmware remotely (Project 06)
14. **Multiple SMTP Accounts**: Load balance across providers
15. **Email Queue System**: Store and retry failed emails
16. **Machine Learning**: Anomaly detection for unusual patterns
17. **OAuth2 Authentication**: Implement modern OAuth2 flow
18. **Custom SMTP Server**: Run your own mail server
19. **Email Parsing**: Receive commands via email (IMAP)
20. **Encryption**: Add end-to-end encryption for sensitive data

### Real-World Projects
- **Home Security System**: Motion + door sensors with photo alerts
- **Server Room Monitor**: Temperature/humidity alerts for IT equipment
- **Greenhouse Monitor**: Automated alerts for plant care
- **Elderly Care System**: Activity monitoring and fall detection
- **Pet Monitor**: Food/water level alerts and activity tracking
- **Equipment Maintenance**: Vibration/temperature monitoring for machinery
- **Parking Space Detector**: Notify when driveway is occupied
- **Package Delivery Alert**: Notify when package arrives
- **Water Leak Detector**: Prevent water damage with early alerts
- **Air Quality Monitor**: Alert when pollution levels are high

## 📖 References

### ESP_Mail_Client Library
- [ESP_Mail_Client GitHub](https://github.com/mobizt/ESP-Mail-Client)
- [ESP_Mail_Client Documentation](https://github.com/mobizt/ESP-Mail-Client#readme)
- [Library Examples](https://github.com/mobizt/ESP-Mail-Client/tree/master/examples)
- [SMTP Examples](https://github.com/mobizt/ESP-Mail-Client/tree/master/examples/SMTP)

### Gmail SMTP Configuration
- [Gmail SMTP Settings](https://support.google.com/mail/answer/7126229)
- [Gmail App Passwords](https://support.google.com/accounts/answer/185833)
- [Enable 2-Step Verification](https://support.google.com/accounts/answer/185839)
- [Gmail Sending Limits](https://support.google.com/mail/answer/22839)

### Email Protocols
- [SMTP Protocol (RFC 5321)](https://tools.ietf.org/html/rfc5321)
- [MIME Format (RFC 2045)](https://tools.ietf.org/html/rfc2045)
- [Email Authentication (SPF, DKIM, DMARC)](https://www.cloudflare.com/learning/email-security/dmarc-dkim-spf/)
- [TLS/SSL for Email](https://www.cloudflare.com/learning/ssl/what-is-ssl-tls-handshake/)

### Other SMTP Providers
- [Outlook SMTP Settings](https://support.microsoft.com/en-us/office/pop-imap-and-smtp-settings-8361e398-8af4-4e97-b147-6c6c4ac95353)
- [Yahoo SMTP Settings](https://help.yahoo.com/kb/SLN4075.html)
- [SendGrid Documentation](https://docs.sendgrid.com/)
- [Mailgun SMTP Guide](https://documentation.mailgun.com/en/latest/user_manual.html#sending-via-smtp)

### HTML Email Best Practices
- [HTML Email Guide](https://www.emailonacid.com/blog/article/email-development/html-email-guide/)
- [Email Client CSS Support](https://www.campaignmonitor.com/css/)
- [Responsive Email Design](https://www.smashingmagazine.com/2011/08/designing-html-emails-responsive/)

### ESP32 Resources
- [ESP32 WiFi Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [ESP32 Deep Sleep Tutorial](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/)
- [ESP32 Power Consumption](https://www.mischianti.org/2021/03/06/esp32-practical-power-saving-management-of-wifi-and-cpu-1/)

### Security Best Practices
- [IoT Security Guidelines](https://www.owasp.org/index.php/IoT_Security_Guidance)
- [Storing Secrets in ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/security/flash-encryption.html)
- [Email Security Best Practices](https://www.eff.org/deeplinks/2013/06/why-metadata-matters)

### Troubleshooting Resources
- [ESP32 Troubleshooting Guide](https://randomnerdtutorials.com/esp32-troubleshooting-guide/)
- [Gmail SMTP Errors](https://support.google.com/mail/answer/7126229#zippy=%2Csmtp-relay-errors)
- [Arduino ESP32 Forum](https://forum.arduino.cc/c/international/31)
- [ESP32 Reddit Community](https://www.reddit.com/r/esp32/)

---

**Project Complete!** 🎉

You've built a professional email notification system that can alert you about important events from anywhere in the world! This project demonstrates real-world IoT communication patterns used in commercial security systems, industrial monitoring, and smart home automation. The ability to send HTML emails with custom formatting, handle multiple trigger conditions, and implement intelligent rate limiting are valuable skills for any IoT developer!
