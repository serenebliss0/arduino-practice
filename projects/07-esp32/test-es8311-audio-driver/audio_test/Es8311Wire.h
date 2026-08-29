#pragma once

#include <stdint.h>

// Es8311Codec
//
// Wire-based port of Espressif's ES8311 reference driver. The original
// driver talks I2C via the new i2c_master_dev_handle_t API, which can't
// safely coexist on the same bus as the FT6336 touch driver (which uses
// the legacy Wire API). This version does register I/O over Wire instead,
// so it can share the bus that FT6336 already owns.
//
// Only the DAC/output path is exposed -- Mini has no mic, so ADC-side
// controls from the original driver were left out on purpose.

#define ES8311_ADDR_CE_LOW  0x18u
#define ES8311_ADDR_CE_HIGH 0x19u

struct Es8311ClockConfig
{
    bool mclk_inverted;
    bool sclk_inverted;
    bool mclk_from_mclk_pin; // true: MCLK pin. false: derive from BCLK/SCK pin.
    int  mclk_frequency;     // Hz. Ignored if mclk_from_mclk_pin is false.
    int  sample_frequency;   // Hz
};

enum class Es8311Resolution : uint8_t
{
    Res16 = 16,
    Res18 = 18,
    Res20 = 20,
    Res24 = 24,
    Res32 = 32
};

class Es8311Codec
{
public:
    // i2cAddr: ES8311_ADDR_CE_LOW (0x18) or ES8311_ADDR_CE_HIGH (0x19)
    // depending on how the board wires the CE pin.
    // Assumes Wire.begin() has already been called by whoever owns the bus
    // (on Mini, that's the touch driver -- see AudioManager comments).
    bool init(uint8_t i2cAddr, const Es8311ClockConfig& clkCfg,
              Es8311Resolution resIn, Es8311Resolution resOut);

    // Call again at runtime if you need to switch sample rate without a
    // full re-init (mclk_from_mclk_pin config from init() still applies).
    bool sampleFrequencyConfig(int mclkFrequency, int sampleFrequency);

    bool volumeSet(int volume, int* volumeSetOut = nullptr); // 0-100
    bool volumeGet(int* volumeOut);
    bool voiceMute(bool enable);
    bool microphoneConfig(bool digitalMic); // Mini: always call with false

private:
    uint8_t addr = ES8311_ADDR_CE_LOW;

    bool writeReg(uint8_t reg, uint8_t val);
    bool readReg(uint8_t reg, uint8_t* valOut);

    bool clockConfig(const Es8311ClockConfig& clkCfg, Es8311Resolution res);
    bool fmtConfig(Es8311Resolution resIn, Es8311Resolution resOut);
};
