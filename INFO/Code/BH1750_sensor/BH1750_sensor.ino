#include <BH1750.h>

BH1750 lightMeter;
int call_time = 0;

void setup() {
  // start serial port  
	Serial.begin(115200); 
	Serial.setTimeout(1); 
  lightMeter.begin();
  delay(200);
}

void loop() 
{
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("12");
  }
  if (call_time){
    uint16_t lux = lightMeter.readLightLevel();
    Serial.println(lux);
    delay(call_time);
  }
}

