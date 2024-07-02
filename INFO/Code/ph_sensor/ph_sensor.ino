#define SensorPin        A1                                                                   // pH meter Analog output to Arduino Analog Input 1
#define Offset           -6.00                                                                // Компенсация смещения
#define samplingInterval 20                                                                   // Интервал в мс между измерениями
#define ArrayLenth       40                                                                   // Количество выборок
int pHArray[ArrayLenth];                                                                      // Массив для определения среднего показания напряжения считанного с датчика
int pHArrayIndex=0;                                                                           // Индекс элемента массива pHArray значения которого требуется изменить

int call_time = 0;

void setup(void){
  // start serial port
  Serial.begin(9600);
}                                                                                             

void loop(void){
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.parseInt();
    call_time = command * 1000;
    Serial.println("14");
  }
  static unsigned long samplingTime = millis();
  static float pHValue, voltage;

  //  Проводим измерения:
  if(millis() - samplingTime > samplingInterval){                                         // Выполняем код в теле оператора if через каждые samplingInterval мс
    pHArray[pHArrayIndex++] = analogRead(SensorPin);                                      // Читаем данные в очередной элемент массива pHArray
    if(pHArrayIndex==ArrayLenth) pHArrayIndex=0;                                          // Если достигли последнего элемента массива pHArray, то сбрасываем номер текущего элемента этого массива в 0
    voltage = averagearray(pHArray, ArrayLenth) * 5.0 / 1023;                             // Получаем среднее напряжение в мВ из массива напряжений pHArray
    pHValue = 3.5 * voltage + Offset;                                                     // Преобразуем мВ в pH
    samplingTime = millis();                                                              // Обновляем время для переменной samplingTime
  }

  if (call_time){
    if (pHValue < 0) pHValue = 0;
    Serial.println(pHValue);
    delay(call_time);
  }
}


//  Функция определения среднего значения напряжения                                          // Эта функция возвращает среднее арифметическое значение данных массива arr без учёта одного максимального и одного минимального значения массива.
double averagearray(int* arr, int number){                                                    //
    int i,max,min;                                                                            // Объявляем переменные для цикла и экстремумов
    double avg;                                                                               // Объявляем переменную для вывода среднего значения
    long amount=0;                                                                            // Определяем переменную для подсчёта среднего значения
    if(number<=0){ Serial.println("Error number for the array to avraging!/n");  return 0;}   // В массиве arr не может быть 0 и менее элементов
    if(number< 5){ for(i=0; i<number; i++){amount+=arr[i];} avg = amount/number; return avg;  // Если в массиве arr менее 5 элементов, то среднее значение является средним арифметическим значением
    }else{                                                                                    // Если в массиве arr более 5 элементов, то среднее значение считаем иначе ...
        if(arr[0]<arr[1]){ min = arr[0]; max=arr[1];}                                         // Определяем минимальное и максимальное число из первых двух элементов массива
        else             { min = arr[1]; max=arr[0];}                                         // Определяем минимальное и максимальное число из первых двух элементов массива
        for(i=2; i<number; i++){                                                              // Проходим по остальным элементам массива
                 if(arr[i]<min){ amount+=min; min=arr[i]; }                                   // Если значение очередного элемента меньше минимального,  то добавляем к значению amount предыдущее минимальное значение  и обновляем значение min
            else if(arr[i]>max){ amount+=max; max=arr[i]; }                                   // Если значение очередного элемента больше максимального, то добавляем к значению amount предыдущее максимальное значение и обновляем значение max
            else               { amount+=arr[i];          }                                   // Если значение очередного элемента находится в пределах между min и max, то добавляем значение этого элемента к amount
        }                                                                                     //
        avg = (double) amount/(number-2);                                                     // Получаем среднее арифметическое значение (без учета значений первых двух элементов массива arr, т.к. они не добавлялись к amount)
    }                                                                                         //
    return avg;                                                                               // Возвращаем полученное среднее значение
}     
