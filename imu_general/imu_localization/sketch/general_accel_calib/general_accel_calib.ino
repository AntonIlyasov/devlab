#include <Wire.h>

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out; // Outputs
float X_mean = 0., Y_mean = 0., Z_mean = 0.; // Means values

void setup() {
  Serial.begin(230400);
  Wire.begin();
  configureADXL345(); // Configure the sensor
  calibrateADXL345(); // Calibrate the sensor
}

void calibrateADXL345() {

  float numReadings = 10000;
  float xSum = 0;
  float ySum = 0;
  float zSum = 0;

  Serial.println("Beginning Calibration");

  for (int i = 0; i < numReadings; i++) {
    Serial.println(i);

    Wire.beginTransmission(ADXL345);
    Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);

    X_out = (Wire.read() | Wire.read() << 8);
    Y_out = (Wire.read() | Wire.read() << 8);
    Z_out = (Wire.read() | Wire.read() << 8);

    xSum += X_out;
    ySum += Y_out;
    zSum += Z_out;

  }

  X_mean = xSum / numReadings / 128.0;
  Serial.print("X_mean= " );
  Serial.print(X_mean, 6);

  Y_mean = ySum / numReadings / 128.0;
  Serial.print("  Y_mean= " );
  Serial.print(Y_mean, 6);

  Z_mean = zSum / numReadings / 128.0;
  Serial.print("  Z_mean= " );
  Serial.print(Z_mean, 6);

  Serial.println();
  delay(1000);
}

void configureADXL345() {
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  Wire.beginTransmission( ADXL345 );      // start transmission to device
  Wire.write( 0x31 );                     // send register address
  Wire.write( 0x01 );                     // send value to write
  Wire.endTransmission();                 // end transmission

  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // Enable measurement (D3 bit high)
  Wire.endTransmission();
  delay(10);
}

void loop() {}