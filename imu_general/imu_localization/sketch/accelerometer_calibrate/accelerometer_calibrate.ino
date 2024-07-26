#include <Wire.h>

#define X_CALIB 0
#define Y_CALIB 1
#define Z_CALIB 2
#define MODE_CALIB Z_CALIB

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out; // Outputs
int X_offset = 0, Y_offset = 0, Z_offset = 0; // Offset values

void setup() {
  Serial.begin(230400);
  Wire.begin();
  configureADXL345(); // Configure the sensor
  // Note: You should calibrate upon re-powering the sensor
  // Uncomment if you want to calibrate
  calibrateADXL345(); // Calibrate the sensor
}

void calibrateADXL345() {
  // You should calibrate with the positive axis pointed upwards against gravity
  // You should calibrate each axis separately and change the offset constant, 
  // You need to run this code three times if you want to calibrate all three axes
  float numReadings = 1000;

#if MODE_CALIB == X_CALIB
  float xSum = 0;
#endif

#if MODE_CALIB == Y_CALIB
  float ySum = 0;
#endif

#if MODE_CALIB == Z_CALIB
  float zSum = 0;
#endif

  Serial.print("Beginning Calibration");
  Serial.println();
  for (int i = 0; i < numReadings; i++) {
    Serial.print(i);
    Serial.println();
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);
    X_out = (Wire.read() | Wire.read() << 8);
    Y_out = (Wire.read() | Wire.read() << 8);
    Z_out = (Wire.read() | Wire.read() << 8);

#if MODE_CALIB == X_CALIB
    xSum += X_out;
#endif

#if MODE_CALIB == Y_CALIB
    ySum += Y_out;
#endif

#if MODE_CALIB == Z_CALIB
    zSum += Z_out;
#endif

  }

#if MODE_CALIB == X_CALIB
  X_offset = (128 - xSum / numReadings) / 4;
  Serial.print("X_offset= " );
  Serial.print(X_offset);
#endif

#if MODE_CALIB == Y_CALIB
  Y_offset = (128 - ySum / numReadings) / 4;
  Serial.print("Y_offset= " );
  Serial.print(Y_offset);
#endif

#if MODE_CALIB == Z_CALIB
  Z_offset = (128 - (zSum / numReadings)) / 4;
  Serial.print("Z_offset= " );
  Serial.print(Z_offset);
#endif

  Serial.println();
  delay(1000);
  // We need to add it to the offset channel respectively =
  // Once you write to the offset channel you do not need to do it again,
  // unless you unpower and power the device
  // Each offset address is different for X, Y, and Z

#if MODE_CALIB == X_CALIB
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(X_offset);
  Wire.endTransmission();
#endif

#if MODE_CALIB == Y_CALIB
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(Y_offset);
  Wire.endTransmission();
#endif

#if MODE_CALIB == Z_CALIB
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(Z_offset);
  Wire.endTransmission();
#endif
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

void loop() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  X_out = (Wire.read() | Wire.read() << 8);
  X_out = X_out / 128;
  Y_out = (Wire.read() | Wire.read() << 8);
  Y_out = Y_out / 128;
  Z_out = (Wire.read() | Wire.read() << 8);
  Serial.print(Z_out);
  Z_out = Z_out / 128;

  // Serial.print("Xa= ");
  // Serial.print(X_out);
  // Serial.print("   Ya= ");
  // Serial.print(Y_out);
  Serial.print("   Za= ");
  Serial.println(Z_out);
  delay(10);
}