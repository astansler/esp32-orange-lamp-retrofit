////////////////////////////////////
//   DEVICE-SPECIFIC DHT11 SERVICES //
////////////////////////////////////

#include "DHT.h"

// DHT11 sensor configuration
#define DHT_TYPE DHT11
#define DHT_READ_INTERVAL 2000  // Read sensor every 2 seconds (DHT11 minimum)

struct DEV_DHT11_Temperature : Service::TemperatureSensor {     // Temperature Sensor Service

  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  DHT *dht;                                         // DHT sensor object
  int dhtPin;                                       // DHT sensor pin
  unsigned long lastRead;                           // last sensor read time
  
  DEV_DHT11_Temperature(int pin) : Service::TemperatureSensor(){
    
    this->dhtPin = pin;
    this->dht = new DHT(pin, DHT_TYPE);
    this->lastRead = 0;
    
    // Initialize temperature characteristic with a default value
    temp = new Characteristic::CurrentTemperature(20.0);        // Initial temperature 20°C
    temp->setRange(-10, 50);                                   // DHT11 range: -10°C to 50°C
        
    // Initialize DHT sensor
    dht->begin();
    
    Serial.printf("DHT11 Temperature Sensor initialized on pin %d\n", pin);

  } // end constructor

  void loop() override {                              // loop() method for periodic sensor reading

    unsigned long currentTime = millis();
    
    // Check if it's time to read the sensor (every 2 seconds minimum)
    if(currentTime - lastRead >= DHT_READ_INTERVAL) {
      
      float temperature = dht->readTemperature();
      
      // Check if reading is valid (not NaN)
      if(!isnan(temperature)) {
        temp->setVal(temperature);
        LOG1("Temperature: %.1f°C\n", temperature);
      } else {
        LOG1("Failed to read temperature from DHT11\n");
      }
      
      lastRead = currentTime;
    }
    
  } // loop
  
}; // end DEV_DHT11_Temperature

//////////////////////////////////

struct DEV_DHT11_Humidity : Service::HumiditySensor {     // Humidity Sensor Service

  SpanCharacteristic *humidity;                    // reference to the Current Relative Humidity Characteristic
  DHT *dht;                                        // DHT sensor object (shared with temperature)
  int dhtPin;                                      // DHT sensor pin
  unsigned long lastRead;                          // last sensor read time
  
  DEV_DHT11_Humidity(int pin) : Service::HumiditySensor(){
    
    this->dhtPin = pin;
    this->dht = new DHT(pin, DHT_TYPE);
    this->lastRead = 0;
    
    // Initialize humidity characteristic with a default value
    humidity = new Characteristic::CurrentRelativeHumidity(50.0);  // Initial humidity 50%
    humidity->setRange(20, 90);                                   // DHT11 range: 20% to 90%
        
    // Initialize DHT sensor
    dht->begin();
    
    Serial.printf("DHT11 Humidity Sensor initialized on pin %d\n", pin);

  } // end constructor

  void loop() override {                              // loop() method for periodic sensor reading

    unsigned long currentTime = millis();
    
    // Check if it's time to read the sensor (every 2 seconds minimum)
    if(currentTime - lastRead >= DHT_READ_INTERVAL) {
      
      float humidityValue = dht->readHumidity();
      
      // Check if reading is valid (not NaN)
      if(!isnan(humidityValue)) {
        humidity->setVal(humidityValue);
        LOG1("Humidity: %.1f%%\n", humidityValue);
      } else {
        LOG1("Failed to read humidity from DHT11\n");
      }
      
      lastRead = currentTime;
    }
    
  } // loop
  
}; // end DEV_DHT11_Humidity

//////////////////////////////////
