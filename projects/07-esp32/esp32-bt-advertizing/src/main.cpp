
#include <Arduino.h>
#include <qrcode.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Preferences.h>
#include <WiFi.h>

// ── UUIDs ─────────────────────────────────────────────────────────────────────
#define WAVELET_SERVICE_UUID    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_DEVICE_INFO_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR_COMMAND_UUID       "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define CHAR_WIFI_SSID_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26aa"
#define CHAR_WIFI_PASS_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26ab"
#define CHAR_STATUS_UUID        "beb5483e-36e1-4688-b7f5-ea07361b26ac"
#define CHAR_DEVICE_NAME_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26ad"

// ── Globals ───────────────────────────────────────────────────────────────────
BLEServer*          pServer     = nullptr;
BLECharacteristic*  pStatus     = nullptr;
BLECharacteristic*  pDeviceInfo = nullptr;

bool deviceConnected    = false;
bool isSetupComplete    = false;

String receivedSSID     = "";
String receivedPassword = "";
String receivedName     = "";

// ── NVS ───────────────────────────────────────────────────────────────────────
void saveWifiCredentials(String ssid, String pass) {
    Preferences prefs;
    prefs.begin("wavelet", false);
    prefs.putString("wifi_ssid", ssid);
    prefs.putString("wifi_pass", pass);
    prefs.end();
    Serial.println("[NVS] WiFi credentials saved");
}

void saveDeviceName(String name) {
    Preferences prefs;
    prefs.begin("wavelet", false);
    prefs.putString("device_name", name);
    prefs.end();
    Serial.printf("[NVS] Device name saved: %s\n", name.c_str());
}

void saveSetupComplete() {
    Preferences prefs;
    prefs.begin("wavelet", false);
    prefs.putBool("setup_done", true);
    prefs.end();
    Serial.println("[NVS] Setup marked complete");
}

// ── WiFi Connection ───────────────────────────────────────────────────────────
void attemptWifiConnection() {
    if (receivedSSID.isEmpty() || receivedPassword.isEmpty()) {
        Serial.println("[WiFi] Missing SSID or password, skipping");
        return;
    }

    Serial.printf("[WiFi] Connecting to %s...\n", receivedSSID.c_str());
    pStatus->setValue("WIFI_CONNECTING");
    pStatus->notify();

    WiFi.begin(receivedSSID.c_str(), receivedPassword.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
        saveWifiCredentials(receivedSSID, receivedPassword);
        pStatus->setValue("WIFI_CONNECTED");
        pStatus->notify();
    } else {
        Serial.println("[WiFi] Connection failed — wrong password or out of range");
        pStatus->setValue("WIFI_FAILED");
        pStatus->notify();
        // clear so user can retry
        receivedSSID     = "";
        receivedPassword = "";
    }
}

// ── Server Callbacks ──────────────────────────────────────────────────────────
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
        Serial.println("[BLE] Client connected");
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        Serial.println("[BLE] Client disconnected — restarting advertising");
        BLEDevice::startAdvertising();
    }
};

// ── Characteristic Callbacks ──────────────────────────────────────────────────
class CommandCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
        String value = pChar->getValue().c_str();
        Serial.printf("[BLE] Command: %s\n", value.c_str());

        if (value == "SETUP_COMPLETE") {
            saveSetupComplete();
            isSetupComplete = true;
            pStatus->setValue("READY");
            pStatus->notify();
            Serial.println("[BLE] Setup complete — rebooting into normal mode");
            delay(500);
            ESP.restart();
        }
    }
};

class WifiSSIDCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
        receivedSSID = pChar->getValue().c_str();
        Serial.printf("[BLE] SSID received: %s\n", receivedSSID.c_str());
    }
};

class WifiPassCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
        receivedPassword = pChar->getValue().c_str();
        Serial.printf("[BLE] Password received (length: %d)\n", receivedPassword.length());
        attemptWifiConnection();
    }
};

class DeviceNameCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
        receivedName = pChar->getValue().c_str();
        Serial.printf("[BLE] Device name: %s\n", receivedName.c_str());
        saveDeviceName(receivedName);
    }
};

// ── BLE Init ──────────────────────────────────────────────────────────────────
void initBLE(String deviceModel, String deviceColor) {
    String advertisingName = "WVL-" + deviceModel;
    BLEDevice::init(advertisingName.c_str());
    BLEDevice::setPower(ESP_PWR_LVL_P7);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService* pService = pServer->createService(WAVELET_SERVICE_UUID);

    // ── Device Info (read only) ───────────────────────────────────────────────
    pDeviceInfo = pService->createCharacteristic(
        CHAR_DEVICE_INFO_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    String info = "{\"model\":\"" + deviceModel +
                  "\",\"color\":\"" + deviceColor +
                  "\",\"fw\":\"1.0.0\"}";
    pDeviceInfo->setValue(info.c_str());

    // ── Status (notify) ───────────────────────────────────────────────────────
    pStatus = pService->createCharacteristic(
        CHAR_STATUS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pStatus->addDescriptor(new BLE2902()); // required for notify
    pStatus->setValue("IDLE");

    // ── Command (write) ───────────────────────────────────────────────────────
    BLECharacteristic* pCommand = pService->createCharacteristic(
        CHAR_COMMAND_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pCommand->setCallbacks(new CommandCallback());

    // ── WiFi SSID (write) ─────────────────────────────────────────────────────
    BLECharacteristic* pWifiSSID = pService->createCharacteristic(
        CHAR_WIFI_SSID_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pWifiSSID->setCallbacks(new WifiSSIDCallback());

    // ── WiFi Password (write) ─────────────────────────────────────────────────
    BLECharacteristic* pWifiPass = pService->createCharacteristic(
        CHAR_WIFI_PASS_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pWifiPass->setCallbacks(new WifiPassCallback());

    // ── Device Name (write) ───────────────────────────────────────────────────
    BLECharacteristic* pDeviceName = pService->createCharacteristic(
        CHAR_DEVICE_NAME_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    pDeviceName->setCallbacks(new DeviceNameCallback());

    pService->start();

    // ── Advertising ───────────────────────────────────────────────────────────
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(WAVELET_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.printf("[BLE] Advertising as %s\n", advertisingName.c_str());
}

// ── Setup Entry Point ─────────────────────────────────────────────────────────
void beginSetupMode(String deviceModel, String deviceColor) {
    Serial.println("[WAVELET] First boot — entering BLE setup mode");
    initBLE(deviceModel, deviceColor);
}

  void setup() {
      // ... your existing setup code
      Serial.begin(115200);
  
      Preferences prefs;
      prefs.begin("wavelet", true);
      bool setupDone = prefs.getBool("setup_done", false);
      prefs.end();
  
      //debug only, negate logic in prod
      if (!setupDone) {
          //showSetupQR("WVL-2A4F9C", "strawberry_pink", lv_scr_act());
          beginSetupMode("max", "strawberry_pink");
          printQRToSerial(buildQRPayload("max", "pink"));
      }
  }


void loop() {
  // BLE is interrupt driven, nothing needed here during setup
  delay(10);
}