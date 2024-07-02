#include <Wire.h>
#define samplingInterval 20                                           // Интервал в мс между измерениями

int    BH1750_Device = 0x23;                                          // I2C address for BH1720 light sensor
unsigned int lux, lux_av; 
int    call_time = 0;

     
void setup() {
  // start serial port
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(BH1750_Device);
  Wire.write(0x10);                                                  // sets resolution to 1 Lux
  Wire.endTransmission();
  delay(200);
}

void loop() 
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("12");
  }

  static unsigned long samplingTime = millis();

  //  Проводим измерения:
  if(millis() - samplingTime > samplingInterval){
    lux = BH1750_Read();
    lux_av = (lux_av + lux)/2;
    samplingTime = millis();
  }

  if (call_time){
    Serial.println(lux_av);
    delay(call_time);
  }
}

// subroutine

unsigned int BH1750_Read() {

  unsigned int i=0;
  Wire.beginTransmission (BH1750_Device);
  Wire.requestFrom (BH1750_Device, 2);
  while(Wire.available ()) 
  {
    i <<=8;
    i|= Wire.read();  
  }
  Wire.endTransmission();  
  return i/1.2;  // Convert to Lux
}
