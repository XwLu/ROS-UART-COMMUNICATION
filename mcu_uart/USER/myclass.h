#ifndef _MYCLASS_H_
#define _MYCLASS_H_
#include "string.h"
#include "stm32f10x.h"
#include "queue.h"

#define MAX_DATA_LEN 100

/*smart home struct*/
typedef struct room{
	unsigned char room_id;
	unsigned char light1;
	unsigned char light2;
	unsigned char light3;
	unsigned char curtain1;
	unsigned char curtain2;
} Room;

typedef struct House{
	Room bedrroom;
	Room kitchen;
	Room dining_room;
}House;

typedef struct Communication{
	volatile Queue rx_queue;//收到的数据存储在这个队列中
	uint8_t data;//每次循环读到的数据
	uint16_t checksum;//校验和
	uint8_t buffer[MAX_DATA_LEN];//存放数据的数组
	uint8_t num;//buffer中当前存放到第几个数据
	uint8_t object;//命令的控制对象
	uint16_t data_length;//数据长度
	enum Recstate								   		//状态机 
	{
		RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
	} rs485Recstate;
}Communication;
#endif
