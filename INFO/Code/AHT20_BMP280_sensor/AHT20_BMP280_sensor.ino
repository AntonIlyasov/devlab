// Подключение к I2C ESP32: GPIO21=SDA; GPIO22=SCL

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>  //https://github.com/adafruit/Adafruit_BMP280_Library
#include "DFRobot_AHT20.h"  //https://github.com/DFRobot/DFRobot_AHT20

int call_time = 0;

DFRobot_AHT20 aht20;

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
  // start serial port  
	Serial.begin(115200); 
	Serial.setTimeout(1); 

  bmp.begin();
  aht20.begin();

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */
}

void loop() {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("4");   
  }
  if (call_time){

    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);

    pressure_event.pressure = pressure_event.pressure * 0.750062;

    // Check if reading was successful
    if(pressure_event.pressure) //&& pressure_event.pressure && aht20.startMeasurementReady(true) 
    {
      // Serial.println(temp_event.temperature);
      Serial.println(pressure_event.pressure);
      // Serial.println(aht20.getTemperature_C());
      // Serial.println(aht20.getHumidity_RH());

    }
    else
    {
      Serial.println("Error");
    }
    delay(call_time);
  }
}
