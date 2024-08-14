import serial
import struct

# Открываем последовательный порт. Замените '/dev/ttyS0' на нужный порт, если необходимо.
ser = serial.Serial('/dev/ttyS0', 230400)

def read_three_ints():
    # Читаем 12 байт (3 числа по 4 байта)
    data = ser.read(12)

    if len(data) == 12:
        # Конвертируем байты в три 32-битных числа (маленький порядок байтов)
        number1, number2, number3 = struct.unpack('<iii', data)
        return number1, number2, number3
    else:
        return None, None, None

try:
    while True:
        number1, number2, number3 = read_three_ints()
        if number1 is not None:
            print(f"Received numbers: {number1}, {number2}, {number3}")
except KeyboardInterrupt:
    ser.close()
    print("Program interrupted")
