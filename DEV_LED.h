////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_DimmableLED_WithButton : Service::LightBulb {       // Dimmable LED with Button Control

  LedPin *ledPin;                                   // PWM LED pin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  SpanButton *pushButton;                           // button for manual control
  
  DEV_DimmableLED_WithButton(int ledPin, int buttonPin) : Service::LightBulb(){

    power = new Characteristic::On();     
    level = new Characteristic::Brightness(50);       // Initial brightness 50%
    level->setRange(5, 100, 1);                       // Range: 5% to 100%, steps of 1%

    this->ledPin = new LedPin(ledPin);                // Configure PWM LED
    
    // Configure button with explicit settings
    pinMode(buttonPin, INPUT_PULLUP);                 // Enable internal pull-up
    this->pushButton = new SpanButton(buttonPin, 2000, 50, 200, SpanButton::TRIGGER_ON_LOW);
    Serial.printf("Button configured on pin %d\n", buttonPin);
    
  } // end constructor

  boolean update(){                              // update() method

    // Set LED brightness based on power state and brightness level
    ledPin->set(power->getNewVal() * level->getNewVal());    
   
    return(true);                               // return true
  
  } // update

  void button(int buttonPin, int pressType) override {  // button() method for manual control

    Serial.printf("HomeSpan Button Event: pin=%d, type=%d\n", buttonPin, pressType);

    if(pressType == SpanButton::SINGLE) {               // Single press: toggle power
      bool newPowerState = !power->getVal();
      power->setVal(newPowerState);
      Serial.printf("Button single press: LED %s\n", newPowerState ? "ON" : "OFF");
      
      // Manually update LED hardware
      ledPin->set(newPowerState * level->getVal());
      Serial.printf("LED hardware updated: power=%s, brightness=%d%%, actual=%d%%\n", 
                    newPowerState ? "ON" : "OFF", level->getVal(), newPowerState * level->getVal());
    }
    else if(pressType == SpanButton::DOUBLE) {          // Double press: set to favorite brightness (75%)
      power->setVal(true);
      level->setVal(75);
      Serial.println("Button double press: LED ON at 75%");
      
      // Manually update LED hardware
      ledPin->set(75);
      Serial.println("LED hardware updated: 75%");
    }
    else if(pressType == SpanButton::LONG) {            // Long press: cycle through brightness levels
      int currentLevel = level->getVal();
      int newLevel;
      
      if(currentLevel < 25) newLevel = 25;
      else if(currentLevel < 50) newLevel = 50;
      else if(currentLevel < 75) newLevel = 75;
      else if(currentLevel < 100) newLevel = 100;
      else newLevel = 5;
      
      power->setVal(true);
      level->setVal(newLevel);
      Serial.printf("Button long press: LED ON at %d%%\n", newLevel);
      
      // Manually update LED hardware
      ledPin->set(newLevel);
      Serial.printf("LED hardware updated: %d%%\n", newLevel);
    }
    
  } // button
  
}; // end DEV_DimmableLED_WithButton
      
//////////////////////////////////
