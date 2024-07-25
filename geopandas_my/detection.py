from ultralytics import YOLO
from deep_sort_realtime.deepsort_tracker import DeepSort
import math
import cv2
import numpy as np



class Detector():

    def __init__(self, detection_queue,targets_queue, telemetry):
        self.model = YOLO("yolov8n.pt")
        self.telemetry = telemetry
        self.targets_queue = targets_queue
        self.pixelSize = 2.2 * 10 ** (-6)
        self.width = 2592
        self.latLength = 111000
        self.zoom = 1
        self.lon = 0
        self.detection_queue = detection_queue
        self.lat = 0
        self.alt = 0
        self.pitch = 0
        self.height = 1944
        self.xcenter = 1296
        self.ycenter = 972
        self.tracker = DeepSort(max_age = 10)
        self.CONFIDENCE_THRESHOLD = 0.65
        self.detectionFrame = []
        self.flag = False
        self.detection()


    def detection(self):
        while True:
            if not self.detection_queue.empty():
                # print("frame")
                frame = self.detection_queue.get()

                buffer = np.fromstring(frame, np.uint8)
                frame = cv2.imdecode(buffer, cv2.IMREAD_COLOR)

                detections = self.model(frame, verbose=False)[0] 
                detected = detections.boxes.data.tolist()
                # self.get_logger().info(f'frame')
                results = []
                for data in detected:
                    if data[4] < self.CONFIDENCE_THRESHOLD:
                        continue
                    xmin,ymin, xmax, ymax, class_id = int(data[0]), int(data[1]), int(data[2]), int(data[3]), int(data[5])
                    results.append([[xmin,ymin, xmax-xmin, ymax-ymin], data[4], class_id])
                tracks = self.tracker.update_tracks(results, frame = frame)
                detectionResult = []
                for track in tracks:
                    if not track.is_confirmed():
                        # self.get_logger().info(f'не трек')
                        continue
                    
                    track_id = track.track_id
                    ltrb = track.to_ltrb()
                    xmin, ymin, xmax, ymax = int(ltrb[0]), int(ltrb[1]), int(ltrb[2]), int(ltrb[3])
                    xcenter = xmin + (xmax-xmin)/2
                    ycenter = ymin + (ymax-ymin)/2
                    x,y = self.calculatePosition([xcenter,ycenter])
                    detectionResult.append([[xmin,ymin],[xmax,ymax], [xcenter,ycenter], track_id, [x,y]])
                # print(detectionResult)
                self.targets_queue.put(detectionResult)


    def degToRad(self, deg):
        return math.pi * (deg/180)


    def calculateLength(self, lon):
        return 6378.1*1000*(math.pi/180)*math.cos(lon)
    

    def calculatePosition(self, coordsArray):
        alt = self.telemetry['alt']
        zoom = self.telemetry['zoom']
        heading = self.telemetry['yaw']
        pitch = self.telemetry['pitch'] + self.telemetry['angle_camera']
        lon = self.telemetry['lon']
        lat = self.telemetry['lat']
        focal = 0.0023*zoom
        x = (coordsArray[0] - self.xcenter)*self.pixelSize
        y = (coordsArray[1] - self.ycenter)*self.pixelSize
        distance = (x**2 + y**2)**0.5
        dB = math.asin(x/distance)
        dA = math.atan(distance/focal)
        #A = pitch - dA
        B = heading + dB 
        dx = alt * math.sin(B)
        dy = alt*math.cos(B)
        x = (lon * self.calculateLength(lon) + dx)/self.calculateLength(lon)
        y = (lat * self.latLength + dy) / self.latLength
        return [x,y]



def init_detection(detection_queue,targets_queue, telemetry):
    detector = Detector(detection_queue,targets_queue, telemetry)