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

void setup() {

  Serial.begin(115200);

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
    // Warm Pin: GPIO 21, Cool Pin: GPIO 20
    // Button 1 (Control): GPIO 9, Button 2 (Color): GPIO 10
    new DEV_TunableWhiteLED_WithButtons(21, 20, 9, 10);

} // end of setup()

void loop(){

  homeSpan.poll();

} // end of loop()
