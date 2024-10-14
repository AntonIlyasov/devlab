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

try:
    plt.ion()
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 12))
    time_arr = []
    vel_enc_arr = []
    vel_imu_arr = []

    # Исходные линии для обновления
    line1, = ax1.plot([], [], color='blue', linewidth=3, label='Encoders')
    line2, = ax2.plot([], [], color='red', linewidth=3, label='IMU')

    ax1.grid(True)
    ax1.set_title('From encoders')
    ax1.set_xlabel('time (s)')
    ax1.set_ylabel('velocity (mm/s)')

    ax2.grid(True)
    ax2.set_title('From IMU')
    ax2.set_xlabel('time (s)')
    ax2.set_ylabel('velocity (mm/s)')

    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    pwm_and_duration = [[0, 50]]
    start_time = time.time()
    count = 0
    elapsed_time = 0

    total_duration = 0
    for pwm, duration in pwm_and_duration:
        total_duration += duration
    print(f"total_duration: {total_duration}")

    for pwm, duration in pwm_and_duration:
        start = time.time()
        message = f"{pwm},{duration}\n"
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")

        received_data = client_socket.recv(8)
        if len(received_data) == 8:
            response_ints = struct.unpack('ii', received_data)
            print("Received:", response_ints)

            elapsed_time = time.time() - start_time
            count += 1
            vel_enc_arr.append(response_ints[0])
            vel_imu_arr.append(response_ints[1])
            time_arr.append(elapsed_time)

            remaining_time = (1. / update_freq) - (time.time() - start)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")

    while elapsed_time < total_duration or (response_ints[0] != 0 and response_ints[1] != -1):
        message = "move_request\n"
        client_socket.sendall(message.encode())
        print(f"Sent: {message}")

        received_data = client_socket.recv(8)
        if len(received_data) == 8:
            response_ints = struct.unpack('ii', received_data)
            print("Received:", response_ints)

            elapsed_time = time.time() - start_time
            count += 1
            vel_enc_arr.append(response_ints[0])
            vel_imu_arr.append(response_ints[1])
            time_arr.append(elapsed_time)

            start = time.time()

            # Обновление данных линий
            line1.set_data(time_arr, vel_enc_arr)
            line2.set_data(time_arr, vel_imu_arr)

            # Пересчет границ для динамического графика
            ax1.set_xlim(0, elapsed_time)
            ax1.set_ylim(min(vel_enc_arr), max(vel_enc_arr))
            ax2.set_xlim(0, elapsed_time)
            ax2.set_ylim(min(vel_imu_arr), max(vel_imu_arr))

            plt.draw()
            plt.pause(0.001)

            print('time.time() - start: ', (time.time() - start)*1000)

            remaining_time = (1. / update_freq) - (time.time() - start)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")
finally:
    client_socket.close()
    print("Connection closed")