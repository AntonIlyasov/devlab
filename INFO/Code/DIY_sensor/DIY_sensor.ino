#include <SPI.h>
#define LOG_PERIOD 10000
#define MAX_PERIOD 60000
unsigned long counts;
unsigned long cpm;
unsigned int multiplier;
unsigned long previousMillis;
int call_time = 0;
float mkzvHours = 0.0; // мкЗв/ч

void tube_impulse(){
  counts++;   
}

void setup(){
  counts = 0;
  cpm = 0;
  multiplier = MAX_PERIOD / LOG_PERIOD;
  // start serial port  
	Serial.begin(115200); 
	Serial.setTimeout(1); 
  pinMode(2, INPUT);
  attachInterrupt(1, tube_impulse, RISING);
}

void loop(){
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("8");
  }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > LOG_PERIOD){
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    mkzvHours = cpm / 151.0;
    counts = 0;
  }
  if (call_time){
    Serial.println(mkzvHours);
  }
} 