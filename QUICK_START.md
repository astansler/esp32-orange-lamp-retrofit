# Smart Orange - Quick Start Guide

## What You Get

This HomeSpan sketch creates a complete HomeKit device with:

1. **Dimmable LED** (Pin 21) - Control via Home app or physical button
2. **Physical Button** (Pin 18) - Manual control with smart functions
3. **Temperature Sensor** (DHT11 on Pin 19) - Real-time temperature readings
4. **Humidity Sensor** (DHT11 on Pin 19) - Real-time humidity readings

## Hardware Setup (5 minutes)

### Required Components
- ESP32 development board
- DHT11 temperature/humidity sensor
- LED + 220Ω resistor
- Push button
- 10kΩ resistor (for DHT11 pull-up)
- Breadboard and jumper wires

### Quick Wiring
```
ESP32 Pin 21 → LED Anode → LED Cathode → 220Ω Resistor → GND
ESP32 Pin 18 → Push Button → GND
ESP32 Pin 19 → DHT11 Data Pin
ESP32 3.3V → DHT11 VCC
ESP32 GND → DHT11 GND, LED Cathode, Button
10kΩ Resistor → DHT11 Data Pin → 3.3V (pull-up)
```

## Software Setup (5 minutes)

### 1. Install Libraries
In Arduino IDE Library Manager, install:
- **HomeSpan** (by Gregg E. Berman)
- **DHT sensor library** (by Adafruit) - **REQUIRED**
- **Adafruit Unified Sensor** (dependency) - **REQUIRED**

**⚠️ Important**: The DHT sensor library by Adafruit is essential for temperature and humidity sensors to work!

### 2. Upload Sketch
- Open `smart_orange.ino`
- Select your ESP32 board (choose the most appropriate for your module)
- If you get memory errors, switch to **ESP32 Dev Module** with **Huge APP (3MB No OTA)** partition scheme
- Upload the sketch

### 3. Configure WiFi
- Open Serial Monitor (115200 baud)
- Type: `W YourWiFiName`
- Type: `P YourWiFiPassword`

## HomeKit Setup (2 minutes)

1. Open **Home** app on iPhone/iPad
2. Tap **+** → **Add Accessory**
3. Scan QR code or enter setup code: `466-37-726`
4. Follow pairing instructions
5. Name your accessories

## Button Functions

| Action | Function |
|--------|----------|
| **Single Press** | Toggle LED on/off |
| **Double Press** | Turn on at 75% brightness |
| **Long Press** | Cycle brightness: 5% → 25% → 50% → 75% → 100% |

## What You'll See in Home App

- **Smart Orange LED**: On/off switch + brightness slider
- **Orange Temperature**: Current temperature display
- **Orange Humidity**: Current humidity percentage

## Troubleshooting

### LED Not Working?
- Check wiring to pin 21
- Verify current-limiting resistor
- Check LED polarity

### Button Not Responding?
- Check wiring to pin 18
- Ensure button connects to GND when pressed
- Check Serial Monitor for button press messages

### DHT11 Not Reading?
- Check 10kΩ pull-up resistor
- Verify power connections (3.3V, GND)
- Check data wire to pin 19
- DHT11 needs 2+ seconds between readings

### HomeKit Not Pairing?
- Verify WiFi connection
- Check setup code: `466-37-726`
- Ensure ESP32 is in READY-TO-PAIR state (LED blinks twice every 3 seconds)

## Customization

### Change Pins
Edit in `setup()`:
```cpp
new DEV_DimmableLED_WithButton(21, 18);  // LED pin, Button pin
new DEV_DHT11_Temperature(19);           // DHT11 pin
```

### Change Button Behavior
Edit `button()` method in `DEV_LED.h`

### Change Sensor Reading Speed
Edit `DHT_READ_INTERVAL` in `DEV_DHT11.h`

## Files Included

- `smart_orange.ino` - Main sketch
- `DEV_LED.h` - LED and button control
- `DEV_DHT11.h` - Temperature/humidity sensor
- `README.md` - Detailed documentation
- `WIRING_DIAGRAM.md` - Complete wiring guide
- `CONFIGURATION.md` - Customization options
- `QUICK_START.md` - This quick start guide

## Need Help?

1. Check Serial Monitor for error messages
2. Verify all connections match wiring diagram
3. Ensure all required libraries are installed
4. Check HomeSpan documentation: https://github.com/HomeSpan/HomeSpan

## Success!

Once working, you'll have:
- ✅ HomeKit-controlled dimmable LED
- ✅ Physical button with smart functions  
- ✅ Real-time temperature monitoring
- ✅ Real-time humidity monitoring
- ✅ Professional HomeKit integration

Enjoy your new smart home device! 🏠✨
