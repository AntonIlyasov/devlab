#define analog_pin A1               // Вывод которому подключен датчик
 
float R1 = 30000.0;                 // Задаем номинал резистора R1
float R2 = 7500.0;                  // Задаем номинал резистора R2
float ref_voltage = 5.0;            // Задаем опорное напряжение

int call_time = 0;

void setup(void)
{
  pinMode(analog_pin,  INPUT);  
  // start serial port
  Serial.begin(9600);
}

void loop(void)
{
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("2");
  }
  if (call_time){
    float adc_value = analogRead(analog_pin);               // Считываем показания с аналогово вывода
    float adc_voltage = (adc_value * ref_voltage) / 1024.0; // Определение на входе АЦП
    float in_voltage = adc_voltage / (R2/(R1+R2)) ;         // Расчет напряжения
    Serial.println(in_voltage);
    delay(call_time);
  }
}
