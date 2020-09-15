#include "myclass.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "communication_handle.h"
//ALIENTEKս��STM32������ʵ��4
//����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

Communication communication;

 void Init()
 {
	 delay_init();	    	 //��ʱ������ʼ��	  
	 NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart_init(115200);	   //���ڳ�ʼ��Ϊ115200
 }
 
 void communicationInit(){
   /*ͨ�ų�ʼ��*/
	 communication.num = 0;
	 ClearBuffer(communication.buffer, MAX_DATA_LEN);
	 communication.rs485Recstate = RECFF1;
 }

int main(void)
{
  Init();             //ϵͳ��ʼ��
	communicationInit();//ͨ�ų�ʼ��
	
	while(1)
	{
		if(queue_empty(communication.rx_queue))//��������ǿյľͲ�����״̬����ѭ��
			            continue;

		communication.data = (uint8_t)queue_get(&communication.rx_queue);
  
		switch (communication.rs485Recstate)//Ӧ����Init��������ͳ�ʼ��
		{
			case RECFF1:
				if (communication.data == 0xff){
					communication.rs485Recstate = RECFF2;
					communication.checksum=0;//У��λ����
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
			  if(communication.data <= 10){//���ƶ���1-10
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
					
  				//FeedBack();	//���ݷ���������
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


