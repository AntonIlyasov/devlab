#include <Wire.h>
#define I2C_DEV_ADDR 0x08

void setup() {
  Serial.begin(230400);
  while(!Serial) delay(10);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Wire.onReceive(onReceive);
  Serial.println("success setup");
}

void onReceive(int len) {

  if (len >= 4) { // Проверяем, что получили достаточно байт для двух int
    int velocity  = Wire.read() | (Wire.read() << 8); // Читаем первое число
    int time      = Wire.read() | (Wire.read() << 8); // Читаем второе число

    Serial.print("vel [mm/sec]: ");
    Serial.print(velocity);
    Serial.print("  time [sec]: ");
    Serial.println(time);
  }
}

void loop() {}