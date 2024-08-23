#include <Wire.h>      // подключаем библиотеку

void setup() {
  Wire.begin();        // запускаем шину без адреса (мастер)
  Serial.begin(230400);  // запускаем ком-порт для монитора
}

void loop() {
  Wire.requestFrom(8, 6);    // запрашиваем 6 байт с устройства номер 8

  while (Wire.available()) { // пока есть что считывать,
    char c = Wire.read();    // считываем,
    Serial.print(c);         // и выводим в монитор
  }


  delay(500);                // перерыв полсекунды
}