from basic_socket import BasicSocket
import pickle
import struct
import pygame
import time

class SendSocket(BasicSocket):
    def __init__(self, check, key, ip, port, telemetry):
        self.telemetry = telemetry
        self.check = check
        self.key = key
        pygame.init()
        self.joysticks={}
        self.controll_dict={}
        self.armed = False
        while self.telemetry['initialization']:
            if joy := self.joy():
                self.telemetry['joystick'] = joy
        super().__init__(check, key, ip, port)
        

    def start(self):
        print(f'start {self.__class__.__name__}')
        while True:
            # self.armed = self.telemetry["arm"]
            time.sleep(1/61)
            if not self.armed:
                data = f"{self.check} disarm"
                # print(data)
                self.sendData(data)
            joy = self.joy()
            if joy:
                if self.armed and joy['b9'] == '0' and joy['b10'] == '1':
                    self.armed = False
                    time.sleep(0.1)
                elif not self.armed and joy['b9'] == '1' and joy['b10'] == '1':
                    data = f"{self.check} arm"
                    self.sendData(data)
                    print(data)
                    self.armed = True
                    time.sleep(0.1)
                elif self.armed and joy['b10'] == '0':
                    data = f"{self.check} {joy['a0']};{joy['a1']};{joy['a2']};{joy['a3']};{joy['h0']};{joy['b2']};{joy['b3']};{joy['b1']};{joy['b0']}"
                    #print(data)
                    self.sendData(data)
                    #{'a0': '0', 'a1': '0', 'a2': '0', 'a3': '0', 'b0': '0', 'b1': '0', 'b2': '0', 'b3': '0', 'b4': '0', 'b5': '0', 'b6': '0', 'b7': '0', 'b8': '0', 'b9': '0', 'b10': '0', 'b11': '0', 'b12': '0', 'b13': '0', 'h0': '0;0'}
  

    def sendData(self, joy):
        data = joy
        data = self.encode_data(joy)
        data = pickle.dumps(data)
        message = struct.pack('Q', len(data))+data
        self.socket.sendall(message)
        

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
    

    def joy(self):
        '''слушает джойстик и формирует данные'''
        for event in pygame.event.get():
            if event.type == pygame.JOYDEVICEADDED:
                joy = pygame.joystick.Joystick(event.device_index)
                self.joysticks[joy.get_instance_id()] = joy
            elif event.type == pygame.JOYDEVICEREMOVED:
                del self.joysticks[event.instance_id]

        for joystick in self.joysticks.values():
            axes = joystick.get_numaxes()
            for i in range(axes):
                axis = joystick.get_axis(i)
                if axis >-0.040 and axis <0.040:
                    axis = 0 
                self.controll_dict[f'a{i}']= str(round(axis, 3))

            buttons = joystick.get_numbuttons()
            for i in range(buttons):
                button = joystick.get_button(i)
                self.controll_dict[f'b{i}']= str(round(button, 3))

            hats = joystick.get_numhats()
            for i in range(hats):
                hat = joystick.get_hat(i)
                self.controll_dict[f'h{i}']= f'{str(hat[0])};{str(hat[1])}'
        print(str(self.controll_dict))#--------------------------------------------------------
        return self.controll_dict



def init_send(check, key, ip, port, telemetry):
    socket = SendSocket(check, key, ip, port, telemetry)
