from tf2_geometry_msgs import PointStamped

import rclpy
import rclpy.duration
from rclpy.node import Node

import rclpy.time
from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from rclpy.duration import Duration
import tf2_ros
import random

class FrameListener(Node):

    def __init__(self):
        super().__init__('simple_listener')
        self.registration = tf2_ros.TransformRegistration()

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)
        self.timer = self.create_timer(1.0, self.on_timer)

    def on_timer(self):

        laser_point = PointStamped()
        laser_point.header.stamp = rclpy.time.Time()
        laser_point.header.frame_id = 'base_laser'

        laser_point.point.x = random.randint(0,10)/10
        laser_point.point.y = random.randint(0,10)/10
        laser_point.point.z = random.randint(0,10)/10
        
        self.get_logger().info('base_laser: (%.2f)' % laser_point.point.x)
        # self.get_logger().info('base_laser: (%.2f)' % laser_point.point.y)
        # self.get_logger().info('base_laser: (%.2f)' % laser_point.point.z)

        try:
            
            # # base_point = self.tf_buffer.transform(laser_point, 'base_link', rclpy.time.Time(), rclpy.duration.Duration(1))
            # if (self.tf_buffer.can_transform('base_link', 'base_laser', rclpy.time.Time(), timeout=Duration(seconds=1.0))):
            #     base_point = self.tf_buffer.transform(laser_point, 'base_laser', timeout=Duration(seconds=1.0))

            base_point = self.tf_buffer.transform(laser_point, 'base_link', timeout=Duration(seconds=0))
            
            self.get_logger().info('base_link:  (%.2f)' % base_point.point.x)
            # self.get_logger().info('base_link:  (%.2f)' % base_point.point.y)
            # self.get_logger().info('base_link:  (%.2f)' % base_point.point.z)

            t = self.tf_buffer.lookup_transform(                                        # ищет саму трансформацию (сдвиг и поворот)
                'base_link',
                'base_laser',
                rclpy.time.Time())
            


        except TransformException as ex:
            pass

def main():
    rclpy.init()
    node = FrameListener()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()
