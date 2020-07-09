#include "motor.h"
#include "hall.h"
#include "lowpower.h"
#include "dl645.h"

#define NSZ_H           GPIO_WriteHigh(GPIOA,GPIO_PIN_1)
#define NSZ_L           GPIO_WriteLow(GPIOA,GPIO_PIN_1)

#define SSZ_H           GPIO_WriteHigh(GPIOA,GPIO_PIN_2)  
#define SSZ_L           GPIO_WriteLow(GPIOA,GPIO_PIN_2)

volatile uint8_t Motor_RunSta = 0;//电机运行状态
uint16_t Motor_RunTime = 0;
void Motor_Init(void)
{
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_SLOW);//SCR
  
  GPIO_WriteHigh(GPIOD,GPIO_PIN_0);
}

void Motor_Stop(void)
{
  SSZ_H;
  NSZ_H;
  
  DISABLE_SWITCH_POWER;
  Motor_RunTime = 0;//清零电机运行时间
}

//逆时针旋转
void Motor_Counterclockwise(void)
{
  ENABLE_SWITCH_POWER;
  NSZ_H;
  SSZ_L;
}

//顺时针旋转
void Motor_Clockwise(void)
{
  ENABLE_SWITCH_POWER;
  NSZ_L;
  SSZ_H;
}

//电机驱动
void Motor_Driver(void)
{
    switch(Motor_RunSta)//电机运行状态
    {
        case Motor_AutoClosing://电机自动开关模式
            if((CLOSE_STATE == 0)&&(OPEN_STATE == 1)&&(Motor_RunTime < 6000))//已经合闸，且电机运行时间未超时
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
        case Motor_ManualClosing://手动模式暂时没有弄
          if((CLOSE_STATE == 0)&&(OPEN_STATE == 1)&&(Motor_RunTime < 6000))//已经合闸，且电机运行时间未超时
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
        case Motor_Opening:                                           //分闸
            if((OPEN_STATE == 0)&&(Motor_RunTime < 6000))//已经分闸，且运行大约800ms
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
            
        default:break;
    }
}

void Motor_RunTimeCount(void)
{
    //电机处于运行时才开始记录电机运行时间
    if((Motor_RunSta == Motor_AutoClosing) ||(Motor_RunSta ==  Motor_ManualClosing)||(Motor_RunSta == Motor_Opening) )//处于这三种状态下开始记录电机运行时间
    {
        Motor_RunTime++;//记录电机运行时间
    }
}

void Switch_PowerScan(void)
{
  static uint16_t cnt = 0;
  if(SWITCH_POWER_STATE != 0)//开关电源开启
  {
    cnt++;
    if(cnt >= 10000)//10s
    {
      cnt = 0;
      DISABLE_SWITCH_POWER;//关闭开关电源
    }
  }
}