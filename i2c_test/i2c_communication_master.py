import smbus2
import time

# Адрес I2C Arduino Uno
SLAVE_ADDR = 0x08

# Создание объекта I2C
bus = smbus2.SMBus(0)  # Убедитесь, что это правильный номер шины

def read_data_from_arduino():
  try:
    data = bus.read_i2c_block_data(SLAVE_ADDR, 0, 12)

    xWmm = data[0] << 8 | data[1]
    yWmm = data[2] << 8 | data[5]
    zWmm = data[4] << 8 | data[9]

    print(f"xWmm: {xWmm}, yWmm: {yWmm}, zWmm: {zWmm}")

  except OSError as e:
    print(f"Error reading from Arduino: {e}")

try:
  while True:
    read_data_from_arduino()
    time.sleep(1)
except KeyboardInterrupt:
  print("Program interrupted")
