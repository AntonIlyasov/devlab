import serial
import time

arduino = serial.Serial(port='/dev/ttyACM0',   baudrate=9600, timeout=.1)
print('sds1')

def write_read(x):
    data = ""
    arduino.write(bytes(x,   'utf-8'))
    time.sleep(0.05)
    while arduino.in_waiting:
        data = arduino.readline().decode('utf-8').rstrip()
    return data


while True:
    num = input("Enter a number: ")
    value = write_read(num)
    print(value)
