/*
 * ESP32 Bluetooth Speaker
 * 
 * Turn your ESP32 into a Bluetooth speaker that receives audio
 * wirelessly and plays it through a speaker/amplifier via I2S.
 * 
 * Features:
 * - Bluetooth A2DP audio receiver (sink)
 * - High-quality I2S audio output
 * - Connection status monitoring
 * - Volume control via AVRCP
 * - Auto-reconnection
 * - Serial debug output
 * 
 * Hardware:
 * - ESP32 Development Board
 * - MAX98357A I2S Amplifier Module (or PCM5102 DAC)
 * - 4Ω-8Ω Speaker
 * - Connections:
 *   MAX98357A VIN   → ESP32 5V (or 3.3V)
 *   MAX98357A GND   → ESP32 GND
 *   MAX98357A DIN   → ESP32 GPIO 25 (I2S Data)
 *   MAX98357A BCLK  → ESP32 GPIO 26 (I2S Bit Clock)
 *   MAX98357A LRC   → ESP32 GPIO 22 (I2S Word Select)
 *   MAX98357A GAIN  → GND (9dB) or VIN (15dB) or Float (12dB)
 *   Speaker         → MAX98357A OUT+ and OUT-
 * 
 * Libraries Required:
 * - ESP32-A2DP by Phil Schatzmann
 *   Install via: Tools → Manage Libraries → Search "ESP32-A2DP"
 * 
 * A2DP Profile:
 * - Receives Bluetooth audio using A2DP (Advanced Audio Distribution Profile)
 * - Outputs via I2S (Inter-IC Sound) protocol
 * - Sample Rate: 44.1 kHz (CD quality)
 * - Format: 16-bit stereo PCM
 * 
 * Usage:
 * 1. Upload code to ESP32
 * 2. Power on and wait for "Ready" message
 * 3. Connect from your phone/computer via Bluetooth
 * 4. Play audio - it will stream to the speaker!
 */

#include "BluetoothA2DPSink.h"

// ===== CONFIGURATION =====
// CHANGE THESE settings as needed

// Bluetooth device name (appears when scanning)
#define BT_DEVICE_NAME "ESP32_Speaker"    // Change to your preferred name

// I2S Pin Configuration
// These are the default pins - change if using different GPIO pins
#define I2S_DOUT      25                  // I2S Data Out (DIN on MAX98357A)
#define I2S_BCLK      26                  // I2S Bit Clock (BCLK on MAX98357A)
#define I2S_LRC       22                  // I2S Left/Right Clock (LRC on MAX98357A)

// Optional: LED indicator pin
#define STATUS_LED    2                   // Built-in LED (GPIO 2)
#define LED_ENABLED   true                // Set to false to disable LED

// ===== GLOBAL OBJECTS =====
BluetoothA2DPSink a2dp_sink;              // Bluetooth A2DP sink object

// ===== STATE VARIABLES =====
bool isConnected = false;
bool isPlaying = false;
uint8_t currentVolume = 64;               // 0-127, start at ~50%
unsigned long lastActivityTime = 0;
unsigned long lastLEDUpdate = 0;
bool ledState = false;

// ===== CALLBACK FUNCTIONS =====

/**
 * Connection State Changed Callback
 * Called when a device connects or disconnects
 */
void connection_state_changed(esp_a2d_connection_state_t state, void *ptr) {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
    Serial.println("📱 Device Connected!");
    isConnected = true;
    
    // Turn on status LED
    if (LED_ENABLED) {
      digitalWrite(STATUS_LED, HIGH);
    }
    
  } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
    Serial.println("📱 Device Disconnected");
    isConnected = false;
    isPlaying = false;
    
    // Turn off status LED
    if (LED_ENABLED) {
      digitalWrite(STATUS_LED, LOW);
    }
    
    Serial.println("\nWaiting for connection...");
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

/**
 * Audio State Changed Callback
 * Called when audio playback starts or stops
 */
