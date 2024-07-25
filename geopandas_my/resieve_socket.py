from basic_socket import BasicSocket
import pickle
import struct



class ResieveSocket(BasicSocket):
    def __init__(self, check, key, ip, port, telemetry):
        self.data = b''
        self.check = check
        self.key = key
        self.telemetry = telemetry
        self.payload_size = struct.calcsize('Q')
        super().__init__(check, key, ip, port)
        
        
    def start(self):
        print(f'start {self.__class__.__name__}')
        while True:
            data = self.getData()
            data = self.decode_data(data)
            data = data.split(' ')
            # print(data)
            
            if data[0] == self.check:
                telemetry_from_drone = data[1].split(";")
                self.telemetry['voltage'] = float(telemetry_from_drone[0])
                self.telemetry['alt'] = float(telemetry_from_drone[1])
                self.telemetry['vx'] = float(telemetry_from_drone[2])
                self.telemetry['vy'] = float(telemetry_from_drone[3])
                self.telemetry['vz'] = float(telemetry_from_drone[4])
                self.telemetry['mode'] = telemetry_from_drone[5]

                position_from_drone = data[2].split(";")
                self.telemetry['pitch'] = float(position_from_drone[0])
                self.telemetry['roll'] = float(position_from_drone[1])
                self.telemetry['yaw'] = float(position_from_drone[1])
                self.telemetry['heading'] = float(position_from_drone[3])
                self.telemetry['lat'] = float(position_from_drone[4])
                self.telemetry['lon'] = float(position_from_drone[5])

                camera_info_from_drone = data[3].split(";")
                self.telemetry['camera'] = int(camera_info_from_drone[0])
                self.telemetry['zoom'] = round(float(camera_info_from_drone[1]), 1)
                self.telemetry['angle_camera'] = float(camera_info_from_drone[2])

                self.telemetry['arm'] = int(data[4])
            else:
                pass
            # print(self.telemetry)
    
                    
    def getData(self):
        while len(self.data)< self.payload_size:
            packet = self.socket.recv(8)
            self.data += packet
        packed_msg_size = self.data[:self.payload_size]
        self.data = self.data[self.payload_size:]
        msg_size = struct.unpack("Q", packed_msg_size)[0]
        while len(self.data) < msg_size:
            self.data += self.socket.recv(8)
        key_data = self.data[:msg_size]
        self.data = self.data[msg_size:]
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
  


def init_resieve(check, key, ip, port, telemetry):
    socket = ResieveSocket(check, key, ip, port, telemetry)