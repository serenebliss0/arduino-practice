// audio_test.ino
//
// Fully isolated audio hardware test for Wavelet Mini.
// No touch, no SD, no BLE, no LVGL -- just: bring up the shared I2C bus,
// talk to the ES8311, bring up I2S, and play a generated tone.
//
// Goal: answer "does the audio path work at all" with nothing else
// running that could interfere or confuse the result.
//
// This sketch is multi-tab: Es8311Wire.h / Es8311Wire.cpp should sit in
// the same sketch folder (Arduino IDE will show them as separate tabs).
//
// Expected serial output if everything's healthy:
//   [I2C] Chip ID: FD=0x83 FE=0x11   <- exact values may vary by revision,
//                                        but should NOT be 0x00 or 0xFF
//   [I2S] Driver installed
//   [Codec] Init OK
//   [Audio] Playing test tone...
//
// If chip ID reads back 0x00 or 0xFF (or the Wire read just times out),
// that's your answer right there -- I2C isn't reaching the codec at all,
// so nothing downstream (I2S, playback) can be trusted regardless of
// what it reports.

#include <Arduino.h>
#include <Wire.h>
#include "driver/i2s.h"
#include "Es8311Wire.h"

// ---- Pins, per the LCDWIKI datasheet ----
#define AUDIO_I2C_SDA    16
#define AUDIO_I2C_SCL    15
#define AUDIO_I2S_MCLK   4
#define AUDIO_I2S_BCK    5
#define AUDIO_I2S_DOUT   8   // matches manufacturer's proven demo, NOT the raw datasheet label (see note below)
#define AUDIO_I2S_WS     7
#define AUDIO_AMP_ENABLE 1

#define TEST_SAMPLE_RATE 44100
#define TEST_VOLUME      70

Es8311Codec codec;
const i2s_port_t kI2SPort = I2S_NUM_0;

// --- Simple chip-ID sanity check, bypassing the Es8311Codec class so we
// can confirm raw I2C works before trusting anything higher-level. ---
bool readChipId(uint8_t addr, uint8_t reg, uint8_t* out)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return false; // repeated start

    if (Wire.requestFrom((int)addr, 1) != 1) return false;
    *out = Wire.read();
    return true;
}

// --- Generate a short sine wave buffer once, then loop-write it to I2S
// continuously. 440Hz (concert A), easy to recognize, easy to tell apart
// from silence/noise/garbage. ---
constexpr int kToneFreq = 440;
constexpr int kSampleRate = TEST_SAMPLE_RATE;
constexpr int kBufferSamples = kSampleRate / kToneFreq; // one full cycle
int16_t toneBuffer[kBufferSamples * 2]; // stereo interleaved

void buildToneBuffer()
{
    for (int i = 0; i < kBufferSamples; i++)
    {
        float angle = (2.0f * PI * i) / kBufferSamples;
        int16_t sample = (int16_t)(sinf(angle) * 12000); // ~35% volume headroom
        toneBuffer[i * 2]     = sample; // left
        toneBuffer[i * 2 + 1] = sample; // right
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1500); // give serial monitor time to connect
    Serial.println("\n=== Wavelet Mini Audio Isolation Test ===");

    // --- Step 1: bring up I2C, verify the codec responds at all ---
    Wire.begin(AUDIO_I2C_SDA, AUDIO_I2C_SCL);
    delay(50);

    uint8_t chipIdHigh = 0, chipIdLow = 0;
    bool idOkHigh = readChipId(ES8311_ADDR_CE_LOW, 0xFD, &chipIdHigh);
    bool idOkLow  = readChipId(ES8311_ADDR_CE_LOW, 0xFE, &chipIdLow);

    if (!idOkHigh || !idOkLow)
    {
        Serial.println("[I2C] FAILED to read chip ID -- codec not responding on the bus.");
        Serial.println("[I2C] Check wiring/pins before going further. Halting.");
        while (true) delay(1000);
    }

    Serial.printf("[I2C] Chip ID: FD=0x%02X FE=0x%02X\n", chipIdHigh, chipIdLow);
    if (chipIdHigh == 0x00 || chipIdHigh == 0xFF)
    {
        Serial.println("[I2C] Chip ID looks invalid (0x00/0xFF) -- likely no real ACK from codec.");
    }

    // --- Step 2: I2S driver bring-up ---
    pinMode(AUDIO_AMP_ENABLE, OUTPUT);
    digitalWrite(AUDIO_AMP_ENABLE, LOW); // enabled (active low per datasheet)

    i2s_config_t i2sConfig = {};
    i2sConfig.mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    i2sConfig.sample_rate          = TEST_SAMPLE_RATE;
    i2sConfig.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT;
    i2sConfig.channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT;
    i2sConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    i2sConfig.intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1;
    i2sConfig.dma_buf_count        = 6;
    i2sConfig.dma_buf_len          = 256;
    i2sConfig.use_apll             = false;
    i2sConfig.tx_desc_auto_clear   = true;
    i2sConfig.mclk_multiple        = I2S_MCLK_MULTIPLE_256;

    if (i2s_driver_install(kI2SPort, &i2sConfig, 0, nullptr) != ESP_OK)
    {
        Serial.println("[I2S] i2s_driver_install FAILED. Halting.");
        while (true) delay(1000);
    }

    i2s_pin_config_t pinConfig = {};
    pinConfig.mck_io_num   = AUDIO_I2S_MCLK;
    pinConfig.bck_io_num   = AUDIO_I2S_BCK;
    pinConfig.ws_io_num    = AUDIO_I2S_WS;
    pinConfig.data_out_num = AUDIO_I2S_DOUT;
    pinConfig.data_in_num  = I2S_PIN_NO_CHANGE;

    if (i2s_set_pin(kI2SPort, &pinConfig) != ESP_OK)
    {
        Serial.println("[I2S] i2s_set_pin FAILED. Halting.");
        while (true) delay(1000);
    }

    Serial.println("[I2S] Driver installed");

    // --- Step 3: codec init ---
    Es8311ClockConfig clkCfg = {};
    clkCfg.mclk_inverted      = false;
    clkCfg.sclk_inverted      = false;
    clkCfg.mclk_from_mclk_pin = true;
    clkCfg.mclk_frequency     = TEST_SAMPLE_RATE * 256;
    clkCfg.sample_frequency   = TEST_SAMPLE_RATE;

    if (!codec.init(ES8311_ADDR_CE_LOW, clkCfg, Es8311Resolution::Res16, Es8311Resolution::Res16))
    {
        Serial.println("[Codec] Init FAILED. Halting.");
        while (true) delay(1000);
    }

    codec.microphoneConfig(false);
    codec.volumeSet(TEST_VOLUME);
    Serial.println("[Codec] Init OK");

    // --- Step 4: play tone ---
    buildToneBuffer();
    Serial.println("[Audio] Playing test tone (440Hz)... you should hear a steady beep.");
}

void loop()
{
    size_t bytesWritten = 0;
    i2s_write(kI2SPort, toneBuffer, sizeof(toneBuffer), &bytesWritten, portMAX_DELAY);
}
