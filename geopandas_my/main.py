import multiprocessing as mp 
from resieve_socket import init_resieve
from send_socket import init_send
from video_socket import init_video
from detection import init_detection
from gui import init_gui
from ground_socket import init_ground
from keyGen import KeyGen
import time



def main():
    # IP = "192.168.1.145"
    IP = "192.168.12.1"
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
    resieve_p = mp.Process(target=init_resieve, args=(CHECK, KEY, IP, PORT_RESIEVE, telemetry,))
    video_p = mp.Process(target=init_video, args=(CHECK, KEY, IP, PORT_VIDEO, video_dict, detection_queue,))
    detector_p = mp.Process(target=init_detection, args=(detection_queue, targets_queue, telemetry,))
    gui_p = mp.Process(target=init_gui, args=(video_dict, targets_queue, telemetry,))
    # ground_p = mp.Process(target=init_ground, args=(CHECK, KEY, GROUND_IP, PORT_GROUND, telemetry,))

    print("start")
    send_p.start()
    resieve_p.start()
    video_p.start()
    detector_p.start()
    gui_p.start()
    # ground_p.start()
    
    send_p.join()
    resieve_p.join()
    video_p.join()
    detector_p.join()
    gui_p.join()
    # ground_p.join()
    print("join")

if __name__ == '__main__':
    main()