import smbus
import time

# I2C адрес вашего устройства (например, Arduino)
SLAVE_ADDR = 0x08

# Создаем объект I2C
bus = smbus.SMBus(1)  # /dev/i2c-1

def read_data():
    try:
        # Чтение данных с устройства
        data = bus.read_i2c_block_data(SLAVE_ADDR, 0, 12)

        # Конвертируем полученные байты в три 32-битных целых числа (int)
        xWmm = int.from_bytes(data[0:4], byteorder='little', signed=True)
        yWmm = int.from_bytes(data[4:8], byteorder='little', signed=True)
        zWmm = int.from_bytes(data[8:12], byteorder='little', signed=True)
        
        print(f"xWmm: {xWmm}, yWmm: {yWmm}, zWmm: {zWmm}")
    except OSError as e:
        pass

try:
    while True:
        read_data()
        time.sleep(0.001)
except KeyboardInterrupt:
    print("Program interrupted")
