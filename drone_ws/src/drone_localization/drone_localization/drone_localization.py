# ros2 topic echo /tf_static

import math
import sys
from tf2_geometry_msgs import TransformStamped
from geometry_msgs.msg import TransformStamped as GeomTr 
import numpy as np
import rclpy
import rclpy.time
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
from tf2_ros.transform_listener import TransformListener
import cv2
from cv2 import aruco
import yaml
import numpy as np
import tf2_ros
from tf2_ros.buffer import Buffer
import rclpy.duration
from rclpy.duration import Duration
# from tf2_ros import transformations as t
# from tf2_ros import 
from tf2_ros import TransformException
from numpy.linalg import inv

def transform_inverse(rvec, tvec):
    # transform_inv = TransformStamped()
    
    eulerX = rvec[0][0][0]
    eulerY = rvec[0][1][0]
    eulerZ = rvec[0][2][0]
    
    cosX = math.cos(eulerX)
    cosY = math.cos(eulerY)
    cosZ = math.cos(eulerZ)

    sinX = math.sin(eulerX)
    sinY = math.sin(eulerY)
    sinZ = math.sin(eulerZ)

    cc = cosX * cosZ 
    cs = cosX * sinZ 
    sc = sinX * cosZ 
    ss = sinX * sinZ

    rotation_mat_dir = np.array([[cosY * cosZ, sinY * sc - cs, sinY * cc + ss],
                                       [cosY * sinZ, sinY * ss + cc, sinY * cs - sc], 
                                       [-sinY,     cosY * sinX,      cosY * cosX]])
    rotation_mat_inv = inv(rotation_mat_dir)
    # print("rotation_mat_dir:\n", rotation_mat_dir)
    # print("rotation_mat_inv:\n", rotation_mat_inv)
    
    translation_vec_dir = np.array([tvec[0][0][0], tvec[0][1][0], tvec[0][2][0]])
    translation_vec_dir_neg = (-1) * translation_vec_dir
    translation_vec_inv = np.dot(rotation_mat_inv, translation_vec_dir_neg)
    # print("translation_vec_dir:\n", translation_vec_dir)
    # print("translation_vec_inv:\n", translation_vec_inv)
    
    alfa = math.atan(rotation_mat_inv[2][1]/rotation_mat_inv[2][2])
    beta = math.atan((-1)*rotation_mat_inv[2][0]/(math.sqrt(1-rotation_mat_inv[2][0]**2)))
    gamma = math.atan(rotation_mat_inv[1][0]/rotation_mat_inv[0][0])
    
    rvec_inv = np.array([alfa, beta, gamma])
    tvec_inv = np.array([translation_vec_inv[0], translation_vec_inv[1], translation_vec_inv[2]])

    return rvec_inv, tvec_inv

def quaternion_from_euler(ai, aj, ak):
    ai /= 2.0
    aj /= 2.0
    ak /= 2.0
    ci = math.cos(ai)
    si = math.sin(ai)
    cj = math.cos(aj)
    sj = math.sin(aj)
    ck = math.cos(ak)
    sk = math.sin(ak)
    cc = ci*ck
    cs = ci*sk
    sc = si*ck
    ss = si*sk

    q = np.empty((4, ))
    q[0] = cj*sc - sj*cs
    q[1] = cj*ss + sj*cc
    q[2] = cj*cs - sj*sc
    q[3] = cj*cc + sj*ss

    return q

def my_estimatePoseSingleMarkers(corners, marker_size, mtx, distortion):
    marker_points = np.array([[-marker_size / 2, marker_size / 2, 0],
                            [marker_size / 2, marker_size / 2, 0],
                            [marker_size / 2, -marker_size / 2, 0],
                            [-marker_size / 2, -marker_size / 2, 0]], dtype=np.float32)
    trash = []
    rvecs = []
    tvecs = []
    for c in corners:
        nada, R, t = cv2.solvePnP(marker_points, c, mtx, distortion, False, cv2.SOLVEPNP_IPPE_SQUARE)
        rvecs.append(R)
        tvecs.append(t)
        trash.append(nada)
    return rvecs, tvecs, trash
    
