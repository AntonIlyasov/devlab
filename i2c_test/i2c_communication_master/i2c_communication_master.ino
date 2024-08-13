#include <Wire.h>
#define CONTROLLER (0x08)            // Device address of controller

void sendPositionToController(float xW, float yW, float zW){

  // подготовка данных для отправки
  long int xWmm = trunc(xW * 1000);
  long int yWmm = trunc(yW * 1000);
  long int zWmm = trunc(zW * 1000);

  Serial.print(xW);
  Serial.print(" ");
  Serial.println(xWmm);
  Serial.print(yW);
  Serial.print(" ");
  Serial.println(yWmm);
  Serial.print(zW);
  Serial.print(" ");
  Serial.println(zWmm);
  Serial.println();

  // отправляем данные на контроллер
  Wire.beginTransmission(CONTROLLER);
  Wire.write((uint8_t*)&xWmm, sizeof(xWmm));
  Wire.write((uint8_t*)&yWmm, sizeof(yWmm));
  Wire.write((uint8_t*)&zWmm, sizeof(zWmm));
  Wire.endTransmission();

}

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  float xW = random(100, 1000) / 10.;
  float yW = random(100, 1000) / 10.;
  float zW = random(100, 1000) / 10.;
  sendPositionToController(xW, yW, zW);
  delay(1000);
}
