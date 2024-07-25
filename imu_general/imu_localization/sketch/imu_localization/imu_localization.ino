#include <Wire.h>
#include "integrator.h"
#include "GY_85.h"
#include <BasicLinearAlgebra.h>
#include <math.h>

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
    integrator_gx = new Integrator(0.0);
    integrator_gy = new Integrator(0.0);
    integrator_gz = new Integrator(0.0);
  }

  void update_state(float dt){        // dt - [sec]

            // START COMMUNICATION WITH IMU //
            
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);

            // GET FROM SENSOR //

    // get current linear acceleration
    float X_out, Y_out, Z_out;  // Outputs
    X_out = (Wire.read() | Wire.read() << 8);
    drone_state.ax = X_out / 128;
    Y_out = (Wire.read() | Wire.read() << 8);
    drone_state.ay = Y_out / 128;
    Z_out = (Wire.read() | Wire.read() << 8);
    drone_state.az = Z_out / 128;

    // // get current angular velocities
    float* gyroReadings = GY85.readGyro();
    drone_state.gx = GY85.gyro_x(gyroReadings);
    drone_state.gy = GY85.gyro_y(gyroReadings);
    drone_state.gz = GY85.gyro_z(gyroReadings);
    // if (abs(drone_state.gx) < 0.1) drone_state.gx = 0;
    // if (abs(drone_state.gy) < 0.1) drone_state.gy = 0;
    // if (abs(drone_state.gz) < 0.1) drone_state.gz = 0;

                // CALCULATE //

    // get current angular movements
    drone_state.droll_x  = integrator_gx->update(drone_state.gx, dt);
    drone_state.dpitch_y = integrator_gy->update(drone_state.gy, dt);
    drone_state.dyaw_z   = integrator_gz->update(drone_state.gz, dt);

    drone_state.roll_x  += drone_state.droll_x;
    drone_state.pitch_y += drone_state.dpitch_y;
    drone_state.yaw_z   += drone_state.dyaw_z;

    // drone_state.roll_x_from_ax_rad  = asinf(drone_state.ay/sqrt(drone_state.ax*drone_state.ax+drone_state.ay*drone_state.ay+drone_state.az*drone_state.az));
    // drone_state.pitch_y_from_ay_rad = -asinf(drone_state.ax/sqrt(drone_state.ax*drone_state.ax+drone_state.ay*drone_state.ay+drone_state.az*drone_state.az));
    // drone_state.pitch_y_from_ay_rad  = atan2(drone_state.ax,drone_state.az);
    // drone_state.pitch_y_from_ay_rad  = atan2(drone_state.ax,sqrt(1 - drone_state.ax*drone_state.ax - drone_state.ay*drone_state.ay))
    // drone_state.roll_x_from_ax_rad  = atan2(drone_state.ay, drone_state.az);
    // drone_state.pitch_y_from_ay_rad = atan2(drone_state.ax, drone_state.az);
    drone_state.roll_x_from_ax_rad    = atan(drone_state.ay/drone_state.az);
    drone_state.pitch_y_from_ay_rad   = atan(-drone_state.ax/sqrt(drone_state.ay*drone_state.ay + drone_state.az*drone_state.az));
    drone_state.yaw_z_from_az_rad     = 0; // not enable

    calculate_aW();

    // get current linear velocities in GSK
    // Serial.println(dt, 6);
    drone_state.dvxW = integrator_ax->update(drone_state.axW, dt);
    drone_state.dvyW = integrator_ay->update(drone_state.ayW, dt);
    drone_state.dvzW = integrator_az->update(drone_state.azW, dt);

    drone_state.vxW += drone_state.dvxW;
    drone_state.vyW += drone_state.dvyW;
    drone_state.vzW += drone_state.dvzW;

    // get current linear movement
    drone_state.dxW = integrator_vx->update(drone_state.vxW, dt);
    drone_state.dyW = integrator_vy->update(drone_state.vyW, dt);
    drone_state.dzW = integrator_vz->update(drone_state.vzW, dt);

    drone_state.xW += drone_state.dxW;
    drone_state.yW += drone_state.dyW;
    drone_state.zW += drone_state.dzW;

    // Serial.print("roll_x:");
    // Serial.print(drone_state.roll_x);
    // Serial.print("     pitch_y:");
    // Serial.print(drone_state.pitch_y);
    // Serial.print("     yaw_z:");
    // Serial.print(drone_state.yaw_z);

    Serial.print("ax:");
    Serial.print(drone_state.ax);
    Serial.print("     ay:");
    Serial.print(drone_state.ay);
    Serial.print("     az:");
    Serial.print(drone_state.az);

    Serial.print("     axW:");
    Serial.print(drone_state.axW);
    Serial.print("     ayW:");
    Serial.print(drone_state.ayW);
    Serial.print("     azW:");
    Serial.println(drone_state.azW);

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
    // Serial.print(drone_state.gx);
    // Serial.print("     y:");
    // Serial.print(drone_state.gy);
    // Serial.print("     z:");
    // Serial.print(drone_state.gz);

    // Serial.print("     angulars:");
    // Serial.print("x:");
    // Serial.print(drone_state.roll_x);
    // Serial.print("     y:");
    // Serial.print(drone_state.pitch_y);
    // Serial.print("     z:");
    // Serial.println(drone_state.yaw_z);

    // Serial.print("linear coordinates: ");
    // Serial.print("x:");
    // Serial.print(drone_state.xW);
    // Serial.print("     y:");
    // Serial.print(drone_state.yW);
    // Serial.print("     z:");
    // Serial.println(drone_state.zW);

  }

  struct state
  {
  public:
    // linear
    float xW = 0;
    float yW = 0;
    float zW = 0;
    float dxW;
    float dyW;
    float dzW;
    float vxW = 0;
    float vyW = 0;
    float vzW = 0;
    float dvxW;
    float dvyW;
    float dvzW;
    float ax;
    float ay;
    float az;
    float axW;
    float ayW;
    float azW;
    
    // angular
    float roll_x  = 0;
    float pitch_y = 0;
    float yaw_z   = 0;
    float roll_x_from_ax_rad  = 0;
    float pitch_y_from_ay_rad = 0;
    float yaw_z_from_az_rad   = 0;
    float droll_x;
    float dpitch_y;
    float dyaw_z;
    float gx;
    float gy;
    float gz;
  };
  
