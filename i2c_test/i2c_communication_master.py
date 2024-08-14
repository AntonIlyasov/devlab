# sudo apt-get install python3-smbus

import smbus
import time

# Адрес I2C Arduino Uno
SLAVE_ADDR = 0x08

# Создание объекта I2C
bus = smbus.SMBus(1)  # Убедитесь, что это правильный номер шины

def read_data_from_arduino():
  try:
    data = bus.read_i2c_block_data(SLAVE_ADDR, 0, 12)
    
    # Конвертируем полученные байты в три 32-битных целых числа (int)
    xWmm = int.from_bytes(data[0:4], byteorder='little', signed=True)
    yWmm = int.from_bytes(data[4:8], byteorder='little', signed=True)
    zWmm = int.from_bytes(data[8:12], byteorder='little', signed=True)

    print(f"xWmm: {xWmm}, yWmm: {yWmm}, zWmm: {zWmm}")

  except OSError as e:
    print(f"Error reading from Arduino: {e}")

try:
  while True:
    read_data_from_arduino()
    time.sleep(1)
except KeyboardInterrupt:
  print("Program interrupted")
