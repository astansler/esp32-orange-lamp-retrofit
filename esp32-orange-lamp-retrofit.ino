////////////////////////////////////////////////////////////
//                                                        //
// Orange Lamp - Tunable White LED Retrofit               //
// Features:                                              //
// - Dual-channel PWM control (Warm + Cool White)         //
// - 20kHz PWM frequency (prevents coil whine)            //
// - Two physical buttons for control                     //
// - HomeKit integration via HomeSpan                     //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h"
#include "DEV_LED.h"

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting, "Orange Lamp");

  new SpanAccessory();

    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Orange Lamp");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("ORANGE-001");
      new Characteristic::Model("Orange Lamp 1.0");
      new Characteristic::FirmwareRevision("2.0");

    new DEV_OrangeLamp(21, 20, 9, 10);

}

void loop() {

  homeSpan.poll();

}
