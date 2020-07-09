#ifndef _HALL_H
#define _HALL_H

#include "stm8s.h"
/*
typedef enum
{
  
}Hall_Typedef;*/

#define P_LACK    !!GPIO_ReadInputPin(GPIOB, GPIO_PIN_2)        //对应原理图
#define P_NORMAL  !!GPIO_ReadInputPin(GPIOB, GPIO_PIN_3)

//对应程序逻辑
#define OPEN_STATE           P_LACK                   //低电平处于分闸状态，高电平不处于分闸状态
#define CLOSE_STATE          P_NORMAL                 //低电平处于合闸状态，高电平不处于合闸状态

void Hall_Init(void);
uint8_t GetHall_State(void);

#endif
