#include "LineSensor.h"
#include "LightSensor.h"
#include <ESP32Servo.h>
#include "hcsr04.h"
#include "esp32-hal-ledc.h"
#include <ESP32Encoder.h> 

#define ROBOT_DEBUG_MODE 0
volatile uint32_t ms_cnt = 0;
volatile uint16_t required_ms = 0;

volatile uint32_t _div = 0;

volatile bool start_moving = false;

hw_timer_t * timer = NULL;      //H/W timer defining (Pointer to the Structure)

ESP32Encoder * encoders;
const int ENCODER_CNT = 4;
int64_t enc_data[ENCODER_CNT] = {0};

 

// Robot my_protocol(Robot::ESP_MOTOR);

uint16_t inline make_uint16(uint8_t* buf) {
  return (buf[1] << 8) + buf[0];
}

int16_t inline make_int16(uint8_t* buf) {
  return (buf[1] << 8) + buf[0];
}



void _init_encoders() {
    delay(10);
    
    if (ROBOT_DEBUG_MODE) Serial.println("INIT ENCODERS");
    encoders = new ESP32Encoder[ENCODER_CNT];
    uint8_t out_enc_pin[8] = {4, 39, 17, 16, 34, 35, 36, 14};
    for (size_t i = 0; i < ENCODER_CNT; i++) {
        encoders[i].attachFullQuad(out_enc_pin[2*i], out_enc_pin[2*i+1]);
        encoders[i].setCount(0);
    }
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

void _init_motors() {
    delay(10);
    if (ROBOT_DEBUG_MODE) Serial.println("INIT MOTORS");
    const int PWM_FREQ       = 40000;
    const int PWM_RESOLUTION = 8;
    for (int i = 0; i<=7; i++) {
        ledcSetup(i, PWM_FREQ, PWM_RESOLUTION);
    }

    int m1A = 18;
    int m1B = 23;
    int m2A = 19;
    int m2B = 13;
    int m3A = 32;
    int m3B = 33;
    int m4A = 27;
    int m4B = 26;

    ledcAttachPin(m1A, 0);
    ledcAttachPin(m1B, 1);
    ledcAttachPin(m2A, 2);
    ledcAttachPin(m2B, 3);
    ledcAttachPin(m3A, 4);
    ledcAttachPin(m3B, 5);
    ledcAttachPin(m4A, 6);
    ledcAttachPin(m4B, 7);
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

void setup() {
  Serial.begin(115200);
  _init_encoders();
  _init_motors();
}


void move(int16_t speed1, int16_t speed2, int16_t speed3, int16_t speed4) {
  Serial.printf("\n=======================\n\nMOVE!\nSPD: %d|%d|%d|%d\n", speed1, speed2, speed3, speed4);
  Serial.printf("ENCODERS:\n");
  for (size_t i = 0; i < ENCODER_CNT; i++) {
    enc_data[i] = encoders[i].getCount();
    Serial.printf("|%ld ",enc_data[i]);

  }
  if (speed1 >= 0){
    ledcWrite(0, 0);
    ledcWrite(1, speed1);
  }
  else {
    ledcWrite(0, -speed1);
    ledcWrite(1, 0);
  }
  //second motor
  if (speed2 >= 0){
    ledcWrite(2, 0);
    ledcWrite(3, speed2);
  }
  else{
    ledcWrite(2, -speed2);
    ledcWrite(3, 0);
  }
  //third motor
  if (speed3 >= 0){
    ledcWrite(4, 0);
    ledcWrite(5, speed3);
  }
  else{
    ledcWrite(4, -speed3);
    ledcWrite(5, 0);
  }
  //fourth motor
  if (speed4 >= 0){
    ledcWrite(6, 0);
    ledcWrite(7, speed4);
  }
  else {
    ledcWrite(6, -speed4);
    ledcWrite(7, 0);
  }
}
void loop() {
  move(127,127,127,127);
  delay(100);
}