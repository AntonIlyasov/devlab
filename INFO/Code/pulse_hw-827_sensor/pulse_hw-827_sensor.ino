#include <iarduino_SensorPulse.h>                       
iarduino_SensorPulse Pulse(A1);                         
int call_time = 0;
                                                      
void setup(){             
  // start serial port
  Serial.begin(9600);

  // Start up the library                            
  Pulse.begin();                                  
}                                                      

void loop(){       
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("7");
  }
  if (call_time){
    if(Pulse.check(ISP_VALID)==ISP_CONNECTED){
      float pulse = Pulse.check(ISP_PULSE);
      Serial.println(pulse);
    } else {
      Serial.println("Error");
    }
    delay(call_time);
  }
}