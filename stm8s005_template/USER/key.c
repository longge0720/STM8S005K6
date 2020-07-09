#include "key.h"
#include "uart2.h"

#define KEY_PIN    !!GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)  

uint8_t KeySta = 1;//默认高电平

void Key_Init(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
}

void Key_Scan(void)
{
  static volatile uint8_t buf = 0xff;
  
  buf = (buf << 1) | KEY_PIN;
  
  if(buf == 0xff)
  {
     KeySta = 1;
  }
  else if(buf == 0)
  {
     KeySta = 0;
  }
}

void Key_Driver(void)
{
  static uint8_t bck = 1;
  if(bck != KeySta)
  {
      if(KeySta == 0)
      {
        Uart2_SendStr("key sta = 0\r\n");
      }
      else
      {
        Uart2_SendStr("key sta = 1\r\n");
      }
      bck = KeySta;
  }
}