//
// Created by luyifan on 18-4-8.
//

#include <ros_uart_communication/SerialNode.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <boost/bind.hpp>


using namespace SerialNode;
using namespace boost;
Serial::Serial(ros::NodeHandle nh)
:nh(nh){
    nh.param("serial_port", serial_port_, string("/dev/ttyUSB0"));
    nh.param("buad_rate", baud_rate_, 115200);
    SerialParams serial_params(serial_port_, baud_rate_);

    position_sub = nh.subscribe("/order", 1000, &Serial::dataCallback, this);

    main_serial = make_shared<SerialPort>();
    main_serial->setSerialParams(serial_params);
    main_serial->setCallbackFunc(bind(&Serial::serialCall,this,_1,_2));
    main_serial->setTimeOut(40); //Unit: ms
    main_serial->startThread();
}

Serial::~Serial() {nh.shutdown();}

void Serial::dumpBuffer(const char *buffer, int elements) {
    int j;
    printf(" [");
    for(j = 0; j < elements; j++){
        if(j > 0)
            printf(" ");
        printf("0x%02X", (unsigned int)(buffer[j]&0x0ff));
    }
    printf("]\n");
}

void Serial::serialCall(ByteVector current_data, int id) {
    ros::Publisher &serial_pub = pub_[id];
    stringstream recv_name;
    if(!serial_pub){
        recv_name<<"/feedback"<<id;
        serial_pub = nh.advertise<ros_uart_communication::feedback>(recv_name.str(), 100);
    }
    ros_uart_communication::feedback pub_data;
    pub_data.id = id;
    pub_data.header.stamp = ros::Time::now();
    for(int i = 0; i < current_data.size(); i++){
        pub_data.data.push_back(current_data[i]);
    }
    serial_pub.publish(pub_data);
}

void Serial::dataCallback(const ros_uart_communication::cmd::ConstPtr &ptr) {
    ByteVector current_data;
    size_t data_len = ptr->data.size();
    current_data.resize(data_len+1, 0);

    uint32_t check_sum = 0;

    for(size_t i = 0; i < data_len; i++){
        if(i>=2) check_sum += ptr->data[i];
        current_data[i] = ptr->data[i];
    }
    current_data[data_len] = (uint8_t)(check_sum%256); //set the last position as the checksum
    main_serial->writeRaw(current_data);
}