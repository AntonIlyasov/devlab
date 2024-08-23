#include <Wire.h>
#define I2C_DEV_ADDR 0x08

int32_t velocity_2  = 0;
int32_t time_2      = 0;

void setup() {
  Serial.begin(230400);
  while(!Serial) delay(10);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Wire.onReceive(onReceive);
  Wire.onRequest(requestEvent);
  Serial.println("success setup");
}

void onReceive(int len) {
  
  if (Wire.available() == 8) {
    uint8_t bytes[8];
    Serial.println("RECEIVED:");
    for (int i = 0; i < 8; i++) {
      bytes[i] = Wire.read();
      Serial.print(bytes[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    int32_t velocity = (int32_t(bytes[3]) << 24) | (int32_t(bytes[2]) << 16) | (int32_t(bytes[1]) << 8) | bytes[0];
    int32_t time = (int32_t(bytes[7]) << 24) | (int32_t(bytes[6]) << 16) | (int32_t(bytes[5]) << 8) | bytes[4];

    Serial.print("vel [mm/sec]: ");
    Serial.print(velocity);
    Serial.print("  time [sec]: ");
    Serial.println(time);

    velocity_2  = velocity * 2;
    time_2      = time * 2;
  }
}

void requestEvent() {
  Serial.print("velocity_2: ");
  Serial.print(velocity_2);
  Serial.print("  time_2: ");
  Serial.println(time_2);

  uint8_t bytes[8];

  // Разложение первого значения
  bytes[3] = (velocity_2 >> 24) & 0xFF;
  bytes[2] = (velocity_2 >> 16) & 0xFF;
  bytes[1] = (velocity_2 >> 8) & 0xFF;
  bytes[0] = velocity_2 & 0xFF;

  // Разложение второго значения
  bytes[7] = (time_2 >> 24) & 0xFF;
  bytes[6] = (time_2 >> 16) & 0xFF;
  bytes[5] = (time_2 >> 8) & 0xFF;
  bytes[4] = time_2 & 0xFF;

  Serial.println("SEND:");
  // Вывод результата в сериал
  for (int i = 0; i < 8; i++) {
    Serial.print(bytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Отправка данных на Orange Pi
  Wire.write((uint8_t*)&velocity_2, sizeof(velocity_2));
  Wire.write((uint8_t*)&time_2, sizeof(time_2));
}

void loop() {}