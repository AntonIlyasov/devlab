#include <ArduinoJson.h>
#include <Wire.h>
#define CONTROLLER (0x53)            // Device address of controller

void sendPositionToController(float xW, float yW, float zW){

  JsonDocument doc;

  // подготовка данных для отправки
  doc["xW"] = xW;
  doc["yW"] = yW;
  doc["zW"] = zW;
  serializeJson(doc, Serial);

  String data;
  serializeJson(doc, data);

  // отправляем данные на контроллер
  Wire.beginTransmission(CONTROLLER);
  Wire.write(data.c_str());
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
  Serial.println(xW);
  Serial.println(yW);
  Serial.println(zW);
  sendPositionToController(xW, yW, zW);
  delay(500);
}
