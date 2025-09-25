# Configuration Guide

## Pin Configuration

The sketch uses the following default pin assignments:

| Component | Pin | Function |
|-----------|-----|----------|
| LED | 21 | PWM output for dimmable LED |
| Button | 18 | Digital input for manual control |
| DHT11 | 19 | Digital input for sensor data |

## Customizing Pin Assignments

To change pin assignments, modify the following lines in `smart_orange.ino`:

```cpp
// Change LED pin (currently 21) and button pin (currently 18)
new DEV_DimmableLED_WithButton(21, 18);

// Change DHT11 pin (currently 19)
new DEV_DHT11_Temperature(19);
new DEV_DHT11_Humidity(19);
```

## HomeSpan Configuration

### WiFi Setup
1. Upload the sketch to your ESP32
2. Open Serial Monitor (115200 baud)
3. Use HomeSpan CLI commands:
   ```
   W <your-wifi-ssid>
   P <your-wifi-password>
   ```

### HomeKit Setup Code
- Default setup code: `466-37-726`
- To change: Use HomeSpan CLI command `S <new-8-digit-code>`

## Button Configuration

### Default Button Behavior
- **Single Press**: Toggle LED on/off
- **Double Press**: Turn on at 75% brightness
- **Long Press**: Cycle through brightness levels

### Customizing Button Behavior
Edit the `button()` method in `DEV_LED.h`:

```cpp
void button(int buttonPin, int pressType) override {
  if(pressType == SpanButton::SINGLE) {
    // Your custom single press action
  }
  else if(pressType == SpanButton::DOUBLE) {
    // Your custom double press action
  }
  else if(pressType == SpanButton::LONG) {
    // Your custom long press action
  }
}
```

## Sensor Configuration

### DHT11 Reading Interval
Default: 2000ms (2 seconds)
To change, modify `DHT_READ_INTERVAL` in `DEV_DHT11.h`:

```cpp
#define DHT_READ_INTERVAL 3000  // Change to 3 seconds
```

### Temperature Range
Default: -10°C to 50°C (DHT11 specification)
To change, modify in `DEV_DHT11_Temperature` constructor:

```cpp
temp->setRange(-20, 60);  // Custom range
```

### Humidity Range
Default: 20% to 90% (DHT11 specification)
To change, modify in `DEV_DHT11_Humidity` constructor:

```cpp
humidity->setRange(10, 95);  // Custom range
```

## LED Configuration

### Brightness Range
Default: 5% to 100% in 1% steps
To change, modify in `DEV_DimmableLED_WithButton` constructor:

```cpp
level->setRange(1, 100, 5);  // 1% to 100% in 5% steps
```

### Initial Brightness
Default: 50%
To change, modify in `DEV_DimmableLED_WithButton` constructor:

```cpp
level = new Characteristic::Brightness(75);  // Start at 75%
```

## HomeKit Accessory Names

### Default Names
- "Smart Orange LED"
- "Orange Temperature"
- "Orange Humidity"

### Customizing Names
Modify the `Characteristic::Name()` lines in `setup()`:

```cpp
new Characteristic::Name("My Custom LED");
new Characteristic::Name("Room Temperature");
new Characteristic::Name("Room Humidity");
```

## Device Information

### Manufacturer and Model
Default values can be changed in `setup()`:

```cpp
new Characteristic::Manufacturer("Your Company");
new Characteristic::Model("Custom LED Controller");
new Characteristic::SerialNumber("LED-2024-001");
new Characteristic::FirmwareRevision("2.0");
```

## Advanced Configuration

### Button Debouncing
Default button timing (in milliseconds):
- Single press: 5ms minimum
- Double press: 200ms maximum between presses
- Long press: 2000ms minimum

To change, modify the SpanButton constructor in `DEV_LED.h`:

```cpp
// Custom timing: long=3000ms, single=10ms, double=300ms
this->button = new SpanButton(buttonPin, 3000, 10, 300);
```

### Logging Level
To enable detailed logging, add to `setup()`:

```cpp
homeSpan.setLogLevel(1);  // Enable detailed logging
```

### Status LED
To add a status LED, add to `setup()`:

```cpp
homeSpan.setStatusPin(2);  // Use built-in LED on pin 2
```

## Troubleshooting Configuration

### Common Issues
1. **Wrong pin assignments**: Double-check pin numbers in setup()
2. **Button not working**: Verify button connects to GND when pressed
3. **DHT11 errors**: Check pull-up resistor and power connections
4. **HomeKit pairing fails**: Verify WiFi credentials and setup code

### Debug Mode
Enable debug logging by adding to `setup()`:

```cpp
homeSpan.setLogLevel(2);  // Maximum logging
```

This will show detailed information about HomeSpan operations, button presses, and sensor readings.
