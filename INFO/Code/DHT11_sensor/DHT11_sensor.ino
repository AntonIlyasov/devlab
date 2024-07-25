#include <iarduino_DHT.h> // Подключаем библиотеку для работы с датчиком DHT
iarduino_DHT sensor(2);   // Объявляем объект, указывая номер вывода, к которому подключён модуль

int call_time = 0;

void setup(){
  // start serial port  
	Serial.begin(115200); 
	Serial.setTimeout(1); 
  delay(1000);            // Приостанавливаем выполнение скетча на 1 секунду, для перехода датчика в активное состояние
}

void loop(){  
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("10");
  }
  if (call_time){
    sensor.read();
    float sensor_hum = sensor.hum;
    float sensor_tem = sensor.tem;
    Serial.print(sensor_hum);
    Serial.print(" ");
    Serial.println(sensor_tem);
    delay(call_time);
  }
}
