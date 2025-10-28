//////////////////////////////////////////////////////
//   ORANGE LAMP - TUNABLE WHITE LED SERVICE        //
//////////////////////////////////////////////////////

struct DEV_OrangeLamp : Service::LightBulb {

  SpanCharacteristic *power;
  SpanCharacteristic *bright;
  SpanCharacteristic *temp;

  LedPin *warmLED;
  LedPin *coolLED;

  SpanButton *controlButton;
  SpanButton *colorButton;

  boolean dimmingUp = true;
  boolean warmingUp = true;

  DEV_OrangeLamp(int warmPin, int coolPin, int button1Pin, int button2Pin) : Service::LightBulb() {

    power = new Characteristic::On();
    bright = new Characteristic::Brightness(100);
    bright->setRange(5, 100, 1);

    temp = new Characteristic::ColorTemperature(320);
    temp->setRange(140, 500, 1);

    warmLED = new LedPin(warmPin, 0, 20000);
    coolLED = new LedPin(coolPin, 0, 20000);

    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);

    controlButton = new SpanButton(button1Pin, 500);
    colorButton = new SpanButton(button2Pin, 500);

  }

  boolean update() override {

    boolean p = power->getNewVal();
    int brightness = bright->getNewVal();
    int mired = temp->getNewVal();

    if (!p) {
      warmLED->set(0);
      coolLED->set(0);
      return true;
    }

    float brightnessFloat = brightness / 100.0;
    float mix = (mired - 140.0) / (500.0 - 140.0);

    float warmLevel = sqrt(mix);
    float coolLevel = sqrt(1.0 - mix);

    warmLED->set(warmLevel * brightnessFloat * 100.0);
    coolLED->set(coolLevel * brightnessFloat * 100.0);

    return true;
  }

  void button(int pin, int pressType) override {

    if (pin == controlButton->getPin()) {

      if (pressType == SpanButton::SINGLE) {
        power->setVal(!power->getVal());
      }
      else if (pressType == SpanButton::LONG) {
        power->setVal(true);
        int currentBright = bright->getVal();

        if (dimmingUp) {
          currentBright += 10;
          if (currentBright >= 100) {
            currentBright = 100;
            dimmingUp = false;
          }
        } else {
          currentBright -= 10;
          if (currentBright <= 5) {
            currentBright = 5;
            dimmingUp = true;
          }
        }
        bright->setVal(currentBright);
      }

    } else if (pin == colorButton->getPin()) {

      if (pressType == SpanButton::SINGLE) {
        int currentTemp = temp->getVal();

        if (currentTemp <= 160) {
          temp->setVal(320);
        } else if (currentTemp <= 400) {
          temp->setVal(500);
        } else {
          temp->setVal(140);
        }
      }
      else if (pressType == SpanButton::LONG) {
        power->setVal(true);
        int currentTemp = temp->getVal();

        if (warmingUp) {
          currentTemp += 20;
          if (currentTemp >= 500) {
            currentTemp = 500;
            warmingUp = false;
          }
        } else {
          currentTemp -= 20;
          if (currentTemp <= 140) {
            currentTemp = 140;
            warmingUp = true;
          }
        }
        temp->setVal(currentTemp);
      }

    }

  }

};
