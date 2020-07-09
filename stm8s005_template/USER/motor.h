#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm8s.h"


typedef enum 
{
    Motor_Default = 0,
    Motor_AutoClosing,//�Զ���բ
    Motor_ManualClosing,//�ֶ���բ
    Motor_Opening,      //��բ
    Motor_Stoped,
    Motor_Err,
}MotorRunSta_Typedef;    

extern volatile uint8_t Motor_RunSta;

void Motor_Init(void);
void Motor_Stop(void);
void Motor_Counterclockwise(void);
void Motor_Clockwise(void);
void Motor_RunTimeCount(void);

void Motor_Driver(void);
#endif
