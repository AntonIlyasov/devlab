
import socket
import pickle
import struct
import time





class Sender:


    def __init__(self, telemetry, key):
        self.key = key
        self.telemetry = telemetry
        self.passPhrase = 'loremipsum'
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.port = 9090
        self.socket.bind(('',self.port))
        self.socket.listen(1)
        self.sendConn, self.addr = self.socket.accept()
        print('connected telemetry')
        self.spin()
        
    def spin(self):
        while True:
            self.sendData()
            time.sleep(1/60)
    
        
    def sendData(self):
        telemetry = (f'{self.passPhrase} {self.telemetry["voltage"]};'
        f'{self.telemetry["alt"]};{self.telemetry["vx"]};{self.telemetry["vy"]};{self.telemetry["vz"]};'
        f'{self.telemetry["mode"]} {self.telemetry["pitch"]};{self.telemetry["roll"]};{self.telemetry["yaw"]};'
        f'{float(self.telemetry["heading"])};{self.telemetry["lat"]};{self.telemetry["lon"]} '
        f'{int(self.telemetry["camera"])};{self.telemetry["zoom"]};{self.telemetry["angle_camera"]} {self.telemetry["arm"]}')
        time.sleep(0.2)
        #data = pickle.dumps(telemetry, pickle.HIGHEST_PROTOCOL).hex()
        
        data = self.encode_data(telemetry)
        #print(f'telemetry: {data}')
        data = pickle.dumps(data)
        message = struct.pack('Q', len(data))+data
        self.sendConn.sendall(message)
        #print(f'telemetry: {message}')

    def encode_data(self, encoded):
        data = list(encoded)
        data = data[::-1]
        j= 0
        for i,item in enumerate(data):
            next_pos = ord(item) + ord(self.key[j])
            while next_pos > 126:
                next_pos = 32+(next_pos-126)
            data[i] = chr(next_pos)
            j+=1
            if j == len(self.key):
                j = 0
            if i % 2 != 0:
                temp = data[i-1]
                data[i-1]= data[i]
                data[i] = temp
        return ''.join(data)
        
def initSender(telemetry, key):
    send = Sender(telemetry, key)
