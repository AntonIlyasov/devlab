#include <Wire.h>
#include "integrator.h"
#include "GY_85.h"
#include <MatrixMath.h>

int update_freq = 100;            // Hz
// int ADXL345 = 0x53;            // The ADXL345 sensor I2C address

GY_85 GY85;

struct Drone_State
{
public:

  Drone_State(){
    integrator_ax = new Integrator(0.0);
    integrator_ay = new Integrator(0.0);
    integrator_az = new Integrator(0.0);
    integrator_vx = new Integrator(0.0);
    integrator_vy = new Integrator(0.0);
    integrator_vz = new Integrator(0.0);
    integrator_wx = new Integrator(0.0);
    integrator_wy = new Integrator(0.0);
    integrator_wz = new Integrator(0.0);
  }

  void update_state(float dt){

    Wire.beginTransmission(ADXL345);
    Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);

    // get current linear acceleration
    float X_out, Y_out, Z_out;  // Outputs
    X_out = (Wire.read() | Wire.read() << 8);
    drone_state.ax = X_out * 9.81 / 256;
    Y_out = (Wire.read() | Wire.read() << 8);
    drone_state.ay = Y_out * 9.81 / 256;
    Z_out = (Wire.read() | Wire.read() << 8);
    drone_state.az = Z_out * 9.81 / 256;

    // // get current angular velocities
    float* gyroReadings = GY85.readGyro();
    drone_state.wx = GY85.gyro_x(gyroReadings);
    drone_state.wy = GY85.gyro_y(gyroReadings);
    drone_state.wz = GY85.gyro_z(gyroReadings);

    // if (abs(drone_state.wx) < 0.1) drone_state.wx = 0;
    // if (abs(drone_state.wy) < 0.1) drone_state.wy = 0;
    // if (abs(drone_state.wz) < 0.1) drone_state.wz = 0;

    // get current linear velocities
    // Serial.println(dt, 6);
    drone_state.dvx = integrator_ax->update(drone_state.ax, dt);
    drone_state.dvy = integrator_ay->update(drone_state.ay, dt);
    drone_state.dvz = integrator_az->update(drone_state.az, dt);

    drone_state.vx += drone_state.dvx;
    drone_state.vy += drone_state.dvy;
    drone_state.vz += drone_state.dvz;

    // get current linear movement
    // drone_state.dx = integrator_vx->update(drone_state.vx, dt);
    // drone_state.dy = integrator_vy->update(drone_state.vy, dt);
    // drone_state.dz = integrator_vz->update(drone_state.vz, dt);

    // drone_state.x += drone_state.dx;
    // drone_state.y += drone_state.dy;
    // drone_state.z += drone_state.dz;

    // get current angular movements
    drone_state.droll_x  = integrator_wx->update(drone_state.wx, dt);
    drone_state.dpitch_y = integrator_wy->update(drone_state.wy, dt);
    drone_state.dyaw_z   = integrator_wz->update(drone_state.wz, dt);

    drone_state.roll_x  += drone_state.droll_x;
    drone_state.pitch_y += drone_state.dpitch_y;
    drone_state.yaw_z   += drone_state.dyaw_z;

    // Serial.print("linear acceleration");
    // Serial.print("     x:");
    // Serial.print(drone_state.ax);
    // Serial.print("     y:");
    // Serial.print(drone_state.ay);
    // Serial.print("     z:");
    // Serial.print(drone_state.az);

    // Serial.print("     linear velocities");
    // Serial.print("     x:");
    // Serial.print(drone_state.vx);
    // Serial.print("     y:");
    // Serial.print(drone_state.vy);
    // Serial.print("     z:");
    // Serial.print(drone_state.vz);

    // Serial.print("angular velocities");
    // Serial.print("     x:");
    // Serial.print(drone_state.wx);
    // Serial.print("     y:");
    // Serial.print(drone_state.wy);
    // Serial.print("     z:");
    // Serial.print(drone_state.wz);

    // Serial.print("     angulars:");
    Serial.print("x:");
    Serial.print(drone_state.roll_x);
    Serial.print("     y:");
    Serial.print(drone_state.pitch_y);
    Serial.print("     z:");
    Serial.println(drone_state.yaw_z);

  }

  struct state
  {
  public:
    // linear
    float x = 0;
    float y = 0;
    float z = 0;
    float dx;
    float dy;
    float dz;
    float vx;
    float vy;
    float vz;
    float dvx;
    float dvy;
    float dvz;
    float ax;
    float ay;
    float az;
    
    // angular
    float roll_x  = 0;
    float pitch_y = 0;
    float yaw_z   = 0;
    float droll_x;
    float dpitch_y;
    float dyaw_z;
    float wx;
    float wy;
    float wz;
  };
  
private:
  Integrator* integrator_ax;
  Integrator* integrator_ay;
  Integrator* integrator_az;
  Integrator* integrator_vx;
  Integrator* integrator_vy;
  Integrator* integrator_vz;
  Integrator* integrator_wx;
  Integrator* integrator_wy;
  Integrator* integrator_wz;
  state       drone_state;
};

// Drone state object
Drone_State drone;

void configureADXL345() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // Enable measurement (D3 bit high)
  Wire.endTransmission();
  delay(10);
}

void setup() {
  Serial.begin(230400);
  GY85.init();
  Wire.begin();
  configureADXL345(); // Configure the sensor
}

void loop() {
  unsigned long start = millis(); 
  drone.update_state(1./update_freq);
  // Serial.print("     time [ms]: ");
  unsigned long end = millis();
  // Serial.println(end - start);
  delay(1000/update_freq - (end - start));
}