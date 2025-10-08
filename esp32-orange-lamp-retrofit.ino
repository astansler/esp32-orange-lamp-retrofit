////////////////////////////////////////////////////////////
//                                                        //
// Smart Orange - Dimmable LED with DHT11 Sensor          //
// Features:                                              //
// - Dimmable LED on pin 21 controlled via HomeKit        //
// - Physical button on pin 18 for manual control         //
// - DHT11 sensor for temperature and humidity readings   //
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

    // Create a Tunable White LED with button control
    // Warm Pin: 22, Cool Pin: 23, Button pin: 18
    new DEV_TunableWhiteLED_WithButton(22, 23, 18);

} // end of setup()

void loop(){

  homeSpan.poll();

} // end of loop()
