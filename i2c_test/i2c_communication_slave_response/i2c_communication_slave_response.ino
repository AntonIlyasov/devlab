#include <Wire.h>               // подключаем библиотеку

void setup() {
  Wire.begin(8);                // запускаем шину на адресе 8
  Wire.onRequest(requestEvent); // назначаем функцию отправки данных
}

void loop() {
}

void requestEvent() {     // фенкция, вызывается автоматически при получении запроса от мастера
  uint8_t bytes[6] = {'h','e','l','l','o',' '};

  Wire.write((uint8_t*)&bytes, sizeof(bytes));
}