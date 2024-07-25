#include "GY_85.h"
#include "integrator.h"

#define UPDATE_FREQ 30  // Hz

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

    // get current linear acceleration
    int* accelerometerReadings = GY85.readFromAccelerometer();
    drone_state.ax = GY85.accelerometer_x(accelerometerReadings);
    drone_state.ay = GY85.accelerometer_y(accelerometerReadings);
    drone_state.az = GY85.accelerometer_z(accelerometerReadings);

    // get current angular velocities
    float* gyroReadings = GY85.readGyro();
    drone_state.wx = GY85.gyro_x(gyroReadings);
    drone_state.wy = GY85.gyro_y(gyroReadings);
    drone_state.wz = GY85.gyro_z(gyroReadings);

    // get current linear velocities
    drone_state.vx = integrator_ax->update(drone_state.ax, dt);
    drone_state.vy = integrator_ay->update(drone_state.ay, dt);
    drone_state.vz = integrator_az->update(drone_state.az, dt);

    Serial.print("linear acceleration");
    Serial.print("\tx:");
    Serial.print(drone_state.ax);
    Serial.print("\ty:");
    Serial.print(drone_state.ay);
    Serial.print("\tz:");
    Serial.println(drone_state.az);

    // Serial.print("linear velocities");
    // Serial.print("\tx:");
    // Serial.print(drone_state.vx);
    // Serial.print("\ty:");
    // Serial.print(drone_state.vy);
    // Serial.print("\tz:");
    // Serial.println(drone_state.vz);

    // get current linear movement
    drone_state.dx = integrator_vx->update(drone_state.vx, dt);
    drone_state.dy = integrator_vy->update(drone_state.vy, dt);
    drone_state.dz = integrator_vz->update(drone_state.vz, dt);

    // get current angular movements
    drone_state.droll_x  = integrator_wx->update(drone_state.wx, dt);
    drone_state.dpitch_y = integrator_wy->update(drone_state.wy, dt);
    drone_state.dyaw_z   = integrator_wz->update(drone_state.wz, dt);
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

void setup() {
  Serial.begin(9600);
  GY85.init();
}

void loop() {  
  drone.update_state(1/UPDATE_FREQ);
  delay(1/UPDATE_FREQ*1000);
}