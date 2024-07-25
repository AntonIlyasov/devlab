from basic_socket import BasicSocket
import time
import struct
import math
import numpy as np

PORT_GROUND = 11000
GROUND_IP = "192.168.0.20"
CHECK = 'loremipsum'



class GroundSocket(BasicSocket):
    def __init__(self, check, key, ip, port, telemetry):
        self.telemetry = telemetry
        self.stationLat = 0
        self.stationLon = 0
        self.stationAlt = 0
        self.stationHeading = 0
        self.latLenght = 111300
        self.lonLength = 0
        self.EarthRadius = 6378100 
        self.data = b''
        self.payload_size = 4
        super().__init__(check, key, ip, port)
        # print('connected')
        
    
    def calculate_length(self):
        self.lonLength = (np.pi/180)*self.EarthRadius*np.cos(np.deg2rad(self.stationLat))
    
    
    def calculate_angles(self):
        dlat = (self.telemetry['lat'] - self.stationLat)*self.latLenght
        dlon = (self.telemetry['lon'] - self.stationLon)*self.lonLength
        dalt = self.telemetry['alt'] - self.stationAlt
        dist_hor = (dlat**2 + dlon **2)**0.5
        dist = (dlat**2 + dlon **2 + dalt**2)**0.5

        vert_angle = int(115 + np.rad2deg(np.arcsin(dalt/dist)))
        hor_angle = int(self.stationHeading + np.rad2deg(np.arctan2(dlat,dlon)))
        hor_angle += 360 if hor_angle < -90 else 0
        return vert_angle, hor_angle
        
    # def getNumber(self):
    #     while len(self.data) < self.payload_size:
    #         packet = self.socket.recv(12)
    #         self.data += packet
    #         if not packet:break
    #     packed_msg_size = self.data[:self.payload_size]
    #     self.data = self.data[self.payload_size:]
    #     return struct.unpack("l", packed_msg_size)[0]
        
    
    def start(self):
        print(f'start {self.__class__.__name__}')
        self.socket.send('a'.encode('ASCII'))
        data_str = self.socket.recv(1024).decode('ASCII')
        station_data = list(map(int, data_str.split()))
        print(station_data)
        self.stationLat = station_data[0]/1_000_000
        self.stationLon = station_data[1]/1_000_000
        self.stationHeading = station_data[2]
        while not (self.telemetry['lat'] and self.telemetry['lon'] and self.telemetry['alt']):
            pass
        self.stationAlt = self.telemetry['alt']+1.6
        print(self.stationAlt)
        self.calculate_length()
        while True:
            vert_angle, hor_angle = self.calculate_angles()
            data = f' {vert_angle} {hor_angle}'
            #print(data)
            data = data.encode('ASCII')
            self.socket.sendall(data)
            time.sleep(1/20)
            
            
def init_ground(check, key, ip, port, telemetry):
    socket = GroundSocket(check, key, ip, port, telemetry)
    
    
def main():
    global CHECK, GROUND_IP, PORT_GROUND
    import multiprocessing as mp
    manager = mp.Manager()
    telemetry = manager.dict()
    telemetry['lat'] = 60.028574
    telemetry['lon'] = 30.257217
    telemetry['alt'] = 20
    ground_p = mp.Process(target=init_ground, args=(CHECK, 'KEY', GROUND_IP, PORT_GROUND, telemetry,))
    ground_p.start()
    while True:
        telemetry['lat'], telemetry['lon'], telemetry['alt'] = list(map(float, input().split()))
    ground_p.join()
    
    
if __name__ == '__main__':
    main()
    
    
    
# 60.028674 30.257217 20\
    
# 60.028628 30.257102 21.6