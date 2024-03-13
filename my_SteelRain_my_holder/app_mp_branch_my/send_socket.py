from basic_socket import BasicSocket
import pygame

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

