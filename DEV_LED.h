//////////////////////////////////////////////////////
//   DEVICE-SPECIFIC TUNABLE WHITE LED SERVICES     //
//////////////////////////////////////////////////////

struct DEV_TunableWhiteLED_WithButtons : Service::LightBulb {

  // HomeKit Characteristics
  SpanCharacteristic *power;
  SpanCharacteristic *bright;
  SpanCharacteristic *temp;

  // Hardware Pins
  LedPin *warmLED;
  LedPin *coolLED;
  SpanButton *button1;  // Control button
  SpanButton *button2;  // Color button

  // State variables for ramping behavior
  boolean dimmingUp = true;     // Direction for brightness ramping
  boolean warmingUp = false;    // Direction for color temp ramping
  int colorPreset = 0;          // Track current color preset (0=warm, 1=mid, 2=cool)

  // Constructor
  DEV_TunableWhiteLED_WithButtons(int warmPin, int coolPin, int button1Pin, int button2Pin) : Service::LightBulb(){

    // Initialize HomeKit Characteristics
    power = new Characteristic::On();
    bright = new Characteristic::Brightness(100);
    bright->setRange(5, 100, 1);

    // Color Temperature is measured in Mireds
    // 140 mireds = cool (~7000K), 500 mireds = warm (~2000K)
    temp = new Characteristic::ColorTemperature(320); // Default to neutral
    temp->setRange(140, 500, 1);

    // Configure PWM LED pins with 20kHz frequency to prevent coil whine
    this->warmLED = new LedPin(warmPin, 0, 20000);
    this->coolLED = new LedPin(coolPin, 0, 20000);

    // Configure buttons
    // SpanButton(pin, longTime, singleTime, doubleTime, triggerType)
    // longTime = 2000ms (hold for 2 seconds to trigger LONG press)
    // singleTime = 5ms (minimum press time)
    // doubleTime = 200ms (max time between presses for DOUBLE)
    // triggerType = TRIGGER_ON_LOW (button connects pin to GND)
    this->button1 = new SpanButton(button1Pin, 2000, 5, 200, SpanButton::TRIGGER_ON_LOW);
    this->button2 = new SpanButton(button2Pin, 2000, 5, 200, SpanButton::TRIGGER_ON_LOW);

    Serial.printf("\n=== Tunable White LED Configured ===\n");
    Serial.printf("  Warm LED: GPIO %d (20kHz PWM)\n", warmPin);
    Serial.printf("  Cool LED: GPIO %d (20kHz PWM)\n", coolPin);
    Serial.printf("  Button 1: GPIO %d (Control)\n", button1Pin);
    Serial.printf("  Button 2: GPIO %d (Color)\n", button2Pin);
    Serial.printf("=====================================\n\n");

  } // end constructor

  // Update method: called when HomeKit characteristics change
  boolean update() override {

    boolean p = power->getNewVal();
    float brightness = bright->getNewVal() / 100.0;  // Convert to 0.0-1.0
    int mired = temp->getNewVal();

    // If power is off, turn both LEDs off
    if (!p) {
      warmLED->set(0);
      coolLED->set(0);
      Serial.println("LED: OFF");
      return(true);
    }

    // Convert Mireds color temp to a linear 0-1 mix value
    // 0.0 = fully cool (140 mireds), 1.0 = fully warm (500 mireds)
    float mix = (float)(mired - 140) / (500 - 140);

    // Apply non-linear curve (sqrt) for smoother visual transitions
    float warmLevel = sqrt(mix);
    float coolLevel = sqrt(1.0 - mix);

    // Apply master brightness and set final PWM values (0-100)
    int warmValue = (int)(warmLevel * brightness * 100.0);
    int coolValue = (int)(coolLevel * brightness * 100.0);

    warmLED->set(warmValue);
    coolLED->set(coolValue);

    Serial.printf("LED: Bright=%d%%, Temp=%dK, Warm=%d, Cool=%d\n", 
                  (int)(brightness*100), mired, warmValue, coolValue);

    return(true);

  } // end update

  // Button handler: called when buttons are pressed
  void button(int pin, int pressType) override {

    Serial.printf("\n>>> BUTTON: Pin=%d, Type=%d <<<\n", pin, pressType);

    // Button 1 (Control): Power and Brightness
    if(pin == button1->getPin()) {
      
      // Single press: Toggle power
      if(pressType == SpanButton::SINGLE) {
        boolean newPower = !power->getVal();
        Serial.printf("Button 1 SINGLE: Toggle power -> %s\n", newPower ? "ON" : "OFF");
        power->setVal(newPower);
      }
      
      // Long press: Ramp brightness up or down
      // Note: LONG press repeats every 2 seconds while held
      else if(pressType == SpanButton::LONG) {
        Serial.println("Button 1 LONG: Ramp brightness");
        power->setVal(true);  // Turn on if off
        
        int currentBright = bright->getVal();
        int newBright;

        if(dimmingUp) {
          // Ramp up by 10%
          newBright = currentBright + 10;
          if(newBright >= 100) {
            newBright = 100;
            dimmingUp = false;  // Toggle direction at maximum
            Serial.println("  -> MAX brightness, reversing direction");
          }
        } else {
          // Ramp down by 10%
          newBright = currentBright - 10;
          if(newBright <= 5) {
            newBright = 5;
            dimmingUp = true;  // Toggle direction at minimum
            Serial.println("  -> MIN brightness, reversing direction");
          }
        }

        Serial.printf("  -> Brightness: %d%% -> %d%%\n", currentBright, newBright);
        bright->setVal(newBright);
      }
    }

    // Button 2 (Color): Color Temperature presets and ramping
    else if(pin == button2->getPin()) {
      
      // Single press: Cycle through 3 color presets
      if(pressType == SpanButton::SINGLE) {
        Serial.println("Button 2 SINGLE: Cycle color preset");
        power->setVal(true);  // Turn on if off

        colorPreset = (colorPreset + 1) % 3;

        int newTemp;
        const char* presetName;
        switch(colorPreset) {
          case 0:  // Full Warm
            newTemp = 500;
            presetName = "WARM";
            break;
          case 1:  // 50/50 Mix (neutral)
            newTemp = 320;
            presetName = "NEUTRAL";
            break;
          case 2:  // Full Cool
            newTemp = 140;
            presetName = "COOL";
            break;
          default:
            newTemp = 320;
            presetName = "DEFAULT";
        }
        Serial.printf("  -> Preset: %s (%d mireds)\n", presetName, newTemp);
        temp->setVal(newTemp);
      }
      
      // Long press: Ramp color temp towards warm or cool
      // Note: LONG press repeats every 2 seconds while held
      else if(pressType == SpanButton::LONG) {
        Serial.println("Button 2 LONG: Ramp color temp");
        power->setVal(true);  // Turn on if off
        
        int currentTemp = temp->getVal();
        int newTemp;

        if(warmingUp) {
          // Ramp towards warm (increase mireds)
          newTemp = currentTemp + 30;
          if(newTemp >= 500) {
            newTemp = 500;
            warmingUp = false;  // Toggle direction at warm limit
            Serial.println("  -> WARM limit, reversing direction");
          }
        } else {
          // Ramp towards cool (decrease mireds)
          newTemp = currentTemp - 30;
          if(newTemp <= 140) {
            newTemp = 140;
            warmingUp = true;  // Toggle direction at cool limit
            Serial.println("  -> COOL limit, reversing direction");
          }
        }

        Serial.printf("  -> Color Temp: %d -> %d mireds\n", currentTemp, newTemp);
        temp->setVal(newTemp);
      }
    }

  } // end button

};
