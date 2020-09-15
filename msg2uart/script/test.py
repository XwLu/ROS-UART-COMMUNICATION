#!/usr/bin/env python
import rospy
from ros_uart_communication.msg import cmd
from geometry_msgs.msg import PoseArray

def shutdown():
    rospy.loginfo("Stopping the test")


if __name__ == '__main__':
    rospy.init_node('SerialPortTest', anonymous=True)
    rospy.on_shutdown(shutdown)
    cmd_pub = rospy.Publisher("/order", cmd, queue_size=10)
    pub_data = cmd()
    pub_data.data
    cmd_pub.publish(pub_data)
    print("cmd send!")
    print(pub_data)
    while not rospy.is_shutdown():
        print("Program is waiting!")
        rospy.sleep(1)
