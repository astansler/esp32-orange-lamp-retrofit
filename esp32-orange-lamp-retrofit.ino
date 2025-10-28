////////////////////////////////////////////////////////////
//                                                        //
// ESP32-C3 Orange Lamp Retrofit - HomeKit Control        //
// Features:                                              //
// - Tunable White LED with PWM control (20kHz)          //
// - Warm White LED on GPIO 21, Cool White on GPIO 20    //
// - Physical button on GPIO 9 for power/brightness      //
// - Physical button on GPIO 10 for color temp control   //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h"
#include "DEV_LED.h"

#define BUTTON_PIN_BRIGHTNESS 4
#define BUTTON_PIN_COLOR 5
#define LED_PIN_WARM 6
#define LED_PIN_COOL 7

void setup() {

  Serial.begin(115200);

  pinMode(BUTTON_PIN_BRIGHTNESS, INPUT_PULLUP);
  pinMode(BUTTON_PIN_COLOR, INPUT_PULLUP);

  // Initialize HomeSpan with Lighting category
  homeSpan.begin(Category::Lighting, "Smart Orange Lamp");

  // Create one accessory for the Tunable White Light
  new SpanAccessory();

    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Orange Lamp");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("ORANGE-001");
      new Characteristic::Model("Orange Lamp 1.0");
      new Characteristic::FirmwareRevision("2.0");

    // Create a Tunable White LED with dual button control
    new DEV_TunableWhiteLED_WithButtons(LED_PIN_WARM, LED_PIN_COOL, BUTTON_PIN_BRIGHTNESS, BUTTON_PIN_COLOR);

} // end of setup()

void loop(){

  homeSpan.poll();

} // end of loop()
