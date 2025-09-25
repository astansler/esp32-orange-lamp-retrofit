# Wiring Diagram

## ESP32 Pin Connections

```
ESP32 Development Board
┌─────────────────────────────────┐
│                                 │
│  Pin 18 ────────────────────────┼─── Push Button ──── GND
│                                 │
│  Pin 19 ────────────────────────┼─── DHT11 Data Pin
│                                 │    (with 10kΩ pull-up to 3.3V)
│  Pin 21 ────────────────────────┼─── LED Anode
│                                 │
│  3.3V ──────────────────────────┼─── DHT11 VCC
│                                 │
│  GND ───────────────────────────┼─── DHT11 GND, LED Cathode, Button
│                                 │
└─────────────────────────────────┘
```

## DHT11 Sensor Pinout

When facing the sensor (grid side up):
```
┌─────────────┐
│ 1 2 3 4     │
│ VCC Data NC GND
└─────────────┘
```

## Component Connections

### LED Circuit
- **LED Anode** → Pin 21 (ESP32)
- **LED Cathode** → 220Ω Resistor → GND
- **Note**: Use appropriate current-limiting resistor based on LED specifications

### Button Circuit
- **Button Terminal 1** → Pin 18 (ESP32)
- **Button Terminal 2** → GND
- **Note**: Button is normally open, connects to GND when pressed

### DHT11 Circuit
- **Pin 1 (VCC)** → 3.3V (ESP32)
- **Pin 2 (Data)** → Pin 19 (ESP32)
- **Pin 2 (Data)** → 10kΩ Resistor → 3.3V (pull-up)
- **Pin 4 (GND)** → GND (ESP32)
- **Pin 3 (NC)** → Not connected

## Power Supply
- **ESP32**: Powered via USB or external 5V supply
- **DHT11**: Powered from ESP32 3.3V rail
- **LED**: Powered from ESP32 pin 21 (3.3V logic)
- **Button**: No external power required (uses internal pull-up)

## Important Notes

1. **Pull-up Resistor**: DHT11 requires a 10kΩ pull-up resistor on the data line
2. **Power Supply**: Ensure stable 3.3V supply for DHT11 sensor
3. **Wiring Length**: Keep DHT11 data wire short (< 20cm) for reliable communication
4. **Current Limiting**: Always use a current-limiting resistor with LEDs
5. **Ground Connection**: Ensure all components share a common ground

## Troubleshooting Tips

- **DHT11 not working**: Check pull-up resistor and power connections
- **LED not dimming**: Verify PWM pin assignment and current-limiting resistor
- **Button not responding**: Check button wiring and ensure it connects to GND when pressed
- **HomeKit not pairing**: Verify WiFi connection and HomeSpan configuration
