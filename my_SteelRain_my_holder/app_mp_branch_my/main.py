import multiprocessing as mp 
from keyGen import KeyGen
from send_socket import init_send
import time

def main():
    IP = "10.42.0.1"
    GROUND_IP = "10.42.0.20"
    CHECK = 'loremipsum'
    PORT_SEND = 8080
    PORT_RESIEVE = 9090
    PORT_VIDEO = 9000
    PORT_KEYGEN = 10000
    PORT_GROUND = 11000

    # print("keygen start")
    keygen = KeyGen(IP, PORT_KEYGEN)
    KEY = keygen.initialize()
    print(KEY)
    # print("keygen stop")
    time.sleep(15)

    manager = mp.Manager()
    telemetry =manager.dict()
    telemetry['voltage'] = 0
    telemetry['alt'] = 0
    telemetry['vx'] = 0 
    telemetry['vy'] = 0
    telemetry['vz'] = 0
    telemetry['mode'] = 0
    telemetry['pitch'] = 0
    telemetry['roll'] = 0
    telemetry['yaw'] = 0
    telemetry['heading'] = 0
    telemetry['lat'] = 0
    telemetry['lon'] = 0
    telemetry['camera'] = 0
    telemetry['zoom'] = 1
    telemetry['angle_camera'] = 0
    telemetry['arm'] = 0
    telemetry['initialization'] = True
    telemetry['joystick'] = {}

    video_dict = manager.dict()
    video_dict['video'] = None

    detection_queue = mp.Queue()
    targets_queue = mp.Queue()

    send_p = mp.Process(target=init_send, args=(CHECK, KEY, IP, PORT_SEND, telemetry,))


    


    

if __name__ == '__main__':
    main()