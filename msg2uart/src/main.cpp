//
// Created by luyifan on 18-4-8.
//
#include <ros_uart_communication/SerialNode.h>

using namespace SerialNode;
int main(int argc, char **argv){
    ros::init(argc, argv, "ros_uart");
    ros::NodeHandle nh;
    Serial serial(nh);
    ros::spin();
    return 0;
}