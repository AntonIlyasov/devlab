import socket
import random
import time
import struct

# Настройки подключения
host = "192.168.4.1"  # IP-адрес ESP32 (можно узнать через WiFi.softAPIP())
port = 80             # Порт, на который будет отправлено сообщение

# Создаем TCP-сокет
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Подключаемся к серверу ESP32
    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    pwm_and_duration = [[100,1],[150,1],[200,1],[150,1],[100,1]]

    for item in pwm_and_duration:
        # Преобразуем массив в строку для отправки
        message = f"{pwm_and_duration[item][0]},{pwm_and_duration[item][1]}"
        # Отправляем сообщение серверу
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")
        # Ожидание получения двух целых чисел (2 * int32)
        received_data = client_socket.recv(8)  # 8 байт для 2 int32
        response_ints = struct.unpack('ii', received_data)
        print("Received:", response_ints)

    message = "move_request"
    client_socket.sendall(message.encode())
    print(f"Sent: {message}")
    # Ожидание получения двух целых чисел (2 * int32)
    received_data = client_socket.recv(8)  # 8 байт для 2 int32
    response_ints = struct.unpack('ii', received_data)
    print("Received:", response_ints)

finally:
    # Закрываем соединение
    client_socket.close()
    print("Connection closed")