void audio_state_changed(esp_a2d_audio_state_t state, void *ptr) {
  if (state == ESP_A2D_AUDIO_STATE_STARTED) {
    Serial.println("▶️  Playback Started");
    isPlaying = true;
    lastActivityTime = millis();
    
  } else if (state == ESP_A2D_AUDIO_STATE_STOPPED) {
    Serial.println("⏸️  Playback Stopped");
    isPlaying = false;
    
  } else if (state == ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND) {
    Serial.println("⏸️  Playback Suspended (Remote)");
    isPlaying = false;
  }
}

/**
 * AVRCP Metadata Callback
 * Called when track metadata is received (title, artist, etc.)
 */
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.print("🎵 Metadata - ");
  
  switch (id) {
    case ESP_AVRC_MD_ATTR_TITLE:
      Serial.print("Title: ");
      break;
    case ESP_AVRC_MD_ATTR_ARTIST:
      Serial.print("Artist: ");
      break;
    case ESP_AVRC_MD_ATTR_ALBUM:
      Serial.print("Album: ");
      break;
    case ESP_AVRC_MD_ATTR_GENRE:
      Serial.print("Genre: ");
      break;
    default:
      Serial.print("Other: ");
      break;
  }
  
  Serial.println((char*)text);
}

/**
 * Volume Changed Callback
 * Called when remote device changes volume
 */
void avrc_rn_volume_change(int volume) {
  currentVolume = volume;
  int percentage = (volume * 100) / 127;
  
  Serial.print("🔊 Volume: ");
  Serial.print(percentage);
  Serial.println("%");
  
  // Visual volume indicator
  Serial.print("   [");
  int bars = percentage / 10;
  for (int i = 0; i < 10; i++) {
    if (i < bars) Serial.print("█");
    else Serial.print("░");
  }
  Serial.println("]");
}

/**
 * Optional: Audio Data Stream Callback
 * Process or analyze audio data as it streams
 * Warning: This runs frequently - keep processing minimal!
 */
/*
void read_data_stream(const uint8_t *data, uint32_t length) {
  // data: PCM audio samples (16-bit stereo interleaved)
  // length: number of bytes
  
  // Example: Calculate audio level
  int16_t *samples = (int16_t*)data;
  uint32_t sum = 0;
  
  for (uint32_t i = 0; i < length/2; i++) {
    sum += abs(samples[i]);
  }
  
  uint32_t avg = sum / (length/2);
  
  // Only print occasionally to avoid slowing down audio
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.printf("📊 Audio Level: %u\n", avg);
    lastPrint = millis();
  }
}
*/

// ===== SETUP =====
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);  // Allow serial to stabilize
  
  // Print startup banner
  Serial.println("\n\n");
  Serial.println("═══════════════════════════════════════");
  Serial.println("     ESP32 Bluetooth Speaker");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  
  // Configure status LED
  if (LED_ENABLED) {
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
    Serial.println("✓ Status LED configured (GPIO " + String(STATUS_LED) + ")");
  }
  
  // Configure I2S pins
  Serial.println("Configuring I2S...");
  
  // Create I2S pin configuration
  i2s_pin_config_t my_pin_config = {
    .bck_io_num = I2S_BCLK,              // Bit Clock
    .ws_io_num = I2S_LRC,                // Word Select (Left/Right Clock)
    .data_out_num = I2S_DOUT,            // Data Out
    .data_in_num = I2S_PIN_NO_CHANGE     // Data In (not used)
  };
  
  // Set custom I2S pins
  a2dp_sink.set_pin_config(my_pin_config);
  
  Serial.println("✓ I2S configured");
  Serial.println("  Data Pin:  GPIO " + String(I2S_DOUT));
  Serial.println("  Clock Pin: GPIO " + String(I2S_BCLK));
  Serial.println("  WS Pin:    GPIO " + String(I2S_LRC));
  Serial.println();
  
  // Register callbacks
  Serial.println("Registering callbacks...");
  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_on_audio_state_changed(audio_state_changed);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_avrc_rn_volumechange(avrc_rn_volume_change);
  
  // Optional: Register audio data callback
  // Uncomment if you want to process/analyze audio data
  // Warning: Keep processing minimal to avoid audio glitches
  // a2dp_sink.set_stream_reader(read_data_stream);
  
  Serial.println("✓ Callbacks registered");
  Serial.println();
  
  // Start Bluetooth A2DP Sink
  Serial.println("Starting Bluetooth A2DP...");
  a2dp_sink.start(BT_DEVICE_NAME);
  
  Serial.println("✓ A2DP initialized");
  Serial.println("  Device Name: " + String(BT_DEVICE_NAME));
  Serial.println("  Mode: Sink (Receiver)");
  Serial.println();
  
  Serial.println("═══════════════════════════════════════");
  Serial.println("   ESP32 Bluetooth Speaker Ready!");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  Serial.println("Waiting for Bluetooth connection...");
  Serial.println("Look for '" + String(BT_DEVICE_NAME) + "' in Bluetooth settings");
  Serial.println();
  Serial.println("TIP: Adjust volume on source device");
  Serial.println("     or use media player volume controls");
  Serial.println();
}