private:
  Integrator* integrator_ax;
  Integrator* integrator_ay;
  Integrator* integrator_az;
  Integrator* integrator_vx;
  Integrator* integrator_vy;
  Integrator* integrator_vz;
  Integrator* integrator_gx;
  Integrator* integrator_gy;
  Integrator* integrator_gz;
  state       drone_state;

  float deg2rad(float x){
    return x * PI / 180;
  }

  float rad2deg(float x){
    return x * 180 / PI;
  }

  void calculate_aW(){

    float roll_x_rad  = deg2rad(drone_state.roll_x);
    float pitch_y_rad = deg2rad(drone_state.pitch_y);
    float yaw_z_rad   = deg2rad(drone_state.yaw_z);

    float sr = sin(roll_x_rad);
    float cr = cos(roll_x_rad);
    float sp = sin(pitch_y_rad);
    float cp = cos(pitch_y_rad);
    float sy = sin(yaw_z_rad);
    float cy = cos(yaw_z_rad);

    BLA::Matrix<3, 3> Rx = {1,  0,   0, 
                            0, cr, -sr, 
                            0, sr, cr};

    BLA::Matrix<3, 3> Ry = {cp,  0, sp, 
                            0,   1,  0, 
                            -sp, 0, cp};

    BLA::Matrix<3, 3> Rz = {cy, -sy, 0, 
                            sy,  cy, 0, 
                             0,   0, 1};

    BLA::Matrix<3, 3> R = Rz*Ry*Rx;     // Rx*Ry*Rz;      // Rz*Ry*Rx;

    BLA::Matrix<3> aL = { drone_state.ax,
                          drone_state.ay,
                          drone_state.az};

    BLA::Matrix<3> aW = R*aL;
    drone_state.axW = aW(0);
    drone_state.ayW = aW(1);
    drone_state.azW = aW(2);

    // Serial.print("drone_state.ax:");
    // Serial.print(drone_state.ax);
    // Serial.print("     drone_state.ay:");
    // Serial.print(drone_state.ay);
    // Serial.print("     drone_state.az:");
    // Serial.print(drone_state.az);

    // Serial.print("roll_x_from_ax_rad:");
    // Serial.print(drone_state.roll_x_from_ax_rad);
    // Serial.print("     pitch_y_from_ay_rad:");
    // Serial.print(drone_state.pitch_y_from_ay_rad);
    // Serial.print("     yaw_z_from_az_rad:");
    // Serial.print(drone_state.yaw_z_from_az_rad);

    // Serial.print("     roll_x_rad:");
    // Serial.print(roll_x_rad);
    // Serial.print("     pitch_y_rad:");
    // Serial.print(pitch_y_rad);
    // Serial.print("     yaw_z_rad:");
    // Serial.println(yaw_z_rad);
  }
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