import math
import socket

import socket

import pickle
import struct
from dronekit import connect, VehicleMode,LocationGlobalRelative
import threading
import time





class Receiver:
    
    def __init__(self, control, key):
        
        self.data = b''
        self.payload_size = struct.calcsize('Q')
        
        self.passPhrase = 'loremipsum'

        self.control = control
        self.key = key
        self.Receivesocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.Receivesocket.bind(('',8080))
        self.Receivesocket.listen(1)
        self.conn, self.addr = self.Receivesocket.accept()
        self.receiveEvent()
        #print(self.vehicle._master.mode_mapping())

        
    def receiveEvent(self):
        while True:
            try: 
                data = self.getData()
                #print('received ',data)
                data = self.decode_data(data)
                # print(data)
                data = data.split()
                if data[0] == self.passPhrase:
                    #print('validated')
                    # print(data[1])
                    joystick = data[1].split(';')
                    waypoints = ['']
                    if joystick[0] == 'arm' or joystick[0] == 'disarm':
                        self.control['arm'] = joystick[0]
                    else:
                        self.control['arm'] = 'await'
                        self.control['throttle'] = float(joystick[1])
                        self.control['yaw'] = float(joystick[0])
                        self.control['pitch']= float(joystick[3])
                        self.control['roll'] = float(joystick[2])
                        self.control['camera'] = int(joystick[6])
                        self.control['zoom+'] = int(joystick[8])
                        self.control['zoom-'] = int(joystick[7])
                        self.control['camera_pitch'] = int(joystick[5])
                else:
                    pass
            except Exception as error:
                print(error)
                self.control['mode'] = 'RTL'
            
    

    



    def getData(self):
        data = self.data
        
        while len(data)< self.payload_size:
            packet = self.conn.recv(8)
            if not packet:
                break
            data += packet
        packed_msg_size = data[:self.payload_size]
        data = data[self.payload_size:]
        msg_size = struct.unpack("Q", packed_msg_size)[0]
        while len(data) < msg_size:
            data += self.conn.recv(1024)
        key_data = data[:msg_size]
        self.data = data[msg_size:]
        return pickle.loads(key_data)

    def decode_data(self, encoded):
        j = 0
        data = list(encoded)
        for i,item in enumerate(data):
            if i%2 == 0 and i<len(data)-1:
                temp = data[i+1]
                data[i+1]= data[i]
                data[i] = temp
        for i,item in enumerate(data):
            next_pos = ord(item) - ord(self.key[j])
            while next_pos <32:
                next_pos = -32+(next_pos+126)
            data[i] = chr(next_pos)
            j+=1
            if j == len(self.key):
                j = 0
        return ''.join(data)[::-1]
    
   

    

        
   

def initReceiver(control, key):
    receiver = Receiver(control, key)


    
