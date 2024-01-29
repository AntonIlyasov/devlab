
import math
import socket


import pickle
import struct
from dronekit import connect, VehicleMode,LocationGlobalRelative
import threading
import time
import multiprocessing as mp

import camera_module, keyGen, sender, receiver

def convert(x, in_min=-1, in_max = 1, out_min = 1000, out_max = 2000):
    x= x**3
    return int((x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min)

def convertAngle(x, in_min=-90, in_max = 90, out_min = 500, out_max = 2500):
    return int((x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min)

def convertPPM(x, in_min=500, in_max = 2500, out_min = -90, out_max = 90):
    return int((x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min)

class Controller():
    def __init__(self, telemetry, control):
        self.ready = False
        self.telemetry = telemetry
        self.control = control
        self.telemetry['mode'] = 'LOITER'
        #self.telemetry['mode'] = "ALT_HOLD"
        #self.telemetry['mode'] = 'STABILIZE'  #for tests only
        self.throw = False
        self.attitude = 0
        self.data = b''
        #self.gimbalState = [0,0,0]
        self.passPhrase = 'loremipsum'
        #self.vehicle = connect('127.0.0.1:5760', wait_ready = True)
        self.vehicle = connect('/dev/ttyACM0', wait_ready = True, baud = 115200)
        #print(self.vehicle._master.mode_mapping())
        self.vehicle.mode = VehicleMode(self.telemetry['mode'])
        self.ready = True
        
    def getTelemetry(self):
        self.telemetry['voltage'] = self.vehicle.battery.voltage
        self.telemetry['alt'] = self.vehicle.location.global_frame.alt if self.vehicle.location.global_frame.alt != None else 0
        self.telemetry['vx'] = self.vehicle.velocity[0]
        self.telemetry['vy'] = self.vehicle.velocity[1]
        self.telemetry['vz'] = self.vehicle.velocity[2]
        self.telemetry['mode'] = self.vehicle.mode.name
        self.telemetry['pitch'] = self.vehicle.attitude.pitch
        self.telemetry['roll'] = self.vehicle.attitude.roll
        self.telemetry['yaw'] = self.vehicle.attitude.yaw
        self.telemetry['heading'] = self.vehicle.heading 
        self.telemetry['lat'] = self.vehicle.location.global_frame.lat
        self.telemetry['lon'] = self.vehicle.location.global_frame.lon
        self.telemetry['arm'] = int(self.vehicle.armed)

    
    

    def control_spin(self):
        while True:
            self.getTelemetry()
    #       print(f'{commands}')
            if self.control['arm'] == 'arm':
                #print('arm')
                self.arm_disarm(True)
            elif self.control['arm'] == 'disarm':
                #print('disarm')
                self.arm_disarm(False)
            else:
                # if waypoints[0] == '':
                self.vehicle.mode = VehicleMode(self.telemetry['mode'])
                #print('fly')
                yaw = convert(self.control['yaw'])
                throttle = convert(-self.control['throttle'])
                roll = convert(self.control['roll'])
                pitch = convert(self.control['pitch'])
                self.telemetry['angle_camera'] += 1 * int(self.control['camera_pitch'])
                if self.telemetry['angle_camera'] < -90:
                    self.telemetry['angle_camera'] = -90
                elif self.telemetry['angle_camera'] > 30:
                    self.telemetry['angle_camera'] = 30
                #print(self.currentAngle)
                self.vehicle.channels.overrides={'7':convertAngle(self.telemetry['angle_camera'])}
                self.joystick_control(roll, pitch, throttle, yaw)
                self.telemetry['camera'] = not self.telemetry['camera'] if self.control['camera'] else self.telemetry['camera']
                self.telemetry['zoom'] += 0.1 if self.control['zoom+'] else -0.1 if self.control['zoom-'] else 0
                self.telemetry['zoom'] = 1 if self.telemetry['zoom'] < 1 else self.telemetry['zoom']
                self.joystick_control(roll, pitch, throttle, yaw)
                # else:
                #     self.vehicle.mode = VehicleMode('GUIDED')
                #     self.vehicle.airspeed = 15
                #     points = []
                #     # for i in waypoints:
                #     #     position = list(map(float, i.split(':')))
                #     #     points.append(LocationGlobalRelative(position[0], position[1], height))
                #     # for point in points:
                #     #     self.vehicle.simple_goto(point)
                #     #     self.awaitFinish(point)
        
    def arm_disarm(self, state):
        self.vehicle.armed = state
        
    def joystick_control(self, roll,pitch ,throttle, yaw):
        self.vehicle.channels.overrides = {'1':roll, '2':pitch, '3':throttle, '4':yaw}
    
    
def main():  
    manager = mp.Manager()
    telemetry = manager.dict()
    control = manager.dict()

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
    telemetry['arm'] = ''   
    
    control['arm'] = 'disarm'
    control['throttle'] = 0
    control['yaw'] = 0
    control['pitch'] = 0
    control['roll'] = 0
    control['zoom+'] = 0
    control['zoom-'] = 0
    control['camera'] = False
    control['camera_pitch'] = 0
    control['camera_yaw'] = 0
    control['waypoints'] = []
    key = keyGen.keyGen()
    print('keygen ', key)
    #key = 'key'
    camera_mp = mp.Process(target=camera_module.initCamera, args=(telemetry,))
    camera_mp.start()
    print('started camera')
    controller = Controller(telemetry, control)
    while not controller.ready:
        pass
    send = mp.Process(target = sender.initSender, args = (telemetry,key))
    receive = mp.Process(target = receiver.initReceiver, args = (control, key))
    send.start()
    receive.start()
    controller.control_spin()
    camera_mp.join()
    send.join()
    receive.join()
    
    


if __name__ == '__main__':
    main()
