#include "motor.h"
#include "hall.h"
#include "lowpower.h"
#include "dl645.h"

#define NSZ_H           GPIO_WriteHigh(GPIOA,GPIO_PIN_1)
#define NSZ_L           GPIO_WriteLow(GPIOA,GPIO_PIN_1)

#define SSZ_H           GPIO_WriteHigh(GPIOA,GPIO_PIN_2)  
#define SSZ_L           GPIO_WriteLow(GPIOA,GPIO_PIN_2)

volatile uint8_t Motor_RunSta = 0;//�������״̬
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
  Motor_RunTime = 0;//����������ʱ��
}

//��ʱ����ת
void Motor_Counterclockwise(void)
{
  ENABLE_SWITCH_POWER;
  NSZ_H;
  SSZ_L;
}

//˳ʱ����ת
void Motor_Clockwise(void)
{
  ENABLE_SWITCH_POWER;
  NSZ_L;
  SSZ_H;
}

//�������
void Motor_Driver(void)
{
    switch(Motor_RunSta)//�������״̬
    {
        case Motor_AutoClosing://����Զ�����ģʽ
            if((CLOSE_STATE == 0)&&(OPEN_STATE == 1)&&(Motor_RunTime < 6000))//�Ѿ���բ���ҵ������ʱ��δ��ʱ
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
        case Motor_ManualClosing://�ֶ�ģʽ��ʱû��Ū
          if((CLOSE_STATE == 0)&&(OPEN_STATE == 1)&&(Motor_RunTime < 6000))//�Ѿ���բ���ҵ������ʱ��δ��ʱ
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
        case Motor_Opening:                                           //��բ
            if((OPEN_STATE == 0)&&(Motor_RunTime < 6000))//�Ѿ���բ�������д�Լ800ms
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
            
        default:break;
    }
}

void Motor_RunTimeCount(void)
{
    //�����������ʱ�ſ�ʼ��¼�������ʱ��
    if((Motor_RunSta == Motor_AutoClosing) ||(Motor_RunSta ==  Motor_ManualClosing)||(Motor_RunSta == Motor_Opening) )//����������״̬�¿�ʼ��¼�������ʱ��
    {
        Motor_RunTime++;//��¼�������ʱ��
    }
}

void Switch_PowerScan(void)
{
  static uint16_t cnt = 0;
  if(SWITCH_POWER_STATE != 0)//���ص�Դ����
  {
    cnt++;
    if(cnt >= 10000)//10s
    {
      cnt = 0;
      DISABLE_SWITCH_POWER;//�رտ��ص�Դ
    }
  }
}