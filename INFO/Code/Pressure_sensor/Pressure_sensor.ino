#define analog_pin A1               // Вывод которому подключен датчик

int call_time = 0;

void setup() {
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
    Serial.println("13");
  }
  if (call_time){
    float press_val = analogRead(analog_pin);               
    float press_val_Pa = map(press_val, 0, 1023, 0, 500000);   // 0 - 500000 Pa
    Serial.println(press_val_Pa);
    delay(call_time);
  }
}
