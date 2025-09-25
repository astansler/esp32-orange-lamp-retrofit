/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/

////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Smart Orange - Dimmable LED with DHT11 Sensor         //
// Features:                                              //
// - Dimmable LED on pin 21 controlled via HomeKit       //
// - Physical button on pin 18 for manual control        //
// - DHT11 sensor for temperature and humidity readings  //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h"
#include "DEV_LED.h"
#include "DEV_DHT11.h"

void setup() {

  Serial.begin(115200);

  // Initialize HomeSpan with Lighting category
  homeSpan.begin(Category::Lighting, "Smart Orange");

  // Create first accessory - Dimmable LED with button control
  new SpanAccessory(); 
  
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();
      new Characteristic::Name("Smart Orange LED");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("ORANGE-001");
      new Characteristic::Model("Smart Orange LED with Button");
      new Characteristic::FirmwareRevision("1.0");

    // Create dimmable LED with button control on pin 18
    // LED pin: 21, Button pin: 18
    new DEV_DimmableLED_WithButton(21, 18);

  // Create second accessory - Temperature Sensor
  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Identify();
      new Characteristic::Name("Orange Temperature");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("ORANGE-TEMP-001");
      new Characteristic::Model("Smart Orange Temperature Sensor");
      new Characteristic::FirmwareRevision("1.0");
  
    // Create temperature sensor (DHT11 pin: 19)
    new DEV_DHT11_Temperature(19);

  // Create third accessory - Humidity Sensor
  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Identify();
      new Characteristic::Name("Orange Humidity");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("ORANGE-HUM-001");
      new Characteristic::Model("Smart Orange Humidity Sensor");
      new Characteristic::FirmwareRevision("1.0");
  
    // Create humidity sensor (DHT11 pin: 19 - same sensor)
    new DEV_DHT11_Humidity(19);

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
