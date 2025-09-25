# Smart Orange - HomeSpan Dimmable LED with DHT11 Sensor

This HomeSpan sketch provides a complete HomeKit integration for ESP32 with:
- Dimmable LED control (pin 21)
- Physical button control (pin 18)
- DHT11 temperature and humidity sensor (pin 19)

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

```
ESP32 Pin Connections:
┌─────────────────┐
│       ESP32     │
│                 │
│ Pin 18 ─────────┼─── Push Button ──── GND
│                 │
│ Pin 19 ─────────┼─── DHT11 Data Pin
│                 │    (with 10kΩ pull-up to 3.3V)
│ Pin 21 ─────────┼─── LED Anode
│                 │
│ 3.3V ───────────┼─── DHT11 VCC
│                 │
│ GND ────────────┼─── DHT11 GND, LED Cathode, Button
└─────────────────┘

DHT11 Pinout (when facing the sensor):
┌─────────────┐
│ 1 2 3 4     │
│ VCC Data NC GND
└─────────────┘
```

## Installation

1. **Install Required Libraries**:
   - **HomeSpan** (via Arduino Library Manager)
   - **DHT sensor library by Adafruit** (via Arduino Library Manager) - **REQUIRED**
   - **Adafruit Unified Sensor library** (dependency of DHT library) - **REQUIRED**
   
   **Important**: The DHT sensor library by Adafruit is essential for the temperature and humidity sensors to work. Without it, the sketch will not compile.

2. **Upload the Sketch**:
   - Open `smart_orange.ino` in Arduino IDE
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
