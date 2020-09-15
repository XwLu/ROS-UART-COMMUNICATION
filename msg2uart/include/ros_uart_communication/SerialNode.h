//
// Created by luyifan on 18-4-8.
//

#ifndef ROS_UART_COMMUNICATION_SERIALNODE_H
#define ROS_UART_COMMUNICATION_SERIALNODE_H

#include <ros/ros.h>
#include "SerialPort.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <map>
#include <ros_uart_communication/feedback.h>
#include <ros_uart_communication/cmd.h>

using namespace serial;
using namespace boost;

namespace SerialNode{
    class Serial{
    public:
        Serial(ros::NodeHandle nh);
        ~Serial();

        void serialCall(ByteVector current_data, int id);

    private:
        string serial_port_;
        int baud_rate_;
        shared_ptr<SerialPort>main_serial;

        ros::NodeHandle nh;
        ros::Subscriber position_sub;
        void dumpBuffer(const char *buffer, int elements);
        void dataCallback(const ros_uart_communication::cmd::ConstPtr &ptr);
        map<u_int8_t, ros::Publisher> pub_;

    protected:
    };
}

#endif //ROS_UART_COMMUNICATION_SERIALNODE_H
