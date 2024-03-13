import socket
from random import randint, seed
import time

class KeyGen():
    def __init__(self, ip, port):
        print('__init__')
        self.connected = False 
        self.ip = ip
        self.port = port 
        
        self.socked = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while not self.connected:
            try:
                self.socked.connect((self.ip, self.port))               # blocked
                self.connected = True
                print(f'connected {self.__class__.__name__}')
            except socket.error:
                print(f'disconnected {self.__class__.__name__}')
                time.sleep(1)
        self.g = 0
        self.p = 0
        self.K = 0
        self.key = ''

    def initialize(self):
        self.g = self.getNumber()
        self.p = self.getNumber()

        self.number = randint(10000, 100000)
        self.receivedNumber = self.getNumber()
        self.generatedNumber = (self.g**self.number)%self.p
        # print(self.generatedNumber)

        self.sendNumber(self.generatedNumber)
        self.K = (self.receivedNumber**self.number)%self.p
        print(self.K)

        seed(self.K)

        for i in range(128):
            self.key+=chr(randint(32,126))

        return self.key


        
        




    def getNumber():
        pass


    def sendNumber(self, number):
        pass

        
            
                
                