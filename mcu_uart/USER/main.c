#include "myclass.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "communication_handle.h"
//ALIENTEK战舰STM32开发板实验4
//串口实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

Communication communication;

 void Init()
 {
	 delay_init();	    	 //延时函数初始化	  
	 NVIC_Configuration(); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 uart_init(115200);	   //串口初始化为115200
 }
 
 void communicationInit(){
   /*通信初始化*/
	 communication.num = 0;
	 ClearBuffer(communication.buffer, MAX_DATA_LEN);
	 communication.rs485Recstate = RECFF1;
 }

int main(void)
{
  Init();             //系统初始化
	communicationInit();//通信初始化
	
	while(1)
	{
		if(queue_empty(communication.rx_queue))//如果队列是空的就不进入状态机的循环
			            continue;

		communication.data = (uint8_t)queue_get(&communication.rx_queue);
  
		switch (communication.rs485Recstate)//应该在Init函数里面就初始化
		{
			case RECFF1:
				if (communication.data == 0xff){
					communication.rs485Recstate = RECFF2;
					communication.checksum=0;//校验位清零
					communication.num= 0;
				}
				break;
	
			case RECFF2:
				if (communication.data == 0xff)
					communication.rs485Recstate = RECID;
				else
					communication.rs485Recstate = RECFF1;
				break;
	
			case RECID:				 					
			  if(communication.data <= 10){//控制对象1-10
					communication.object = communication.data;
					communication.checksum += communication.data;
					communication.rs485Recstate = RECLEN1;
				  }
				else
					communication.rs485Recstate = RECFF1;	  
				break;
	
			case RECLEN1:				 				
				communication.checksum += communication.data;
			  communication.data_length = 0x0000;
				communication.data_length = communication.data << 8 | 0x00;
				communication.rs485Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				communication.checksum += communication.data;			 				
				communication.data_length = communication.data_length | communication.data;
				communication.rs485Recstate = RECSEL;
				break;
	
			case RECSEL:
				communication.checksum += communication.data;
				communication.buffer[communication.num++] = communication.data;
				if(communication.num >= communication.data_length)
					communication.rs485Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				communication.checksum = communication.checksum%256;
				
				if(communication.data == communication.checksum){			
					HandleData(communication.buffer, communication.object, communication.data_length);
					
  				//FeedBack();	//数据反馈在这里
					communication.checksum=0;	
					communication.rs485Recstate = RECFF1;	 
				}
				else
					communication.rs485Recstate = RECFF1;
				break;
			  default:
					communication.rs485Recstate = RECFF1;
		 }	   	
	 }	 
 }


