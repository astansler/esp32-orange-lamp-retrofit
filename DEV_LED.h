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
  SpanButton *button1;  // Control button (GPIO 9)
  SpanButton *button2;  // Color button (GPIO 10)

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

    // Configure buttons with 500ms long-press threshold
    // Internal pull-ups are enabled by default by SpanButton
    this->button1 = new SpanButton(button1Pin, 500);
    this->button2 = new SpanButton(button2Pin, 500);

    LOG1("Configured Tunable White LED:\n");
    LOG1("  Warm LED Pin: %d (20kHz PWM)\n", warmPin);
    LOG1("  Cool LED Pin: %d (20kHz PWM)\n", coolPin);
    LOG1("  Button 1 (Control): GPIO %d\n", button1Pin);
    LOG1("  Button 2 (Color): GPIO %d\n", button2Pin);

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

    LOG2("Update: Power=%d, Brightness=%.0f%%, Temp=%d mireds\n", p, brightness*100, mired);
    LOG2("  Mix=%.2f, WarmLevel=%.2f (%d), CoolLevel=%.2f (%d)\n", 
         mix, warmLevel, warmValue, coolLevel, coolValue);

    return(true);

  } // end update

  // Button handler: called when buttons are pressed
  void button(int pin, int pressType) override {

    LOG1("Button press: Pin=%d, Type=%d\n", pin, pressType);

    // Button 1 (Control): Power and Brightness
    if(pin == button1->getPin()) {
      
      // Single press: Toggle power
      if(pressType == SpanButton::SINGLE) {
        LOG1("Button 1 Single: Toggle power\n");
        power->setVal(!power->getVal());
      }
      
      // Long press: Ramp brightness up or down
      else if(pressType == SpanButton::LONG) {
        LOG1("Button 1 Long: Ramp brightness\n");
        power->setVal(true);  // Turn on if off
        
        int currentBright = bright->getVal();
        int newBright;

        if(dimmingUp) {
          // Ramp up by 10%
          newBright = currentBright + 10;
          if(newBright >= 100) {
            newBright = 100;
            dimmingUp = false;  // Toggle direction at maximum
            LOG1("  Reached max brightness, next press will dim down\n");
          }
        } else {
          // Ramp down by 10%
          newBright = currentBright - 10;
          if(newBright <= 5) {
            newBright = 5;
            dimmingUp = true;  // Toggle direction at minimum
            LOG1("  Reached min brightness, next press will dim up\n");
          }
        }

        bright->setVal(newBright);
        LOG1("  Brightness: %d -> %d\n", currentBright, newBright);
      }
    }

    // Button 2 (Color): Color Temperature presets and ramping
    else if(pin == button2->getPin()) {
      
      // Single press: Cycle through 3 color presets
      if(pressType == SpanButton::SINGLE) {
        LOG1("Button 2 Single: Cycle color preset\n");
        power->setVal(true);  // Turn on if off

        colorPreset = (colorPreset + 1) % 3;

        int newTemp;
        switch(colorPreset) {
          case 0:  // Full Warm
            newTemp = 500;
            LOG1("  Preset: Full Warm (500 mireds)\n");
            break;
          case 1:  // 50/50 Mix (neutral)
            newTemp = 320;
            LOG1("  Preset: Neutral Mix (320 mireds)\n");
            break;
          case 2:  // Full Cool
            newTemp = 140;
            LOG1("  Preset: Full Cool (140 mireds)\n");
            break;
        }
        temp->setVal(newTemp);
      }
      
      // Long press: Ramp color temp towards warm or cool
      else if(pressType == SpanButton::LONG) {
        LOG1("Button 2 Long: Ramp color temp\n");
        power->setVal(true);  // Turn on if off
        
        int currentTemp = temp->getVal();
        int newTemp;

        if(warmingUp) {
          // Ramp towards warm (increase mireds)
          newTemp = currentTemp + 30;
          if(newTemp >= 500) {
            newTemp = 500;
            warmingUp = false;  // Toggle direction at warm limit
            LOG1("  Reached warm limit, next press will cool\n");
          }
        } else {
          // Ramp towards cool (decrease mireds)
          newTemp = currentTemp - 30;
          if(newTemp <= 140) {
            newTemp = 140;
            warmingUp = true;  // Toggle direction at cool limit
            LOG1("  Reached cool limit, next press will warm\n");
          }
        }

        temp->setVal(newTemp);
        LOG1("  Color Temp: %d -> %d mireds\n", currentTemp, newTemp);
      }
    }

  } // end button

};
