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
    # !!! Включить интерактивный режим для анимации
    plt.ion()

    time_arr = []
    vel_enc_arr = []
    vel_imu_arr = []

    # Исходные линии для обновления

    client_socket.connect((host, port))
    print(f"Connected to {host}:{port}")

    pwm_and_duration = [[150, 2],[255, 2],[150, 2],
                        [-150, 2],[-255, 2],[-150, 2]]*2
    # pwm_and_duration = [[0, 1],[-255, 3],[-100, 1]]

    # pwm_and_duration = [[0, 10]]
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
            if (-1210 < response_ints[0] < 1210):
                vel_enc_arr.append(response_ints[0])
                vel_imu_arr.append(response_ints[1])
                time_arr.append(elapsed_time)

            # Ограничение на последние 100 точек
            if len(time_arr) > 10000:
                time_arr = time_arr[-10000:]
                vel_enc_arr = vel_enc_arr[-10000:]
                vel_imu_arr = vel_imu_arr[-10000:]

            # !!! Очистить текущую фигуру
            plt.clf()  

            # Отобразить график
            plt.plot(np.array(time_arr), np.array(vel_enc_arr), color='blue', linewidth=3)
            plt.plot(np.array(time_arr), np.array(vel_imu_arr), color='red', linewidth=3)
            plt.grid(True)
            plt.draw()
            plt.gcf().canvas.flush_events()
            end = time.time()
            print('end - start: ', end - start)

            remaining_time = (1. / update_freq) - (time.time() - start)
            print('remaining_time: ', remaining_time)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")

    while elapsed_time < total_duration or (response_ints[0] != 0 and response_ints[1] != -1):
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
            if (-1210 < response_ints[0] < 1210):
                vel_enc_arr.append(response_ints[0])
                vel_imu_arr.append(response_ints[1])
                time_arr.append(elapsed_time)

            # Ограничение на последние 100 точек
            if len(time_arr) > 10000:
                time_arr = time_arr[-10000:]
                vel_enc_arr = vel_enc_arr[-10000:]
                vel_imu_arr = vel_imu_arr[-10000:]

            # !!! Очистить текущую фигуру
            plt.clf()  

            # Отобразить график
            plt.plot(np.array(time_arr), np.array(vel_enc_arr), color='blue', linewidth=3)
            plt.plot(np.array(time_arr), np.array(vel_imu_arr), color='red', linewidth=3)
            plt.grid(True)
            plt.draw()
            plt.gcf().canvas.flush_events()
            end = time.time()
            print('end - start: ', end - start)

            remaining_time = (1. / update_freq) - (time.time() - start)
            print('remaining_time: ', remaining_time)
            if remaining_time > 0:
                time.sleep(remaining_time)
        else:
            print("Warning: received incomplete data. Skipping this cycle.")
    # Отключить интерактивный режим по завершению анимации
    plt.ioff()

    # Нужно, чтобы график не закрывался после завершения анимации
    plt.show()
finally:
    client_socket.close()
    print("Connection closed")