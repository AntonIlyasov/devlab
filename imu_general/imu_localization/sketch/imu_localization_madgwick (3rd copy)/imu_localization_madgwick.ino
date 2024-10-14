#include <MadgwickAHRS.h>
#include <Wire.h>
#include "integrator.h"
#include "GY_85.h"
#include <BasicLinearAlgebra.h>
#include <math.h>
#include <WiFi.h>

#define CONTROLLER (0x08)            // Device address of controller

int velFromIMU   = 0;
int moveFromIMU  = 0;

// Настройки точки доступа
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

const int up   = 1;
const int down = -1;

int update_freq = 50;             // Hz.

TaskHandle_t Task1;
TaskHandle_t Task0;

GY_85 GY85;
Madgwick filter;
unsigned long startProgrammTime = 0;

float a11 = 0.999327277469047;
float a12 = -0.0231522205498705;
float a13 = 0.0483052411761684;
float a21 = 0.02133573053548;
float a22 = 0.998966644519848;
float a23 = 0.00472632807009919;
float a31 = -0.0551072790552674;
float a32 = -0.052973922817746;
float a33 = 0.998348682390864;
float tx = -0.0389218546056677;
float ty = 0.0147294567160533;
float tz = -0.0948430711474935;
float ax = 0;
float ay = 0.000110613889490412;
float az = -0.000022656581599363;
float kx = 0.959516958428762;
float ky = 0.962464090497743;
float kz = 1.00446241883244;
float bx = 0.00397099804287237;
float by = 0.0111226940412346;
float bz = 0.00925784589535118;

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

  void filter_madgwick(){    
    filter.updateIMU(drone_state.gx, drone_state.gy, drone_state.gz, drone_state.ax, drone_state.ay, drone_state.az);
    if (drone_state.firstMadgwickMeasure){
      drone_state.offset_roll_x   = filter.getRoll();
      drone_state.offset_pitch_y  = filter.getPitch();
      drone_state.offset_yaw_z    = filter.getYaw();
      drone_state.firstMadgwickMeasure = false;
    }
    drone_state.roll_x_from_Madgwick  = filter.getRoll()  - drone_state.offset_roll_x;
    drone_state.pitch_y_from_Madgwick = filter.getPitch() - drone_state.offset_pitch_y;
    drone_state.yaw_z_from_Madgwick   = filter.getYaw()   - drone_state.offset_yaw_z;
  }

  void update_state(float dt){        // dt - [sec]

            // START COMMUNICATION WITH IMU //
            
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);

            // GET DATA FROM SENSOR //

    // get current linear acceleration
    float X_out, Y_out, Z_out;  // Outputs
    X_out = (Wire.read() | Wire.read() << 8);                   // получаю сырые ускорения
    drone_state.ax = X_out / 128;
    Y_out = (Wire.read() | Wire.read() << 8);
    drone_state.ay = Y_out / 128;
    Z_out = (Wire.read() | Wire.read() << 8);
    drone_state.az = Z_out / 128;

    // get current angular velocities
    float* gyroReadings = GY85.readGyro();
    drone_state.gx = GY85.gyro_x(gyroReadings);                 // получаем сырые угловые скорости
    drone_state.gy = GY85.gyro_y(gyroReadings);
    drone_state.gz = GY85.gyro_z(gyroReadings);

                // CALCULATE //

    do_offset_accelerations();                                  // получаю сырые ускорения в СК коробки
    filter_madgwick();                                          // получаю ориентацию коробки

    // // get current angular movements
    // drone_state.droll_x  = integrator_gx->update(drone_state.gx, dt);   // получаем углы через угловые скорости
    // drone_state.dpitch_y = integrator_gy->update(drone_state.gy, dt);
    // drone_state.dyaw_z   = integrator_gz->update(drone_state.gz, dt);

    // drone_state.roll_x  += drone_state.droll_x;
    // drone_state.pitch_y += drone_state.dpitch_y;
    // drone_state.yaw_z   += drone_state.dyaw_z;

    calculate_aW();                                                     // получаем мировые ускорения

    // if (abs(drone_state.axW) <= 0.03) drone_state.axW = 0;              // фильтруем мировые ускорения
    // if (abs(drone_state.ayW) <= 0.03) drone_state.ayW = 0;
    // if (abs(1 - drone_state.azW) <= 0.05) drone_state.azW = 1;

    // get current linear velocities in GSK
    drone_state.dvxW = integrator_ax->update(drone_state.axW*9.81,        dt);    // получаем линейные скорости через ускорения
    drone_state.dvyW = integrator_ay->update(drone_state.ayW*9.81,        dt);
    drone_state.dvzW = integrator_az->update(drone_state.azW*9.81 - 9.81, dt);

    // Serial.print("     dvxW:");
    // Serial.print(drone_state.dvxW, 6);
    // Serial.print("     dvyW:");
    // Serial.print(drone_state.dvyW, 6);
    // Serial.print("     dvzW:");
    // Serial.print(drone_state.dvzW, 6);     

    drone_state.vxW += drone_state.dvxW;
    drone_state.vyW += drone_state.dvyW;
    drone_state.vzW += drone_state.dvzW;

    // get current linear movement
    drone_state.dxW = integrator_vx->update(drone_state.vxW, dt);     // получаем линейные перемещения через скорости
    drone_state.dyW = integrator_vy->update(drone_state.vyW, dt);
    drone_state.dzW = integrator_vz->update(drone_state.vzW, dt);

    // Serial.print("     dxW:");
    // Serial.print(drone_state.dxW, 6);
    // Serial.print("     dyW:");
    // Serial.print(drone_state.dyW, 6);
    // Serial.print("     dzW:");
    // Serial.print(drone_state.dzW, 6);        !!

    drone_state.xW += drone_state.dxW;
    drone_state.yW += drone_state.dyW;
    drone_state.zW += drone_state.dzW;

    velFromIMU   = trunc(drone_state.vxW*1000);
    moveFromIMU  = trunc(drone_state.xW*1000);
    
    // Serial.print("roll_x_from_Madgwick:");
    // Serial.print(drone_state.roll_x_from_Madgwick);
    // Serial.print("     pitch_y_from_Madgwick:");
    // Serial.print(drone_state.pitch_y_from_Madgwick);
    // Serial.print("     yaw_z_from_Madgwick:");
    // Serial.println(drone_state.yaw_z_from_Madgwick);

    // Serial.print("     roll_x:");
    // Serial.print(drone_state.roll_x);
    // Serial.print("     pitch_y:");
    // Serial.print(drone_state.pitch_y);
    // Serial.print("     yaw_z:");
    // Serial.println(drone_state.yaw_z);

    // Serial.print("     ax:");
    // Serial.print(drone_state.ax);
    // Serial.print("     ay:");
    // Serial.print(drone_state.ay);
    // Serial.print("     az:");
    // Serial.print(drone_state.az);

    // Serial.print("     axW:");
    // Serial.print(drone_state.axW);
    // Serial.print("     ayW:");
    // Serial.print(drone_state.ayW);
    // Serial.print("     azW:");
    // Serial.println(drone_state.azW);

    // Serial.print("     vxW:");
    // Serial.print(drone_state.vxW);
    // Serial.print("     vyW:");
    // Serial.print(drone_state.vyW);
    // Serial.print("     vzW:");
    // Serial.println(drone_state.vzW);

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

    // Serial.print("xW:");
    // Serial.print(drone_state.xW,3);
    // Serial.print("     yW:");
    // Serial.println(drone_state.yW,3);
    // Serial.print("     zW:");
    // Serial.println(drone_state.zW,3);
    // Serial.print("     up:");
    // Serial.println(up);
    // Serial.print("     down:");
    // Serial.println(down);

    // int xWmm = trunc(drone_state.xW*1000);
    // int yWmm = trunc(drone_state.yW*1000);
    // int zWmm = trunc(drone_state.zW*1000);

    // Serial.print("xWmm:");
    // Serial.print(xWmm);
    // Serial.print("     yWmm:");
    // Serial.print(yWmm);
    // Serial.print("     zWmm:");
    // Serial.println(zWmm);

    // sendPositionToControllerUART();
    // sendPositionToControllerI2C();

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
    // float roll_x  = 0;
    // float pitch_y = 0;
    // float yaw_z   = 0;
    float roll_x_from_ax_rad  = 0;
    float pitch_y_from_ay_rad = 0;
    float yaw_z_from_az_rad   = 0;

    float roll_x_from_Madgwick  = 0;
    float pitch_y_from_Madgwick = 0;
    float yaw_z_from_Madgwick   = 0;

    // float droll_x;
    // float dpitch_y;
    // float dyaw_z;
    float gx;
    float gy;
    float gz;

    bool firstMadgwickMeasure = true;
    float offset_roll_x   = 0;
    float offset_pitch_y  = 0;
    float offset_yaw_z    = 0;
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

    float roll_x_rad  = deg2rad(drone_state.roll_x_from_Madgwick);
    float pitch_y_rad = deg2rad(drone_state.pitch_y_from_Madgwick);
    float yaw_z_rad   = deg2rad(drone_state.yaw_z_from_Madgwick);

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
                             0,  1,  0, 
                           -sp,  0, cp};

    BLA::Matrix<3, 3> Rz = {cy, -sy, 0, 
                            sy,  cy, 0, 
                             0,   0, 1};

    BLA::Matrix<3, 3> R = Rz*Ry*Rx;     // Rx*Ry*Rz;      // Rz*Ry*Rx;

    BLA::Matrix<3> aL = {drone_state.ax,
                         drone_state.ay,
                         drone_state.az};

    BLA::Matrix<3> aW = R*aL;
    drone_state.axW = aW(0);
    drone_state.ayW = aW(1);
    drone_state.azW = aW(2);

    // Serial.print("roll_x_rad:");
    // Serial.print(roll_x_rad);
    // Serial.print("     pitch_y_rad:");
    // Serial.print(pitch_y_rad);
    // Serial.print("     yaw_z_rad:");
    // Serial.print(yaw_z_rad);
  }

  void do_offset_accelerations(){
    BLA::Matrix<4, 4> M_Rot_Trans = { a11, a12, a13, tx,
                                      a21, a22, a23, ty,
                                      a31, a32, a33, tz,
                                      ax,  ay,  az,  1};

    BLA::Matrix<4> input = {kx*drone_state.ax+bx,
                            ky*drone_state.ay+by,
                            kz*drone_state.az+bz,
                            1};

    BLA::Matrix<4> result = M_Rot_Trans*input;

    drone_state.ax = result(0);
    drone_state.ay = result(1);
    drone_state.az = result(2);
  }

  void sendPositionToControllerUART(){

    int xWmm = trunc(drone_state.xW*1000);
    int vxWmm = trunc(drone_state.vxW*1000);
    int axWmm = trunc(drone_state.axW*1000);

    // Отправляем три числа последовательно
    Serial.write((uint8_t*)&xWmm, sizeof(xWmm));
    Serial.write((uint8_t*)&vxWmm, sizeof(vxWmm));
    Serial.write((uint8_t*)&axWmm, sizeof(axWmm));

  }

  void sendPositionToControllerI2C(){

    int xWmm = trunc(drone_state.xW*1000);
    int vxWmm = trunc(drone_state.vxW*1000);
    int axWmm = trunc(drone_state.axW*1000);

    Wire.beginTransmission(CONTROLLER);
    Wire.write((uint8_t*)&xWmm, sizeof(xWmm));
    Wire.write((uint8_t*)&vxWmm, sizeof(vxWmm));
    Wire.write((uint8_t*)&axWmm, sizeof(axWmm));
    Wire.endTransmission();

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
  while (Serial.available()) Serial.read();
  GY85.init();
  Wire.begin();
  configureADXL345(); // Configure the sensor
  filter.begin(update_freq);
  startProgrammTime = millis();

  xTaskCreatePinnedToCore(
      WiFiLogic, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0);

  xTaskCreatePinnedToCore(
      IMULogic, /* Function to implement the task */
      "Task0", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &Task0,  /* Task handle. */
      0);

  // Создаем Wi-Fi точку доступа
  WiFi.softAP(ssid, password);

  // Выводим IP адрес точки доступа
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Запускаем сервер
  server.begin();
}

