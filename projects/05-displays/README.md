# Display Projects

This folder contains projects using various display technologies.

## Display Types

### Character Displays
- **LCD 16x2/20x4**: Liquid crystal displays
  - Parallel (6-wire mode, 4-wire mode)
  - I2C adapter (2-wire mode)
- **7-Segment**: Single or multi-digit displays
  - Common cathode/anode
  - TM1637 driver module

### Graphic Displays
- **OLED**: Small, high-contrast displays
  - SSD1306 (128x64, 128x32)
  - I2C or SPI interface
- **TFT LCD**: Color displays
  - ILI9341, ST7735
  - Touch screen variants
- **E-Paper/E-Ink**: Low-power displays

### LED Displays
- **LED Matrix**: 8x8 or larger arrays
  - MAX7219 driver
  - Scrolling text displays
- **RGB LEDs**: Color mixing
  - Individual RGB LEDs
  - WS2812B LED strips (NeoPixels)
  - APA102 LED strips (DotStar)

## Key Concepts

- Display libraries (LiquidCrystal, U8g2, Adafruit_GFX)
- Text and graphics rendering
- Fonts and custom characters
- Animation techniques
- Multiplexing
- Refresh rates
- Power requirements
- Display initialization

## Common Projects

- Temperature/humidity display
- Clock and timer displays
- System status monitor
- Game displays (Pong, Snake)
- Scrolling message board
- RGB mood lighting
- Weather information display
- Menu systems

## Library Recommendations

- **LiquidCrystal**: Built-in library for LCD displays
- **LiquidCrystal_I2C**: I2C LCD displays
- **U8g2**: Universal graphics library for OLED/LCD
- **Adafruit_SSD1306**: OLED displays
- **Adafruit_GFX**: Graphics primitives
- **FastLED**: LED strip control
- **Adafruit_NeoPixel**: WS2812B LED strips
- **TFT_eSPI**: Fast TFT displays

## Wiring Tips

- Check I2C address with I2C scanner
- Use appropriate contrast/brightness settings
- Consider power consumption for large displays
- Use level shifters for 3.3V displays
- Keep display cables short
