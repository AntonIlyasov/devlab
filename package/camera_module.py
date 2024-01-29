
import socket
import struct
import pickle
import datetime
import numpy as np
import cv2
import time
import multiprocessing as mp

pipeline = "v4l2src device=/dev/video0 ! image/jpeg, width=3840, height=2160, pixel-aspect-ratio=1/1, framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! appsink"


class Camera():
    def zoom_at(self, img,zoom, coord=None ):
            # Translate to zoomed coordinates
            h, w, _ = [zoom * i for i in img.shape ]
            
            if coord is None: cx, cy = w/2, h/2
            else: cx, cy = [zoom*c for c in coord ]
            
            img = cv2.resize( img, (0, 0), fx=zoom, fy=zoom)
            img = img[ int(round(cy - h/zoom * .5)) : int(round(cy + h/zoom * .5)),
                    int(round(cx - w/zoom * .5)) : int(round(cx + w/zoom * .5)),
                    : ]
            
            return img

    def __init__(self, telemetry):
        self.telemetry = telemetry

        video_capture = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
        if video_capture.isOpened():
            try:
                # Window
                while True:
                    start = datetime.datetime.now().timestamp()
                    ret_val, frame = video_capture.read()
                    frame = cv2.resize(frame, (1920, 1080))
                    cv2.imshow("ewqeqweqw", frame)
                    stop = datetime.datetime.now().timestamp()
                    print(1/(stop-start))
                    cv2.waitKey(1)

            finally:
                video_capture.release()
                cv2.destroyAllWindows()
        else:
            print("Error: Unable to open camera")




        videoSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        port = 9000
        videoSocket.bind(('',port))
        print('binded')
        videoSocket.listen(1)
        self.conn, addr = videoSocket.accept()
        time.sleep(2)
            
        print('connected camera')
        
        self.frontal = ''
        self.main = ''
        frontNumber = 0

        for frontNumber in range(8):
            pipeline = f"v4l2src device=/dev/video{frontNumber} ! image/jpeg, width=1920, height=1080, pixel-aspect-ratio=1/1, framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! appsink"
            self.frontal = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
            if self.frontal and self.frontal.isOpened():
                break
            self.frontal.release()

        # mainNumber = frontNumber + 1

        # for mainNumber in range(8):
        #     self.main = cv2.VideoCapture(mainNumber)
        #     if self.main and self.main.isOpened():
        #         break
        #     self.main.release()

        # self.frontal.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M', 'J', 'P', 'G'))
        # self.frontal.set(cv2.CAP_PROP_FPS, 30)
        # self.frontal.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
        # self.frontal.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

        # self.main.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M', 'J', 'P', 'G'))
        # self.main.set(cv2.CAP_PROP_FPS, 30)
        # self.main.set(cv2.CAP_PROP_FRAME_WIDTH, 3840)
        # self.main.set(cv2.CAP_PROP_FRAME_HEIGHT, 2160)
        self.capture()


    


    def capture(self):
        time.sleep(1)
        counter = 0
        while(1):
            #start = datetime.datetime.now()
            #choosenCamera= self.telemetry['camera']    
            zoomNum = self.telemetry['zoom'] if self.telemetry['zoom'] >= 1 else 1
            #self.get_logger().info(f'camera {self.camera}')
            ret,frame = 0,0
            #if choosenCamera:
            #    ret,frame = self.main.read()
            #else:
            ret,frame = self.frontal.read()
        
            if ret:
                #time_start = time.time()
                #print(zoomNum)
                frame = self.zoom_at(frame,zoomNum)
                #print('frame')
                data = cv2.imencode('.jpeg',frame, [int(cv2.IMWRITE_JPEG_QUALITY),95])[1].tostring()
                
                
                package_length = len(data)
                #print(package_length)
                message = struct.pack('Q', package_length) + data
                self.conn.sendall(message)
                cv2.imshow('image', frame)
            #print(f'{datetime.datetime.now()-start}')
            if cv2.waitKey(1) == ord('q'):
                break

def initCamera(telemetry):
    video = Camera(telemetry)

    