void loop() {}

void IMULogic(void *pvParameters){
  for (;;) {
    unsigned long start = millis(); 
    drone.update_state(1./update_freq);
    unsigned long end = millis();
    delay(1000/update_freq - (end - start) - 1);
    delay(1);
  }
  vTaskDelete(NULL);
}

void WiFiLogic(void *pvParameters) {
  for (;;) {
    // Ожидание клиента
    WiFiClient client = server.available();
    
    if (client) {
      Serial.println("New Client Connected");

      while (client.connected()){ 
        if (client.available()) {
          
          int start_1 = millis();
          // Читаем сообщение от клиента
          String request = client.readStringUntil('\n');

          Serial.print("Received: ");
          Serial.println(request);
          if (request.length() > 0 && request != "move_request"){
            Serial.println("request.length() > 0 && request != move_request");
            // Парсим полученные числа
            int pwm, duration;
            sscanf(request.c_str(), "%d,%d", &pwm, &duration);

            uint8_t bytes[4];
            bytes[0] = pwm & 0xFF;
            bytes[1] = (pwm >> 8) & 0xFF;
            bytes[2] = duration & 0xFF;
            bytes[3] = (duration >> 8) & 0xFF;
            
            Wire.beginTransmission(CONTROLLER);
            Serial.println("SEND:");
            // Вывод результата в сериал
            for (int i = 0; i < 4; i++) {
              Wire.write(bytes[i]);
              Serial.print(bytes[i], HEX);
              Serial.print(" ");
            }
            Wire.endTransmission();
            Serial.println();

            // Отправляем полученные числа

            // Wire.write((uint8_t*)&pwm, sizeof(pwm));
            // Wire.write((uint8_t*)&duration, sizeof(duration));

          } else if (request == "move_request"){
            Serial.println("request = move_request");
          }

          // delay(3000);
          
          Wire.requestFrom(CONTROLLER, 9);

          if (Wire.available() == 9) {
            uint8_t bytes[9];
            Serial.println("RECEIVED:");
            for (int i = 0; i < 9; i++) {
              bytes[i] = Wire.read();
              Serial.print(bytes[i], HEX);
              Serial.print(" ");
            }
            Serial.println();
            
            int val_1 = (bytes[1] << 8) | bytes[0];
            int val_2 = (bytes[3] << 8) | bytes[2];
            int val_3 = (bytes[5] << 8) | bytes[4];
            int val_4 = (bytes[7] << 8) | bytes[6];
            uint8_t data_ready = bytes[8];

            int val_mean = (val_1 + val_2 + val_3 + val_4) / 4;

            Serial.print(val_1);
            Serial.print(" ");
            Serial.print(val_2);
            Serial.print(" ");
            Serial.print(val_3);
            Serial.print(" ");
            Serial.print(val_4);
            Serial.print(" ");
            Serial.println(val_mean);

            int responseInts[2] = {0};
            Serial.print("data_ready: ");
            Serial.println(data_ready);
            if (data_ready){
              responseInts[0] = 0;
              responseInts[1] = -1;
            } else {
              responseInts[0] = val_mean;
              responseInts[1] = velFromIMU;
            }
            client.write((uint8_t *)responseInts, sizeof(responseInts));
            Serial.println("Sent response: ");
            for (int i = 0; i < 2; i++) {
              Serial.print("int");
              Serial.print(i + 1);
              Serial.print(": ");
              Serial.println(responseInts[i]);
            }
          }
          Serial.print("TIME client.available():");
          Serial.println(millis() - start_1);
          Serial.println();
        }
      }
      // Закрываем соединение
      client.stop();
      Serial.println("Client Disconnected");
    }
    delay(1);
  }
  vTaskDelete(NULL);
}