// ===== MAIN LOOP =====
void loop() {
  // The ESP32-A2DP library handles everything automatically
  // Audio is received via Bluetooth and sent to I2S automatically
  
  // Optional: Add visual feedback
  if (LED_ENABLED && isPlaying) {
    // Blink LED slowly when playing audio
    if (millis() - lastLEDUpdate > 1000) {
      ledState = !ledState;
      digitalWrite(STATUS_LED, ledState);
      lastLEDUpdate = millis();
    }
  }
  
  // Optional: Print status updates
  static unsigned long lastStatusPrint = 0;
  if (millis() - lastStatusPrint > 10000) {  // Every 10 seconds
    Serial.println();
    Serial.println("Status Check:");
    Serial.print("  Connected: ");
    Serial.println(isConnected ? "Yes" : "No");
    Serial.print("  Playing: ");
    Serial.println(isPlaying ? "Yes" : "No");
    Serial.print("  Volume: ");
    Serial.print((currentVolume * 100) / 127);
    Serial.println("%");
    Serial.print("  Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    Serial.println();
    
    lastStatusPrint = millis();
  }
  
  // Small delay to prevent watchdog issues
  delay(100);
}

// ===== HELPER FUNCTIONS =====

/**
 * Advanced: Implement audio effects
 * Call this from read_data_stream() callback
 */
/*
void apply_volume_effect(int16_t *samples, uint32_t count, float gain) {
  for (uint32_t i = 0; i < count; i++) {
    // Apply gain
    float sample = samples[i] * gain;
    
    // Clip to prevent overflow
    if (sample > 32767) sample = 32767;
    if (sample < -32768) sample = -32768;
    
    samples[i] = (int16_t)sample;
  }
}
*/

/**
 * Advanced: Stereo to mono conversion
 * Useful if using a single speaker
 */
/*
void stereo_to_mono(int16_t *samples, uint32_t stereo_count) {
  // stereo_count is number of stereo samples (L+R pairs)
  for (uint32_t i = 0; i < stereo_count; i++) {
    int16_t left = samples[i * 2];
    int16_t right = samples[i * 2 + 1];
    int16_t mono = (left + right) / 2;
    samples[i * 2] = mono;
    samples[i * 2 + 1] = mono;
  }
}
*/

/**
 * Advanced: Calculate audio level for VU meter
 */
/*
uint16_t calculate_audio_level(const int16_t *samples, uint32_t count) {
  uint32_t sum = 0;
  
  for (uint32_t i = 0; i < count; i++) {
    sum += abs(samples[i]);
  }
  
  return sum / count;
}
*/

/**
 * Advanced: Detect silence for auto-standby
 */
/*
bool is_silent(const int16_t *samples, uint32_t count, uint16_t threshold) {
  uint16_t level = calculate_audio_level(samples, count);
  return level < threshold;
}
*/
