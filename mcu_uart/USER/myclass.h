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
	volatile Queue rx_queue;//�յ������ݴ洢�����������
	uint8_t data;//ÿ��ѭ������������
	uint16_t checksum;//У���
	uint8_t buffer[MAX_DATA_LEN];//������ݵ�����
	uint8_t num;//buffer�е�ǰ��ŵ��ڼ�������
	uint8_t object;//����Ŀ��ƶ���
	uint16_t data_length;//���ݳ���
	enum Recstate								   		//״̬�� 
	{
		RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
	} rs485Recstate;
}Communication;
#endif
