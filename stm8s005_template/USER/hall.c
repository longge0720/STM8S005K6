#include "hall.h"

//开关电源给霍尔供电，所以通过霍尔检测位置时需要开启开关电源

void Hall_Init(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);//p_lack
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);//p_normal
}

//四种状态 00  01 10 11
uint8_t GetHall_State(void)
{
  uint8_t tem = 0;
  tem = (uint8_t)P_LACK;
  tem <<= 1;
  tem |= (uint8_t)P_NORMAL;
  
  return tem;
}