class DroneLocalization(Node):

    def __init__(self):
        super().__init__('drone_localization')

        # Initialize the transform broadcaster
        self.tf_broadcaster = TransformBroadcaster(self)

        # Initialize the listener
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # main function
        self.localize_drone()

    def localize_odom2ar(self):
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'marker_1'

        t.transform.translation.x = 0.0
        t.transform.translation.y = 0.0
        t.transform.translation.z = 0.0
        quat = quaternion_from_euler(
            float(0.0), float(0.0), float(0.0))
        t.transform.rotation.x = quat[0]
        t.transform.rotation.y = quat[1]
        t.transform.rotation.z = quat[2]
        t.transform.rotation.w = quat[3]

        self.tf_broadcaster.sendTransform(t)

    def localize_cam2ar(self, tvec, rvec):
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'marker_1'
        t.child_frame_id = 'camera'

        t.transform.translation.x = tvec[0][0][0]
        t.transform.translation.y = tvec[0][1][0]
        t.transform.translation.z = tvec[0][2][0]
        quat = quaternion_from_euler(
            float(rvec[0][0][0]), float(rvec[0][1][0]), float(rvec[0][2][0]))
        t.transform.rotation.x = quat[0]
        t.transform.rotation.y = quat[1]
        t.transform.rotation.z = quat[2]
        t.transform.rotation.w = quat[3]

        self.tf_broadcaster.sendTransform(t)

        return t

    def localize_ar2cam(self):

        # # ar2camTransform = cam2arTransform.in
        # if (tf_buffer.can_transform('camera', 'marker_1', rclpy.time.Time())):
        #     print('ALL OK')

        rclpy.spin_once(self)
        try:
            ar2camTransform = self.tf_buffer.lookup_transform(
                'odom',
                'camera',
                rclpy.time.Time())
            # print('ALL OK')
        except TransformException as ex:
            self.get_logger().info(
                f'Could not transform odom to camera: {ex}')
        # print("ar2camTransform", ar2camTransform)

    def localize_drone(self):
        video_capture = cv2.VideoCapture(0, cv2.CAP_V4L2)
        ret_val, frame = video_capture.read()

        with open('/home/anton202/devlab/drone_ws/src/drone_localization/drone_localization/calibration.yaml') as f:
            loadeddict = yaml.safe_load(f)
        mtx = loadeddict.get('camera_matrix')
        dist = loadeddict.get('dist_coeff')
        mtx = np.array(mtx)
        dist = np.array(dist)
        h,  w = frame.shape[:2]
        newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))

        dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_250)
        parameters =  cv2.aruco.DetectorParameters()
        detector = cv2.aruco.ArucoDetector(dictionary, parameters)

        if video_capture.isOpened():
            try:
                while True:

                    ret_val, frame = video_capture.read()
                    # cv2.imshow("frame", frame)

                    rvec = []
                    tvec = []
                    markerPoints = []
                    markerCorners, markerIds, rejectedCandidates = detector.detectMarkers(frame)
                    if len(markerCorners) > 0:
                        for i in range(0, len(markerIds)):
                            rvec, tvec, markerPoints = my_estimatePoseSingleMarkers(markerCorners, 0.05, newcameramtx, dist)
                            rvec_inv, tvec_inv = transform_inverse(rvec, tvec)
                            # print("Translation cam2ar: ", tvec)
                            # print("Rotation cam2ar: ", rvec)
                            # self.localize_odom2ar()
                            # self.localize_cam2ar(tvec, rvec)
                            # rclpy.spin_once(self)
                            # print("cam2arTransform", cam2arTransform)
                            # self.localize_ar2cam()

                    img_aruco = aruco.drawDetectedMarkers(frame.copy(), markerCorners, markerIds)
                    # cv2.aruco.drawAxis(frame, newcameramtx, dist, rvec, tvec, 0.01)
                    cv2.imshow("aruco", img_aruco)
                    cv2.waitKey(1)
            finally:
                video_capture.release()
                cv2.destroyAllWindows()
        else:
            print("Error: Unable to open camera")

def main():
    rclpy.init()
    node = DroneLocalization()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()
