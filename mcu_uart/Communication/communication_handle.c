#include "communication_handle.h"

void ClearBuffer(uint8_t *buffer, int len){
	int i = 0;
	for(i = 0; i < len; i++ ){
		buffer[i] = 0;
	}
}

/*
void FeedBack(){
	//TODO: 回传给上位机的数据在这里
}
*/

void HandleData(uint8_t buf[], uint8_t object, uint16_t len)    
{
	switch(object)
	{ 
		case 1://处理对象1
			if(buf[0] == 1){//对象1的1号设备
				break;
		   }
			else if(buf[0] == 2){
				break;					
			 }
			else if(buf[0]==3){
				break;
			}
			else break;
		case 2://处理对象2
			if(buf[0] == 1){
				
				break;
		   }
			else if(buf[0] == 2){
				break;					
			 }
			else if(buf[0]==3){
				break;
			}
			else break;
		default:break;
	}  
} 

