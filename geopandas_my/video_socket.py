from basic_socket import BasicSocket
import struct

video_frame = ''

class VideoSocket(BasicSocket):
    def __init__(self, check, key, ip, port, video_dict, detection_queue):
        self.video_dict = video_dict
        self.detection_queue = detection_queue

        self.data = b''
        self.payload_size = struct.calcsize('Q')

        super().__init__(check, key, ip, port)
        

    def start(self):
        print(f'start {self.__class__.__name__}')
        counter = 0
        while True:
            frame_data=self.getData()
            if frame_data:
                if counter == 0:
                    self.detection_queue.put(frame_data)
                counter +=1
                if counter == 3:
                    counter = 0
                
                self.video_dict['video'] = frame_data


    def getData(self):
        while len(self.data)< self.payload_size:
            packet = self.socket.recv(1024*2*1024)
            if not packet:
                break
            self.data += packet

        packed_msg_size = self.data[:self.payload_size]
        self.data = self.data[self.payload_size:]
        msg_size = struct.unpack("Q", packed_msg_size)[0]

        while len(self.data) < msg_size:
            self.data += self.socket.recv(1024)

        frame_data = self.data[:msg_size]
        self.data = self.data[msg_size:]

        return frame_data



def init_video(check, key, ip, port, video_dict, detection_queue):
    socket = VideoSocket(check, key, ip, port, video_dict, detection_queue)
