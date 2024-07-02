#include "GY_85.h"

// Create module object
GY_85 GY85;

int call_time = 0;

void setup(void)
{
  // Initialize the serial communication:
  Serial.begin(9600);

  // Initialize module
  GY85.init();
}

void loop(void)
{
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("17");
  }
  if (call_time){
    // Read data from sensors
    int* accelerometerReadings = GY85.readFromAccelerometer();
    float ax = GY85.accelerometer_x(accelerometerReadings) * 9.81 * (4/1024.); 
    float ay = GY85.accelerometer_y(accelerometerReadings) * 9.81 * (4/1024.); 
    float az = GY85.accelerometer_z(accelerometerReadings) * 9.81 * (4/1024.); 
    
    int* compassReadings = GY85.readFromCompass();
    int cx = GY85.compass_x(compassReadings);
    int cy = GY85.compass_y(compassReadings);
    int cz = GY85.compass_z(compassReadings);

    float* gyroReadings = GY85.readGyro();
    float gx = GY85.gyro_x(gyroReadings);
    float gy = GY85.gyro_y(gyroReadings);
    float gz = GY85.gyro_z(gyroReadings);

    // Log it to serial port
    Serial.print(ax);
    Serial.print(" ");
    Serial.print(ay);
    Serial.print(" ");
    Serial.print(az);
    
    Serial.print(" ");
    Serial.print(cx);
    Serial.print(" ");
    Serial.print(cy);
    Serial.print(" ");
    Serial.print(cz);
    
    Serial.print(" ");
    Serial.print(gx);
    Serial.print(" ");
    Serial.print(gy);
    Serial.print(" ");
    Serial.println(gz);

    delay(call_time);
  }
}
