#include "communication_handle.h"

void ClearBuffer(uint8_t *buffer, int len){
	int i = 0;
	for(i = 0; i < len; i++ ){
		buffer[i] = 0;
	}
}

/*
void FeedBack(){
	//TODO: �ش�����λ��������������
}
*/

void HandleData(uint8_t buf[], uint8_t object, uint16_t len)    
{
	switch(object)
	{ 
		case 1://�������1
			if(buf[0] == 1){//����1��1���豸
				break;
		   }
			else if(buf[0] == 2){
				break;					
			 }
			else if(buf[0]==3){
				break;
			}
			else break;
		case 2://�������2
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

