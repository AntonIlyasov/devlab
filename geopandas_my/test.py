import socket 
import pickle
import time
import struct

class Test():
    
    def __init__(self):
        self.socket = socket.socket()
        self.payload_size = 4
        self.data = b''
        self.ip = '192.168.0.20'
        self.port = 11000
        self.connected = False
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect_socket()
        print('connected')
        self.spin()

    def connect_socket(self):
        while not self.connected:
            try:  
                self.socket.connect((self.ip, self.port)) 
                self.connected = True  
                print(f'connected {self.__class__.__name__}')
            except socket.error:  
                print(f'disconnected {self.__class__.__name__}')
                time.sleep(3)
        
    def getData(self):
        while len(self.data) < self.payload_size:
            packet = self.socket.recv(8)
            self.data += packet
            if not packet:break
        packed_msg_size = self.data[:self.payload_size]
        self.data = self.data[self.payload_size:]
        msg_size = struct.unpack("l", packed_msg_size)[0]
        return msg_size
        # while len(self.data) < msg_size:
        #     self.data += self.socket.recv(8)
        # key_data = self.data[:msg_size]
        # self.data = self.data[msg_size:]
        # return pickle.loads(key_data)

    def spin(self):
        self.socket.send('hui'.encode('ASCII'))
        while True:
            print(self.getData())
            
        
if __name__ == '__main__':
    test = Test()