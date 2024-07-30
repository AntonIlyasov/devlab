import time
import pigpio

# Адрес I2C slave устройства
SLAVE_ADDR = 0x08

def i2c_slave_callback(id, tick):
    global pi
    # Получаем данные с I2C
    (s, b, d) = pi.bsc_i2c(SLAVE_ADDR)
    if b:  # Если есть данные
        print("Received data:", d)

# Инициализация библиотеки pigpio
pi = pigpio.pi()
if not pi.connected:
    exit()

# Устанавливаем адрес slave
pi.bsc_i2c(SLAVE_ADDR)

# Устанавливаем callback для обработки данных
cb = pi.callback(0, pigpio.EITHER_EDGE, i2c_slave_callback)

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    pass
finally:
    # Отключаем I2C и освобождаем ресурсы
    pi.bsc_i2c(0)
    pi.stop()
