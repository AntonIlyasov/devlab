#!/usr/bin/env python3
#
#  USB Camera - Simple
#
#  Copyright (C) 2021-22 JetsonHacks (info@jetsonhacks.com)
#
#  MIT License
#

import sys
import datetime
import cv2

window_title = "USB Camera"

# ASSIGN CAMERA ADRESS to DEVICE HERE!
pipeline = " ! ".join(["v4l2src device=/dev/video1",
                       "image/jpeg,width=3840,height=2160,pixel-aspect-ratio=1/1,framerate=30/1",
                       "videoconvert",
                       "appsink"
                       ])

pipeline = "v4l2src device=/dev/video0 ! image/jpeg, width=3840, height=2160, pixel-aspect-ratio=1/1, framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! appsink"


# width=(int)2592, height=(int)1944
def show_camera():

    # Full list of Video Capture APIs (video backends): https://docs.opencv.org/3.4/d4/d15/group__videoio__flags__base.html
    # For webcams, we use V4L2
    video_capture = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
    # video_capture = cv2.VideoCapture(0, cv2.CAP_V4L2)
    # video_capture.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M', 'J', 'P', 'G'))
    # video_capture.set(cv2.CAP_PROP_FPS, 30)
    # video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 3840)
    # video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 2160)
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


if __name__ == "__main__":

    show_camera()





























    # v4l2 gstv4l2object.c:4541:gst_v4l2_object_probe_caps:<v4l2src0:src> probed caps: video/x-raw, format=(string)YUY2, width=(int)3840, height=(int)2160, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)1/1; video/x-raw, format=(string)YUY2, width=(int)2592, height=(int)1944, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)1/1; video/x-raw, format=(string)YUY2, width=(int)2048, height=(int)1536, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)3/1; video/x-raw, format=(string)YUY2, width=(int)1920, height=(int)1080, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)5/1; video/x-raw, format=(string)YUY2, width=(int)1600, height=(int)1200, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)5/1; video/x-raw, format=(string)YUY2, width=(int)1280, height=(int)960, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)5/1; video/x-raw, format=(string)YUY2, width=(int)1280, height=(int)720, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 10/1, 5/1 }; video/x-raw, format=(string)YUY2, width=(int)800, height=(int)600, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 20/1, 15/1, 10/1, 5/1 }; video/x-raw, format=(string)YUY2, width=(int)640, height=(int)480, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; video/x-raw, format=(string)YUY2, width=(int)320, height=(int)240, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)3840, height=(int)2160, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)2592, height=(int)1944, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)2048, height=(int)1536, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)1920, height=(int)1080, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)1600, height=(int)1200, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)1280, height=(int)960, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)1280, height=(int)720, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)800, height=(int)600, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)640, height=(int)480, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }; image/jpeg, width=(int)320, height=(int)240, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction){ 30/1, 25/1, 20/1, 15/1, 10/1, 5/1 }
