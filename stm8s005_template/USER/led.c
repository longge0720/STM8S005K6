#include "led.h"
//Pd3
void Led1_Init(void)
{
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_WriteLow(GPIOD, GPIO_PIN_3);
}

void Led1_Flash(void)
{
  static uint16_t div = 0;//亮灭周期为5s
  div++;
  if(div == 5000)
  {
    div = 0;
    GPIO_WriteReverse(GPIOD,GPIO_PIN_3);
  }
}

void Led1_Reverse(void)
{
  GPIO_WriteReverse(GPIOD,GPIO_PIN_3);
}




//PD4
void Led2_Init(void)
{
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_WriteLow(GPIOD, GPIO_PIN_4);
}

void Led2_Flash(void)
{
  static uint16_t div = 0;//亮灭周期为5s
  div++;
  if(div == 5000)
  {
    div = 0;
    GPIO_WriteReverse(GPIOD,GPIO_PIN_4);
  }
}

void Led2_Reverse(void)
{
  GPIO_WriteReverse(GPIOD,GPIO_PIN_4);
}


#if 0
void Led1_Flash(uint8_t cnt)//周期性闪烁n次
{
  static uint16_t div = 0;//亮灭周期为1s
  if(cnt > 0)
  {
    div++;
    if(div == 500)
    {
      div = 0;
      cnt--;
      GPIO_WriteReverse(GPIOB,GPIO_PIN_4);
    }
  }
}

#endif