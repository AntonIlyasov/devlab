void setup() {
  Serial.begin(9600); // Устанавливаем скорость передачи данных 9600 бод
}

void loop() {
  int32_t number1 = 12345678;
  int32_t number2 = -12345678;
  int32_t number3 = 1122334455;

  // Отправляем три числа последовательно
  Serial.write((uint8_t*)&number1, sizeof(number1));
  Serial.write((uint8_t*)&number2, sizeof(number2));
  Serial.write((uint8_t*)&number3, sizeof(number3));
  
  delay(1000); // Задержка 1 секунда перед следующей отправкой
}
