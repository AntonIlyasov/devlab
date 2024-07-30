#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);                             // I2C адрес Arduino
  Wire.onRequest(requestEvent);                 // Обработчик запроса
  while(!Serial) delay(10);
  Serial.println("success setup");
}

void requestEvent() {    
  float xW = random(100, 1000) / 10.;
  float yW = random(100, 1000) / 10.;
  float zW = random(100, 1000) / 10.;

  int xWmm = xW * 1000;
  int yWmm = yW * 1000;
  int zWmm = zW * 1000;

  Serial.println(xWmm);
  Serial.println(yWmm);
  Serial.println(zWmm);

  // Отправка данных на Orange Pi
  Wire.write((uint8_t*)&xWmm, sizeof(xWmm));
  Wire.write((uint8_t*)&yWmm, sizeof(yWmm));
  Wire.write((uint8_t*)&zWmm, sizeof(zWmm));
}

void loop() {
  // Ничего не делаем в loop, ожидаем запросов
}
