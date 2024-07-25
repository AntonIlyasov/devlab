#define analog_pin A1               // Вывод которому подключен датчик
int call_time = 0;

void setup(void)
{
  pinMode(analog_pin,  INPUT); 
  // start serial port
	Serial.begin(115200); 
	Serial.setTimeout(1); 
}

void loop() {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("5");
  }
  if (call_time){
    float in_humidity = analogRead(analog_pin);
    float soilmoisturepercent = map(in_humidity, 0, 1023, 100, 0);
    Serial.println(soilmoisturepercent);
    delay(call_time);
  }
}
