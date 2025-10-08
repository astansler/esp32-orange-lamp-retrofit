//////////////////////////////////////////////////////
//   DEVICE-SPECIFIC TUNABLE WHITE LED SERVICES     //
//////////////////////////////////////////////////////

struct DEV_TunableWhiteLED_WithButton : Service::LightBulb {

  // Characteristics for this service
  SpanCharacteristic *power;
  SpanCharacteristic *level;
  SpanCharacteristic *temp;

  // Hardware Pins
  LedPin *warmPin;
  LedPin *coolPin;
  SpanButton *pushButton;

  DEV_TunableWhiteLED_WithButton(int warmLEDPin, int coolLEDPin, int buttonPin) : Service::LightBulb(){

    // Initialize the Characteristics
    power = new Characteristic::On();
    level = new Characteristic::Brightness(100);
    level->setRange(5, 100, 1);

    // Color Temperature is measured in Mireds. 140 is cool (~7000K), 500 is warm (~2000K).
    temp = new Characteristic::ColorTemperature(320); // Default to a neutral white
    temp->setRange(140, 500, 1);

    // Configure the PWM LED pins
    this->warmPin = new LedPin(warmLEDPin);
    this->coolPin = new LedPin(coolLEDPin);

    // Configure button
    pinMode(buttonPin, INPUT_PULLUP);
    this->pushButton = new SpanButton(buttonPin);

  } // end constructor

  boolean update() override {

    boolean p = power->getNewVal();
    int brightness = level->getNewVal();
    int mired = temp->getNewVal();

    // If power is off, turn both LEDs off
    if (!p) {
      warmPin->set(0);
      coolPin->set(0);
      return(true);
    }

    // Convert Mireds color temp to a 0-1 ratio for warm/cool mixing
    // 0.0 = fully cool, 1.0 = fully warm
    float warmRatio = (float)(mired - 140) / (500 - 140);

    // Calculate the brightness for each channel
    int warmBrightness = brightness * warmRatio;
    int coolBrightness = brightness * (1 - warmRatio);

    // Set the PWM values
    warmPin->set(warmBrightness);
    coolPin->set(coolBrightness);

    return(true);

  } // update

  void button(int buttonPin, int pressType) override {

    if(pressType == SpanButton::SINGLE) {
      power->setVal(!power->getVal());
    }
    else if(pressType == SpanButton::DOUBLE) {
      power->setVal(true);
      level->setVal(100);
      temp->setVal(320); // Set to neutral white at 100%
    }
    else if(pressType == SpanButton::LONG) {
      power->setVal(true);
      int currentMired = temp->getVal();

      if(currentMired > 400)      // If it's warm...
        temp->setVal(140);        // ...make it cool.
      else if(currentMired < 240) // If it's cool...
        temp->setVal(500);        // ...make it warm.
      else                        // If it's neutral...
        temp->setVal(500);        // ...make it warm.
    }

    // After a button press, the update() method is called automatically.

  } // button

};
