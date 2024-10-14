import socket
import time
import struct
import numpy as np
import matplotlib.pyplot as plt

update_freq = 30  # Hz

# Настройки подключения
host = "192.168.4.1"  # IP-адрес ESP32
port = 80  # Порт

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
time_arr = []
vel_enc_arr = []
vel_imu_arr = []

try:
    # Подключение к серверу ESP32
    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    pwm_and_duration = [[200, 4]]
    start_time = time.time()
    count = 0
    elapsed_time = 0

    total_duration = 0
    for pwm, duration in pwm_and_duration:
        total_duration += duration
    print(f"total_duration: {total_duration}")
    
    for pwm, duration in pwm_and_duration:
        start = time.time()
        # Преобразование и отправка сообщения
        message = f"{pwm},{duration}\n"
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")

        # Ожидание получения 8 байт (2 * int32)
        received_data = client_socket.recv(8)
        if len(received_data) == 8:
            response_ints = struct.unpack('ii', received_data)
            print("Received:", response_ints)

            elapsed_time = time.time() - start_time
            count += 1
            print('count: ', count)
            print('Elapsed time: ', elapsed_time)
            vel_enc_arr.append(response_ints[0])
            vel_imu_arr.append(response_ints[1])
            time_arr.append(elapsed_time)

            remaining_time = (1. / update_freq) - (time.time() - start)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")

    # Цикл обработки движений
    while (elapsed_time < total_duration or (response_ints[0] != 0 and response_ints[1] != -1)):
        start = time.time()
        message = "move_request\n"
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")

        received_data = client_socket.recv(8)
        if len(received_data) == 8:
            response_ints = struct.unpack('ii', received_data)
            print("Received:", response_ints)

            elapsed_time = time.time() - start_time
            count += 1
            print('count: ', count)
            print('Elapsed time: ', elapsed_time)
            vel_enc_arr.append(response_ints[0])
            vel_imu_arr.append(response_ints[1])
            time_arr.append(elapsed_time)

            remaining_time = (1. / update_freq) - (time.time() - start)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")

    # Плотинг
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 12))

    ax1.plot(np.array(time_arr), np.array(vel_enc_arr), color='blue', linewidth=3)
    ax1.grid(True)
    ax1.set_title('From encoders')
    ax1.set_xlabel('time (s)')
    ax1.set_ylabel('velocity (mm/s)')

    ax2.plot(np.array(time_arr), np.array(vel_imu_arr), color='red', linewidth=3)
    ax2.grid(True)
    ax2.set_title('From IMU')
    ax2.set_xlabel('time (s)')
    ax2.set_ylabel('velocity (mm/s)')

    plt.subplots_adjust(hspace=0.5)
    plt.show()

finally:
    # Закрытие соединения
    client_socket.close()
    print("Connection closed")