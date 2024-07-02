#define TURBIDITYSENSORPIN A1 //Ambient light sensor reading 

int call_time = 0;

void setup() {
  pinMode(TURBIDITYSENSORPIN,  INPUT);  
  // start serial port
  Serial.begin(9600);
}

void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("15");
  }
  if (call_time){
    float reading = analogRead(TURBIDITYSENSORPIN); //Read light level
    float sensorVoltage = reading / 1024 * 5.0;
    Serial.println(sensorVoltage);
    delay(call_time);
  }
}