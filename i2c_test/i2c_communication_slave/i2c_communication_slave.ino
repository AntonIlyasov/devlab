#include <Wire.h>

void setup() {
  Serial.begin(230400);
  Wire.begin(0x08);                             // I2C адрес Arduino
  Wire.onRequest(requestEvent);                 // Обработчик запроса
  while(!Serial) delay(10);
  Serial.println("success setup");
}

void requestEvent() {    
  float xW = random(10000, 100000) / 1000.;
  float yW = random(10000, 100000) / 1000.;
  float zW = random(10000, 100000) / 1000.;

  int32_t xWmm = trunc(xW * 1000);
  int32_t yWmm = trunc(yW * 1000);
  int32_t zWmm = trunc(zW * 1000);

  Serial.print(xWmm);
  Serial.print(" ");
  Serial.print(yWmm);
  Serial.print(" ");
  Serial.print(zWmm);
  Serial.println();

  // Отправка данных на Orange Pi
  Wire.write((uint8_t*)&xWmm, sizeof(xWmm));
  Wire.write((uint8_t*)&yWmm, sizeof(yWmm));
  Wire.write((uint8_t*)&zWmm, sizeof(zWmm));
}

void loop() {
  // Ничего не делаем в loop, ожидаем запросов
}
