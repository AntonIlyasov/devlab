#include <Wire.h>
#include "esp32-hal-ledc.h"
#include <Vector.h>
#include <MotorDevs.h> 
#include <ESP32Encoder.h> 

#define I2C_DEV_ADDR 0x08
#define M1N 23
#define M1P 18
#define M2P 19
#define M2N 13
#define M3P 32
#define M3N 33
#define M4N 26
#define M4P 27
#define PI 3.141592654

const int ENC_NUM = 4;
ESP32Encoder wheelEncoders[ENC_NUM];

/* Пины A и B для энкодеров */
byte enc_A_pins[ENC_NUM] = {ENC1_A, ENC2_A, ENC3_A, ENC4_A};
byte enc_B_pins[ENC_NUM] = {ENC1_B, ENC2_B, ENC3_B, ENC4_B};

int64_t prev_enc_count[4] = {0,0,0,0};
int64_t curr_enc_count[4] = {0,0,0,0};

int revs[4] = {0,0,0,0};
int counters[4] = {0,0,0,0};
int velocity_storage[255];
Vector<int> velocity(velocity_storage);
int time_storage[255];
Vector<int> timer(time_storage);
int count = 0;
int last_time[4] = {0,0,0,0};
int speed[4] = {0,0,0,0};
float speedFloat[4] = {0.};
float radius = 0.07;
bool initialize = 1;
int direction = 1;
bool started = 0;
bool data_ready = 0;
int results[4] = {0,0,0,0};
TaskHandle_t Task1;
TaskHandle_t Task0;
void IRAM_ATTR motor1()
{
  counters[0]++;
  revs[0] += counters[0] / 330;
  counters[0] = counters[0] % 330;
  int sec = micros();
  speed[0] = (35*2*PI/330)/(sec - last_time[0])*1000000;   // (-1*(1-direction))*       // [mm/sec]
  last_time[0] = sec;
  curr_enc_count[0] = wheelEncoders[0].getCount();
  if (curr_enc_count[0] < prev_enc_count[0]) speed[0] *= (-1);
  prev_enc_count[0] = curr_enc_count[0];
  
}
void IRAM_ATTR motor2()
{
  counters[1]++;
  revs[1] += counters[1] / 330;
  counters[1] = counters[1] % 330;
  int sec = micros();
  speed[1] = (35*2*PI/330)/(sec - last_time[1])*1000000;   // (-1*(1-direction))*       // [mm/sec]
  last_time[1] = sec;
  curr_enc_count[1] = wheelEncoders[1].getCount();
  if (curr_enc_count[1] < prev_enc_count[1]) speed[1] *= (-1);
  prev_enc_count[1] = curr_enc_count[1];
}
void IRAM_ATTR motor3()
{
  counters[2]++;
  revs[2] += counters[2] / 330;
  counters[2] = counters[2] % 330;
  int sec = micros();
  speed[2] = (35*2*PI/330)/(sec - last_time[2])*1000000;   // (-1*(1-direction))*       // [mm/sec]
  last_time[2] = sec;
  curr_enc_count[2] = wheelEncoders[2].getCount();
  if (curr_enc_count[2] < prev_enc_count[2]) speed[2] *= (-1);
  prev_enc_count[2] = curr_enc_count[2];
}
void IRAM_ATTR motor4()
{
  counters[3]++;
  revs[3] += counters[3] / 330;
  counters[3] = counters[3] % 330;
  int sec = micros();
  speed[3] = (35*2*PI/330)/(sec - last_time[3])*1000000;   // (-1*(1-direction))*       // [mm/sec]
  last_time[3] = sec;
  curr_enc_count[3] = wheelEncoders[3].getCount();
  if (curr_enc_count[3] < prev_enc_count[3]) speed[3] *= (-1);
  prev_enc_count[3] = curr_enc_count[3];
}
void setup() {
  Serial.begin(230400);
  while(!Serial) delay(10);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRquest);
  Serial.println("success setup");
  xTaskCreatePinnedToCore(
                    motorCode,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,&Task1,      /* Task handle to keep track of created task */
                    0);
  xTaskCreatePinnedToCore(Task0code, "Task0", 10000, NULL, 1, &Task0, 1);
  ledcAttach(M1P, 30000, 8);
  ledcAttach(M1N, 30000, 8);
  ledcAttach(M2P, 30000, 8);
  ledcAttach(M2N, 30000, 8);
  ledcAttach(M3P, 30000, 8);
  ledcAttach(M4N, 30000, 8);
  ledcAttach(M4P, 30000, 8);
  ledcAttach(M4N, 30000, 8);
  pinMode(39, INPUT);
  pinMode(35, INPUT);
  pinMode(16, INPUT);
  pinMode(14, INPUT);
  attachInterrupt(39, motor1, RISING);
  attachInterrupt(35, motor3, RISING);
  attachInterrupt(16, motor2, RISING);
  attachInterrupt(14, motor4, RISING);

  for (int i = 0; i < ENC_NUM; i++) {
    /* Подключим энкодеры */
    wheelEncoders[i].attachFullQuad(enc_A_pins[i], enc_B_pins[i]);
    wheelEncoders[i].setCount(0);
  }
}

