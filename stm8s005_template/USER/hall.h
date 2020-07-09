#ifndef _HALL_H
#define _HALL_H

#include "stm8s.h"
/*
typedef enum
{
  
}Hall_Typedef;*/

#define P_LACK    !!GPIO_ReadInputPin(GPIOB, GPIO_PIN_2)        //��Ӧԭ��ͼ
#define P_NORMAL  !!GPIO_ReadInputPin(GPIOB, GPIO_PIN_3)

//��Ӧ�����߼�
#define OPEN_STATE           P_LACK                   //�͵�ƽ���ڷ�բ״̬���ߵ�ƽ�����ڷ�բ״̬
#define CLOSE_STATE          P_NORMAL                 //�͵�ƽ���ں�բ״̬���ߵ�ƽ�����ں�բ״̬

void Hall_Init(void);
uint8_t GetHall_State(void);

#endif
