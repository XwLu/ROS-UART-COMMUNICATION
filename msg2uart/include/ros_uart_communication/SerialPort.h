//
// Created by luyifan on 18-4-8.
//

#ifndef ROS_UART_COMMUNICATION_SERIALPORT_H
#define ROS_UART_COMMUNICATION_SERIALPORT_H

#include <inttypes.h>
#include <vector>
#include <queue>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

namespace serial{

  /*串口通信选项*/
  class SerialParams{
  public:
    string serialPort;        //串口的设备文件
    unsigned int baud_rate;   //波特率
    unsigned int flow_control;//流控
    unsigned int parity;      //校验位
    unsigned int stop_bits;   //停止位

    SerialParams():
        serialPort("/dev/ttyUSB0"), baud_rate(115200), flow_control(0), parity(0), stop_bits(0)
    {
    }
    SerialParams(
        string _serial_port,
        unsigned int _baud_rate,
        unsigned int _flow_control = 0,
        unsigned int _parity = 0,
        unsigned int _stop_bits = 0
    ) :
        serialPort(_serial_port),
        baud_rate(_baud_rate),
        flow_control(_flow_control),
        parity(_parity),
        stop_bits(_stop_bits)
    {
    }
  };

  typedef vector<uint8_t> ByteVector;
  typedef shared_ptr<ByteVector> pByteVector;

  class SerialPort{
  private:
    shared_ptr<deadline_timer> m_ptimer;  //超时定时器
    shared_ptr<io_service> m_pios;        //io_service对象
    shared_ptr<serial_port> m_pSerial;    //串口对象的指针
    mutex m_serialMutex;                  //串口对象的互斥锁. 按照boost官方文档, serial_port对象不是线程安全的. 故需要此锁

    enum {HEADER_LEN = 3};

    enum STATE {
      WAITING_FF, WAITING_FF2, READING_HEAD, READING_DATA, READING_CHECKSUM
    } m_state;

    SerialParams m_serialParams; 		// 串口的配置数据
    int m_timeOut; 				// 数据报超时时间

    ByteVector m_tempBuf;				// 数据读取的临时缓冲区

    ByteVector m_currentHeader;		// 正在读取的报头(4字节)
    size_t m_HeaderBytesRead;		// 报头已经读取的字节数

    ByteVector m_currentData;			// 正在读取的报文数据
    size_t m_DataBytesRead;		// 数据已经读取的字节数

    queue<pByteVector> m_writeQueue;		// 待发送数据的队列
    mutex m_writeQueueMutex;	// 队列的互斥锁

    function<void(ByteVector, int)> m_dataCallbackFunc;		// 数据回调函数
    function<void()> m_errorCallbackFunc;	// 错误回调函数

    // 跑io_service::run()的线程
    boost::thread m_thread;

    // 线程的主过程, 主要是在跑io_service::run()
    void mainRun();

    // 为了方便写的函数
    void start_a_read();
    void start_a_write();

    // async_read_some的Handler
    void readHandler(const system::error_code &ec, size_t bytesTransferred);
    // async_write_some的Handler
    void writeHandler(const system::error_code &ec);
    // 超时定时器的Handler
    void timeoutHandler(const system::error_code &ec);

  public:
    SerialPort();

    virtual ~SerialPort();

    void setSerialParams(const SerialParams &params);		// 设置串口参数

    void setTimeOut(int timeout);							// 设置超时时间

    bool startThread();										// 启动线程
    bool stopThread();										// 停止线程

    // 设置收到数据之后的回调函数
    void setCallbackFunc(const function<void(ByteVector,int)> &func);

    // 向串口中直接写入一串数据
    bool writeRaw(const ByteVector &rawData);
  };
}

#endif //ROS_UART_COMMUNICATION_SERIALPORT_H
