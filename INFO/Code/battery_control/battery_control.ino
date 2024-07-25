#define analog_pin A0               // Вывод которому подключен датчик

int call_time = 0;

void setup(void)
{
  pinMode(analog_pin,  INPUT);  
  // start serial port  
	Serial.begin(115200); 
	Serial.setTimeout(1); 
}

void loop(void)
{
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("200");
  }
  if (call_time){
    float adc_value = analogRead(analog_pin);               // Считываем показания с аналогово вывода
    float in_voltage = adc_value / 1024 * 10.0;
    Serial.println(in_voltage);
    delay(call_time);
  }
}
