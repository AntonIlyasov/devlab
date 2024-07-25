import cv2
import numpy as np
import math
import datetime



def init_gui(video_dict, targets_queue, telemetry):
    gui = GUI(video_dict, targets_queue, telemetry)



class GUI():
    def __init__(self, video_dict, targets_queue, telemetry):
        self.video_dict = video_dict
        self.targets_queue = targets_queue
        self.telemetry = telemetry

        self.detectedTargets = []
        self.video_width = 1280
        self.video_height = 720
        
        # self.video_width = 1920
        # self.video_height = 1080

        # self.video_width = 3840
        # self.video_height = 2160

        # self.video_width = 2592
        # self.video_height = 1944

        self.center_width = self.video_width // 2
        self.center_height = self.video_height // 2

        self.center_compas = (self.video_width-150, self.video_height-125)
        self.radius_compas = 100

        self.cross_size = 20
        self.color = (0, 130, 109)

        self. horizon_thickness = 2  # Толщина линии горизонта
        self.line_length = 230
        self.gap = 120
        
        self.font = cv2.FONT_HERSHEY_COMPLEX
        self.font_scale = 1
        self.font_thickness = 2

        self.start_recording = False
        self.record = False
        self.outvideo = ''

       
        
        self.frame_data = np.zeros((self.video_height, self.video_width, 4), dtype=np.uint8)
        self.permanent_frame_data =np.zeros((self.video_height, self.video_width, 4), dtype=np.uint8)
        self.alpha = np.ones((self.video_height, self.video_width), dtype=np.uint8) 
        # self.img = cv2.imread('test.jpeg')
        # self.permanent_frame_data[:, :] = (0, 0, 0, 0)
        self.draw_permanent()
        # self.video_dict["video"] = self.img
        self.video()
    
    #callbacks===================================================
    # def targetsCollector(self, data):
    #     self.detectedTargets = pickle.loads(bytes.fromhex(data.data))



    def video(self):
        print("gui")
        while True:

            self.frame_data = self.video_dict['video']
            # if not self.video_queue.empty():
            # self.frame_data = self.video_queue.get()
            if not self.frame_data is None:
                buffer = np.fromstring(self.frame_data, np.uint8)
                self.frame_data = cv2.imdecode(buffer, cv2.IMREAD_COLOR)

                if not self.targets_queue.empty():
                    self.detectedTargets = self.targets_queue.get()

                # print(f"telem: {self.telemetry['arm']}, code: {self.start_recording}")
                
                if self.start_recording != self.telemetry["arm"]:
                    self.start_recording = self.telemetry["arm"]
                    self.record_call()

                
                
                self.draw()
                if self.record: 
                    self.outvideo.write(self.frame_data[:, :, :3])
                
                cv2.imshow('oracle', self.frame_data)
                cv2.waitKey(1)


    def record_call(self):
        if self.start_recording:
             # Инициализировать объект записи видео
            datetime_str = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            # fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
            fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
            self.outvideo = cv2.VideoWriter(f'{datetime_str}.mp4', fourcc, 20, (self.video_width, self.video_height))
            # self.outvideo = cv2.VideoWriter(f'/home/bulat/{datetime_str}.avi', fourcc, 30.0, self.video_width, self.video_height), True)
            self.record = True
        else:
            self.record = False
            self.outvideo.release()
        
    
    #draw========================================================   
    def draw_permanent(self):
        self.draw_cross()
        # self.draw_permanent_compas()
        self.draw_permanent_horizon()

    def draw(self):
        self.draw_neiral_rectangle()
        # self.video_width = self.frame_data.shape[1]
        # self.video_height = self.frame_data.shape[0]
        self.frame_data = cv2.resize(self.frame_data, dsize=(self.video_width, self.video_height))
        
        self.print_telemetry()
        self.draw_horizont()
        # self.draw_compas()
        self.frame_data = cv2.merge((self.frame_data, self.alpha))
        # self.frame_data = self.frame_data[:, :, :4]
        # print(f"self.frame_data - {self.frame_data.shape}| self.permanent_frame_data {self.permanent_frame_data.shape}")
        
        self.frame_data = cv2.add(self.permanent_frame_data, self.frame_data)
        # self.frame_data = cv2.resize(self.frame_data, (1920,1080))
    

    def print_telemetry(self):
        # Определяем начальные координаты 
        org = [30, 30,]
        height = 50
        
        # for name, value in self.telemetry:
        #     text = f"{name} {value}"
        #     cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        #     org[1]+=height
        # Отображаем каждую надпись в столбике
        text = f"Напряжение {self.telemetry['voltage']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height
        

        text = f"Высота {self.telemetry['alt']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Скорость {round(((self.telemetry['vx']**2+self.telemetry['vy']**2)**0.5), 1)}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Скорость_Z {self.telemetry['vz']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Камера {self.telemetry['camera']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height
        
        text = f"Зум {self.telemetry['zoom']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height
        
        text = f"Режим {self.telemetry['mode']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"ARM {self.telemetry['arm']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Lat {self.telemetry['lat']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Lon {self.telemetry['lon']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

        text = f"Heading {self.telemetry['heading']}"
        cv2.putText(self.frame_data, text, org, self.font, self.font_scale, self.color, self.font_thickness, cv2.LINE_AA)
        org[1]+=height

    
    def draw_cross(self):
        # Рисование вертикальной линии прицела
        cv2.line(self.permanent_frame_data, (self.center_width, self.center_height - self.cross_size), (self.center_width, self.center_height + self.cross_size), self.color, 2)
        # Рисование горизонтальной линии прицела
        cv2.line(self.permanent_frame_data, (self.center_width - self.cross_size, self.center_height), (self.center_width + self.cross_size, self.center_height), self.color, 2)

    def draw_neiral_rectangle(self):
        for i in self.detectedTargets:
            cv2.rectangle(self.frame_data, i[0], i[1], (0,255,0), 2)
            cv2.putText(self.frame_data, str(i[3]), i[0], cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2) 
            cv2.putText(self.frame_data, f'{i[-1][0]}, {i[-1][1]}',i[1],cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)
    def draw_permanent_horizon(self):
        permanent_r = [[0, math.pi/6],
                     [0, -math.pi/6],]
        permanent_p =[[math.pi/12, 0],
                     [-math.pi/12, 0]]
        for p, r in permanent_r:
            line1_s, line1_e, line2_s, line2_e = self.coord_lines(p, r, self.line_length+10, 20)
            cv2.line(self.permanent_frame_data, line1_s, line1_e, self.color, thickness=self.horizon_thickness)
            cv2.line(self.permanent_frame_data, line2_s, line2_e, self.color, thickness=self.horizon_thickness)    
        
        for p, r in permanent_p:
            line1_s, line1_e, line2_s, line2_e = self.coord_lines(p, r, self.line_length-25, 40)
            cv2.line(self.permanent_frame_data, line1_s, line1_e, self.color, thickness=self.horizon_thickness)
            cv2.line(self.permanent_frame_data, line2_s, line2_e, self.color, thickness=self.horizon_thickness)  

    def draw_horizont(self):
        
        pitch_angle = float(self.telemetry["pitch"])
        roll_angle = float(self.telemetry["roll"])
        
        line1_s, line1_e, line2_s, line2_e = self.coord_lines(pitch_angle, roll_angle, self.line_length, self.gap)
        cv2.line(self.frame_data, line1_s, line1_e, self.color, thickness=self.horizon_thickness)
        cv2.line(self.frame_data, line2_s, line2_e, self.color, thickness=self.horizon_thickness)

    def coord_lines(self, pitch_angle, roll_angle, line_length, gap):

        pitch_angle = -pitch_angle
        pitch_pixel_shift = int(self.center_height * np.tan(pitch_angle)) # Расчет углов наклона для отображения
        shifted_horizon_y = self.center_height + pitch_pixel_shift # Сдвиг горизонта на основе угла наклона
        roll_angle = math.pi + roll_angle  # Угол крена самолета в градусах

        ready_cos = np.cos(roll_angle)
        ready_sin = np.sin(roll_angle)

        line1_start_x = int(self.center_width + line_length * ready_cos)
        line1_start_y = int(shifted_horizon_y + line_length * ready_sin)
        line1_end_x = int(line1_start_x - gap*ready_cos)
        line1_end_y = int(line1_start_y - gap*ready_sin)
        line2_start_x = int(self.center_width - line_length * ready_cos)
        line2_start_y = int(shifted_horizon_y - line_length * ready_sin)
        line2_end_x = int(line2_start_x + gap*ready_cos)
        line2_end_y = int(line2_start_y + gap*ready_sin)
        return (line1_start_x, line1_start_y), (line1_end_x, line1_end_y), (line2_start_x, line2_start_y), (line2_end_x, line2_end_y)

  
