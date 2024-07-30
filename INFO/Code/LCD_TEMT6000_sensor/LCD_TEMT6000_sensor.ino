#define LIGHTSENSORPIN A1 //Ambient light sensor reading 

int call_time = 0;

void setup() {
  pinMode(LIGHTSENSORPIN,  INPUT);  
  // start serial port
	Serial.begin(115200); 
	Serial.setTimeout(1); 
}

void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("11");
  }
  if (call_time){
    float reading = analogRead(LIGHTSENSORPIN);
    float lux_value = map(reading, 0, 1023, 0, 4000);
    Serial.println(lux_value);
    delay(call_time);
  }
}