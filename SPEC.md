### Project Goal

To create a smart, high-power, color-temperature-adjustable lamp controlled by Apple HomeKit. The project uses an **ESP32-C3 SuperMini** microcontroller running the **HomeSpan** library.

The lamp has two separate LED assemblies (Warm White and Cool White) that are controlled via PWM to set both master brightness and a "tunable white" color temperature. The lamp is also controllable via two physical pushbuttons for on/off, brightness, and color temperature presets.

### 1. Hardware Components

* **Microcontroller:** 1x ESP32-C3 SuperMini
* **LEDs:** 1x Warm White LED assembly, 1x Cool White LED assembly.
    * *Total current draw is approximately 2A at 5V.*
* **Power Supply:** 1x 5V USB Charger (rated for at least 2A, e.g., 5V/3A).
* **MOSFETs:** 2x **IRLZ44N** (N-Channel Logic-Level MOSFETs).
* **Buttons:** 2x Physical pushbuttons.
* **Resistors:**
    * 2x **10kΩ** (Gate-to-Source pull-down resistors for stability).

### 2. Wiring Diagram & Best Practices

This design uses **low-side switching** with N-Channel MOSFETs to control the high-current LED load. The power path is split to protect the microcontroller.

#### Power Distribution
* **Common Ground:** A common `GND` is essential. The `GND` from the USB charger, the `GND` on the ESP32-C3, and the `Source` pins of both MOSFETs must all be connected.
* **5V Power:** The `5V` line from the USB charger is split:
    1.  **To ESP32:** Connects to the `5V` pin on the ESP32-C3 to power the microcontroller.
    2.  **To LEDs:** Connects directly to the `+` (Positive) terminal of **both** the Warm White and Cool White LED assemblies. *This is critical, as the 2A load must not be drawn through the ESP32.*


#### Control Wiring

**Warm LED Circuit (MOSFET 1):**
* **ESP32 `GPIO 21`** → **Gate (G)** pin of MOSFET 1.
* **Gate (G)** pin of MOSFET 1 → **10kΩ Resistor (R_PD)** → **Source (S)** pin of MOSFET 1 (which is connected to common `GND`).
* **Drain (D)** pin of MOSFET 1 → **`–` (Negative)** terminal of the Warm White LED assembly.
* **Source (S)** pin of MOSFET 1 → Common `GND`.

**Cool LED Circuit (MOSFET 2):**
* **ESP32 `GPIO 20`** → **Gate (G)** pin of MOSFET 2.
* **Gate (G)** pin of MOSFET 2 → **10kΩ Resistor (R_PD)** → **Source (S)** pin of MOSFET 2 (which is connected to common `GND`).
* **Drain (D)** pin of MOSFET 2 → **`–` (Negative)** terminal of the Cool White LED assembly.
* **Source (S)** pin of MOSFET 2 → Common `GND`.

**Button Wiring:**
* **Button 1 (Control):** Connects between **ESP32 `GPIO 9`** and common `GND`.
* **Button 2 (Color):** Connects between **ESP32 `GPIO 10`** and common `GND`.
* *Note: No external pull-up resistors are needed for the buttons. The code will use the ESP32's internal pull-ups (use pinMode(BUTTON_PIN, INPUT_PULLUP))*

---

### 3. Software & Logic (HomeSpan)

The code defines a single HomeKit accessory containing one custom LightBulb service.

#### Core Requirements
1.  **Library:** `HomeSpan.h`
2.  **PWM Frequency:** The PWM frequency must be set to **20,000 Hz (20kHz)**. This is an ultrasonic frequency that prevents the audible high-pitched "coil whine" that is common with lower-frequency PWM.
3.  **HomeKit Characteristics:** The custom service (Orange Lamp) must expose:
    * `Characteristic::On` (Power On/Off)
    * `~Brightness` (Master Brightness, 0-100%)
    * `~ColorTemperature` (Warm White - Cool White)

#### PWM & Logic

The core logic resides in the `update()` method of the custom service. This method translates the three HomeKit characteristics into two brightness values for the `warmLED` and `coolLED` pins.

1.  **Instantiate `LedPin` Objects:** Two `LedPin` objects are created in the constructor, passing the 20kHz frequency directly:
    * `warmLED = new LedPin(WARM_PIN, 0, 20000);`
    * `coolLED = new LedPin(COOL_PIN, 0, 20000);`
2.  **Get Values:** Get the new values for `On`, `Brightness`, and `ColorTemperature`.
3.  **Handle "Off" State:** If `On` is `false`, set both `warmLED->set(0)` and `coolLED->set(0)`.
4.  **Calculate Mix:** If `On` is `true`:
    * Convert the master `Brightness` (0-100) to a float `brightness` (0.0-1.0).
    * Map the `ColorTemperature` value to a linear "mix" float (0.0-1.0), where 0.0 is full cool and 1.0 is full warm.
    * Use a non-linear curve (e.g., `sqrt()`) for a smoother visual transition:
        * `float warmLevel = sqrt(mix);`
        * `float coolLevel = sqrt(1.0 - mix);`
    * Apply the master brightness and set the final 0-100 values for each `LedPin`:
        * `warmLED->set(warmLevel * brightness * 100.0);`
        * `coolLED->set(coolLevel * brightness * 100.0);`

#### Button Behavior

Two `SpanButton` objects are instantiated to handle button presses. The internal pull-up is enabled by default.

* `new SpanButton(BUTTON1_PIN, 500);` (500ms for long-press)
* `new SpanButton(BUTTON2_PIN, 500);`

**Button 1 (`GPIO 9` - Control):**
* **Single Press:** Toggles the `power` Characteristic.
* **Long Press (Hold):** Ramps the `bright` Characteristic up. If it hits 100, a `dimmingUp` boolean flips, and the next long press will ramp down.

**Button 2 (`GPIO 10` - Color):**
* **Single Press:** Cycles the `temp` Characteristic between three presets: Full Warm, 50/50 Mix, and Full Cool.
* **Long Press (Hold):** Ramps the `temp` Characteristic towards warm. If it hits certain value, a `warmingUp` boolean flips, and the next long press will ramp towards cool.

#### Project Structure

Main file should be named `esp32-orange-lamp-retrofit.ino`.

#### URLs
- HomeSpan
    - Repo https://github.com/HomeSpan/HomeSpan/
    - Example https://github.com/HomeSpan/HomeSpan/tree/master/examples
    - Docs https://github.com/HomeSpan/HomeSpan/tree/master/docs
