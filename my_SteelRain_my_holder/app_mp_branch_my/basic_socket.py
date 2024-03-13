import socket
import time



class BasicSocket():
    def __init__(self, check, key, ip, port):
        self.connected = False
        self.check = check
        self.key = key
        self.ip = ip
        self.port = port

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect_socket()


    def connect_socket(self):
        while not self.connected:
            try:  
                self.socket.connect((self.ip, self.port)) 
                self.connected = True  
                print(f'connected {self.__class__.__name__}')
            except socket.error:  
                print(f'disconnected {self.__class__.__name__}')
                time.sleep(3)
        self.start()


    def start(self):
        pass



