
import socket
from random import randint, seed
import struct
import pickle
import time

key = ''

class KeyGen():
    def isPrime(self, number):
        arr = [7,11,13,17,19,23,29,31]
        if number < 2:
            return False
        
        if number == 2 or number == 5 or number == 3:
            return True
        
        if number%2 == 0 or number%3 == 0 or number %5 ==0:
            return False
        c = number**0.5
        i=0
        while i<c:
            for j in arr:
                if j>c:
                    break
                if number % (j+i)==0:
                    return False
            i+=30
        return True

    def generatePrimes(self, count):
        primes = [2,3,5,7,11,13,17,19,23,29]
        array = [31,37,41,43,47,49,53,59]
        result = primes+array
        l = len(result)
        while l<count:
            for i,item in enumerate(array):
                if self.isPrime(item):
                    result.append(item)
                array[i]+=30
            l = len(result)
        return result[count-1]



    def getNumber(self):
        data = b''
        payload_size = struct.calcsize('Q')
        while len(data)< payload_size:
            packet = self.conn.recv(1)
            if not packet:
                break
            data += packet
        packed_msg_size = data[:payload_size]
        data = data[payload_size:]
        return struct.unpack("Q", packed_msg_size)[0]
    

    def sendNumber(self, number):
        data = struct.pack('Q',number)
        self.conn.sendall(data)
        pass 


    def initialize(self):
        self.g = self.generatePrimes(randint(200,3000))
        self.p = self.generatePrimes(randint(3000,4000))
        self.sendNumber(self.g)
        self.sendNumber(self.p)
        self.number = randint(10000, 100000)
        self.generatedNumber = (self.g**self.number)%self.p
        self.sendNumber(self.generatedNumber)
        self.receivedNumber = self.getNumber()
        self.K = (self.receivedNumber**self.number)%self.p
        seed(self.K)
        for i in range(128):
            self.key+=chr(randint(32,126))
        time.sleep(3)
        self.conn.close()


    def __init__(self):
        print('started keygen')
        self.workSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.workSocket.bind(('', 10000))
        print('keygen socket')
        self.workSocket.listen(1)
        self.conn, self.addr = self.workSocket.accept()
        print('connected')
        self.g = 0
        self.p = 0
        self.K = 0
        self.key = ''
        self.initialize()


def keyGen():
    keygen = KeyGen()
    return keygen.key

