#include "Robot.h"

Robot robot(Robot::ESP_M); // "Создадим" робота

int red = 0;
int grn = 0;
int blu = 0;

void setup() {
  Serial.println("Старт!");
  robot.init();
  // Этот блок кода выполнится один раз при старте программы:
}

void loop() {
  robot.moveServo(Robot::use_servo::tool_back_manip, 180);
  robot.moveServo(Robot::use_servo::pitch, 160);
  robot.moveServo(Robot::use_servo::elbow, 140);
  robot.moveServo(Robot::use_servo::base, 120);
  robot.moveServo(Robot::use_servo::cam_left_right, 100);
  robot.moveServo(Robot::use_servo::cam_up_down, 80);
  robot.moveServo(Robot::use_servo::up_down_tool_front, 60);
  robot.moveServo(Robot::use_servo::tool_front_manip, 90);
  robot.moveServo(Robot::use_servo::shoulder, 0);
  // put your main code here, to run repeatedly:
  robot.getColor(red, grn, blu);
  Serial.printf("R: %d G: %d B: %d\n", red, grn, blu);
  

}