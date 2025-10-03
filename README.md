# Smart Orange Lamp 🍊

Giving a stylish 60s-inspired lamp a 21st-century brain with ESP32 and Apple HomeKit.

> ⚠️ Heads up WIP! This is an active project. I'm currently waiting on some MOSFETs to finalize the build. I'll be adding wiring diagrams and a final component list soon!

## The Story
I had this awesome 60s-inspired mushroom lamp—you've probably seen them all over Pinterest and Instagram. It looked great, but the cheap, built-in controller was a pain.

The biggest problem? I couldn't just use a simple smart switch or smart bulbs. Turning the power off and on would reset all its settings (brightness, color temperature), which would force me to walk over and fiddle with it every single time.

I believe that if something is electronic, it should be smart. Especially with AI agents become more common in our lives, I want to control my environment in a seamless way. It's a perfect opportunity to improve a beautiful object and give it a seconds life.

This project is the result: a complete retrofit that turns a stylish but dumb lamp into a fully integrated HomeKit device.

## What It Does
- 💡 Smart Lighting Control: Integrates directly with Apple HomeKit. Turn the lamp on/off and adjust brightness, color temperature from your devices.
- 🌡️ Temperature & 💧 Humidity Sensing: An onboard DHT11 sensor exposes live temperature and humidity data as separate accessories in the Home app.
- 👆 Physical Button Override: Don't want to use your phone? No problem. The original button is now smarter.

## Features

### LED Control
- **HomeKit Integration**: Control LED on/off and brightness (5-100%) via Home app
- **Physical Button**: Manual control with different press types:
  - Single press: Toggle LED on/off
  - Double press: Turn on at 75% brightness
  - Long press: Cycle through brightness levels (5%, 25%, 50%, 75%, 100%)

### Temperature & Humidity Sensors
- **DHT11 Sensor**: Provides both temperature and humidity readings
- **HomeKit Integration**: Separate accessories for temperature and humidity
- **Automatic Updates**: Sensor readings updated every 2 seconds
- **Error Handling**: Graceful handling of sensor read failures

## Hardware Requirements

- ESP32 development board
- DHT11 temperature/humidity sensor
- LED (with appropriate current-limiting resistor)
- Push button (normally open)
- 10kΩ pull-up resistor for DHT11
- Breadboard and jumper wires

## ESP32 Board Configuration

**Choose the most appropriate board for your ESP32 module.** However, if you encounter memory issues or cannot set partition scheme with your current board selection, use these settings:

**ESP32 Dev Module Configuration (for memory/partition issues):**
- **Board**: ESP32 Dev Module
- **Port**: /dev/cu.usbserial-0001 (macOS with USB adapter)
- **Partition Scheme**: Huge APP (3MB No OTA (1MB APP/3MB SPIFFS))
- **Upload Speed**: 115200
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Frequency**: 80MHz
- **Flash Mode**: QIO
- **Flash Size**: 4MB (32Mb)
- **Arduino Runs On**: Core 1
- **Events Run On**: Core 1

**Note**: The HomeSpan library requires significant memory space. If you get compilation errors about insufficient memory or cannot access partition scheme options with your current board selection, switching to "ESP32 Dev Module" will provide access to the Huge APP partition scheme needed for HomeSpan projects.

## Wiring Diagram

TBD

## Installation

1. **Install Required Libraries**:
   - **HomeSpan** (via Arduino Library Manager) - **REQUIRED**
   - **DHT sensor library by Adafruit** (via Arduino Library Manager) - **REQUIRED**

2. **Upload the Sketch**:
   - Open `esp32-orange-lamp-retrofit.ino` in Arduino IDE
   - Select your ESP32 board (choose the most appropriate for your module)
   - If you encounter memory issues, switch to **ESP32 Dev Module** with **Huge APP (3MB No OTA)** partition scheme
   - Upload the sketch

3. **Configure HomeSpan**:
   - Open Serial Monitor (115200 baud)
   - Use HomeSpan CLI to configure WiFi credentials
   - Set up HomeKit pairing code

## HomeKit Setup

1. **Pair with HomeKit**:
   - Open Home app on iPhone/iPad
   - Add Accessory
   - Scan QR code or enter setup code
   - Follow pairing instructions

2. **Accessories Created**:
   - **Smart Orange LED**: Control on/off and brightness
   - **Orange Temperature**: View current temperature
   - **Orange Humidity**: View current humidity

## Button Functions

| Press Type | Function |
|------------|----------|
| Single Press | Toggle LED on/off |
| Double Press | Turn on at 75% brightness |
| Long Press | Cycle brightness: 5% → 25% → 50% → 75% → 100% → 5% |

## Troubleshooting

### LED Issues
- Check wiring to pin 21
- Ensure proper current-limiting resistor
- Verify LED polarity

### Button Issues
- Check wiring to pin 18
- Ensure button connects to GND when pressed
- Test with Serial Monitor for button press detection

### DHT11 Issues
- **Library Error**: Ensure "DHT sensor library by Adafruit" is installed
- **Compilation Error**: Install "Adafruit Unified Sensor" library (dependency)
- Check wiring to pin 19
- Ensure 10kΩ pull-up resistor is connected
- Verify power connections (3.3V and GND)
- Check sensor orientation and pinout
- DHT11 requires 2-second minimum between readings

### HomeKit Issues
- Ensure ESP32 is connected to WiFi
- Check HomeKit pairing code
- Verify device appears in Home app
- Check Serial Monitor for HomeSpan diagnostics

## Customization

### Change Pin Assignments
Edit the pin numbers in `setup()` function:
```cpp
new DEV_DimmableLED_WithButton(21, 18);  // LED pin, Button pin
new DEV_DHT11_Temperature(19);           // DHT11 pin
new DEV_DHT11_Humidity(19);              // DHT11 pin (same sensor)
```

### Modify Button Behavior
Edit the `button()` method in `DEV_LED.h` to change button functions.

### Adjust Sensor Reading Interval
Change `DHT_READ_INTERVAL` in `DEV_DHT11.h` (minimum 2000ms for DHT11).

## References

- [HomeSpan Documentation](https://github.com/HomeSpan/HomeSpan)
- [DHT11 Tutorial](https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/)
- [HomeKit Accessory Protocol](https://developer.apple.com/homekit/)
