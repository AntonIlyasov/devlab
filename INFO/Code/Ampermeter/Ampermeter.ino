// пин подключения контакта OUT
#define PIN_OUT A1
// подключение библиотеки
#include <TroykaCurrent.h>

// создание объекта
ACS712 dataI(PIN_OUT);

int call_time = 0;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
}

void loop()
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("3");
  }
  if (call_time){

    float in_current = dataI.readCurrentDC() ;         // Расчет силы тока
    Serial.println(in_current);

    delay(call_time);
  }
}
