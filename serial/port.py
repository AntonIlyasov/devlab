import serial
import time

# Указываем параметры подключения
port = '/dev/ttyACM0'  
baud_rate = 115200  # Скорость передачи данных (например, 9600)
timeout = 0.1  # Тайм-аут в секундах

# Подключаемся к порту
ser = serial.Serial(port=port,   baudrate=baud_rate, timeout=.1)
time.sleep(0.05)

x = '1'
ser.write(bytes(x,   'utf-8'))
print('Send 1')

try:
    while True:
        while ser.in_waiting:
            # Считываем данные из порта
            data = ser.readline().decode('utf-8').rstrip()
            print(f"Received: {data}")
except KeyboardInterrupt:
    print("Прерывание программы пользователем.")
finally:
    # Закрываем порт
    ser.close()
    print("Порт закрыт.")
