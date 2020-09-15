#ifndef _COMMUNICATION_HANDLE_H
#define _COMMUNICATION_HANDLE_H

#include "sys.h"
#include "myclass.h"

//void FeedBack();
void ClearBuffer(uint8_t *buffer, int len);
void HandleData(uint8_t buf[], uint8_t object, uint16_t len);//上位机数据处理

#endif
