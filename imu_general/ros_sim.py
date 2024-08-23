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

    while True:
        # Генерация случайного массива из двух целых чисел
        # random_numbers = [random.randint(0, 100), random.randint(0, 100)]
        random_numbers = [-1, 2]

        # Преобразуем массив в строку для отправки
        message = f"{random_numbers[0]},{random_numbers[1]}"
        
        # Отправляем сообщение серверу
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")

        # Ожидание получения четырех целых чисел (4 * int32)
        received_data = client_socket.recv(16)  # 16 байт для 4 int32
        response_ints = struct.unpack('iiii', received_data)
        print("Received:", response_ints)

        # Задержка перед отправкой следующего сообщения
        time.sleep(1)

finally:
    # Закрываем соединение
    client_socket.close()
    print("Connection closed")
