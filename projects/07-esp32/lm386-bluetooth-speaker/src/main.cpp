#include <Arduino.h>
#include <ESP32-A2DP.h>

// -------- CONFIG --------
#define PWM_PIN 25        // ESP32 pin to LM386 input
#define PWM_CHANNEL 0     // LEDC channel for PWM
#define PWM_FREQ 40000    // 40 kHz PWM frequency
#define PWM_RES 8         // 8-bit resolution

ESP32A2DP_sink a2dp_sink;

// -------- AUDIO CALLBACK --------
void audio_data_callback(const uint8_t *data, uint32_t len) {
  // data is PCM16, stereo interleaved (L,R)
  for (uint32_t i = 0; i < len; i += 4) { // 2 bytes per channel
    // convert 16-bit left channel to 8-bit for PWM
    int16_t sample = (data[i+1] << 8) | data[i];
    uint8_t pwmValue = map(sample, -32768, 32767, 0, 255);
    ledcWrite(PWM_CHANNEL, pwmValue);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth speaker...");

  // PWM setup
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);

  // Bluetooth setup
  a2dp_sink.set_stream_callback(audio_data_callback);
  a2dp_sink.start("ESP32 Speaker");  // Your phone will see this name
}

void loop() {
  // Nothing here, all handled in callback
}