int minimum(int *value){
  int min_value = INT_MAX;

  // Поиск минимального значения
  if (value[0] < min_value) min_value = value[0];
  if (value[1] < min_value) min_value = value[1];
  if (value[2] < min_value) min_value = value[2];
  if (value[3] < min_value) min_value = value[3];

  return min_value;
}

void onRquest() {
  Serial.println("SEND:");
  if (!data_ready){
    for (int i=0; i<4; i++){

      if (speed[i] > 1210) speed[i] = minimum(speed);

      uint8_t leading_byte = speed[i] & 0xFF;
      uint8_t trailing_byte = (speed[i] >> 8) & 0xFF;
      Serial.println(speed[i]);  
      // Serial.print(leading_byte);
      // Serial.print(" ");
      // Serial.print(trailing_byte);
      // Serial.print(" ");
      Wire.write(leading_byte);
      Wire.write(trailing_byte);
    }
  }else{
    for (int i=0; i<4; i++){
      uint8_t leading_byte = results[i] & 0xFF;
      uint8_t trailing_byte = (results[i] >> 8) & 0xFF;
      Serial.println(results[i]);  
      // Serial.print(leading_byte);
      // Serial.print(" ");
      // Serial.print(trailing_byte);
      // Serial.print(" ");
      Wire.write(leading_byte);
      Wire.write(trailing_byte);
    }
  }
  Wire.write(data_ready);
  Serial.println(data_ready);
  Serial.println("STOP SEND");

}

void onReceive(int len) {

  if (len >= 4) { // Проверяем, что получили достаточно байт для двух int

    uint8_t bytes[4];
    Serial.println("RECEIVED:");
    for (int i = 0; i < 4; i++) {
      bytes[i] = Wire.read();
      Serial.print(bytes[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    int ppm  = (bytes[1] << 8) | bytes[0];
    int duration = (bytes[3] << 8) | bytes[2];

    uint8_t bytes2[4];
    bytes2[0] = ppm & 0xFF;
    bytes2[1] = (ppm >> 8) & 0xFF;
    bytes2[2] = duration & 0xFF;
    bytes2[3] = (duration >> 8) & 0xFF;

    velocity.push_back(ppm);
    timer.push_back(duration);

  }
}





void loop() {


}
void calculate_results(){
  for (int i=0; i<4; i++){
    results[i] = revs[i]*70*3.14*2 + counters[i]*70*3.14*2/330;
  }
}
void motors_write(int pwm, int time){
  int speeds[8] = {0,0,0,0,0,0,0,0};
  for (int i=1; i<8; i = i+2){
    speeds[i-direction] = pwm;
  }
  last_time[0] = 0;
  last_time[1] = 0;
  last_time[2] = 0;
  last_time[3] = 0;
  ledcWrite(M1P, speeds[0]);
  ledcWrite(M1N, speeds[1]);
  ledcWrite(M2P, speeds[2]);
  ledcWrite(M2N, speeds[3]);
  ledcWrite(M3P, speeds[4]);
  ledcWrite(M4N, speeds[5]);
  ledcWrite(M4P, speeds[6]);
  ledcWrite(M4N, speeds[7]);
  delay(time*1000);
}

void stop_motors(){
  ledcWrite(M1P, 0);
  ledcWrite(M1N, 0);
  ledcWrite(M2P, 0);
  ledcWrite(M2N, 0);
  ledcWrite(M3P, 0);
  ledcWrite(M4N, 0);
  ledcWrite(M4P, 0);
  ledcWrite(M4N, 0);
}
void Task0code ( void * pvParameters ){

    // Serial.print(revs[i]);
    // Serial.print("\t");
    // Serial.print(counters[0]);
    // Serial.print("\t");
  for(;;){
  if (Serial.available()>0){
    int ppm = Serial.parseInt();
    int dur = Serial.parseInt();
    velocity.push_back(ppm);
    timer.push_back(dur);
    // Serial.println(velocity.size());
}
for (int i=0; i<4; i++){
  if (speed[i] > 1210) speed[i] = minimum(speed);
  Serial.print(speed[i]);
  Serial.print("\t");
}
Serial.println();
delay(1);
}
vTaskDelete(NULL);
}

int checkNeg(int value){
  if (value > 32767){
    value = value - 65536;
  }
  return value;
}

void motorCode( void * pvParameters ){

  for(;;){
    if (velocity.size() > 0){
      if (!started){
        data_ready = 0;
        started = !started;
      }
      int speed = velocity[0];
      int duration = timer[0];
      velocity.remove(0);
      timer.remove(0);
      speed = checkNeg(speed);
      direction = speed > 0;
      motors_write(abs(speed), duration);
    }else{
      if (started){
        stop_motors();
        calculate_results();
        started = !started;
        data_ready = 1;
      }
    }
   delay(1);
  } 
  vTaskDelete(NULL);
}



