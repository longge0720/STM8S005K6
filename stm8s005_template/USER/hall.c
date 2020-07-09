#include "hall.h"

//���ص�Դ���������磬����ͨ���������λ��ʱ��Ҫ�������ص�Դ

void Hall_Init(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);//p_lack
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);//p_normal
}

//����״̬ 00  01 10 11
uint8_t GetHall_State(void)
{
  uint8_t tem = 0;
  tem = (uint8_t)P_LACK;
  tem <<= 1;
  tem |= (uint8_t)P_NORMAL;
  
  return tem;
}
