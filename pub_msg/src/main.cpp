//
// Created by luyifan on 18-4-9.
//
#include <ros/ros.h>
#include <ros_uart_communication/cmd.h>
#include <iostream>

int main(int argc , char **argv){
    ros::init(argc , argv , "serial_test");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<ros_uart_communication::cmd>("/order", 1000);
    ros::Rate loop_rate(1);
    uint8_t i;

    ros_uart_communication::cmd cmd_msg;
    cmd_msg.data.resize(7, 0);
    uint8_t *data_ptr = cmd_msg.data.data();
    int data_len = 2;
    std::cout<<"ready to send cmd!!"<<std::endl;
    while(ros::ok()){
        ros::spinOnce();
        i = 0;
        for( i = 0; i < 0xff; i++){
            loop_rate.sleep();
            //协议转换
            data_ptr[0] = data_ptr[1] = 0xff;
            data_ptr[2] = 0x02;
            data_ptr[3] = (uint8_t)(data_len>>8);
            data_ptr[4] = (uint8_t)(data_len & 0xff);
            data_ptr[5] = 0x01;
            data_ptr[6] = i;
            //协议转换完成
            pub.publish(cmd_msg);
        }
        std::cout<<"cmd sent!!"<<std::endl;
    }
    return 0;
}