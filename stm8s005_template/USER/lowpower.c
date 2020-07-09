#include "lowpower.h"
#include "uart2.h"
#include "config.h"

volatile uint8_t IsLowPower = 0;
uint16_t LowPower_Time = 0;//����͹���ʱ��

void EnterLowPower(void)
{
    Uart2_SendStr("enter sleep\r\n");
    TIM4_Cmd(DISABLE);
    wfi();
    IsLowPower = 0;     //���Ѻ�ȡ���͹���ģʽ
    TIM4_Cmd(ENABLE);
    Uart2_SendStr("wake up\r\n");
}

//���Ѻ󾭹� xxx ms �޴���������˯��
void LowPowerScan(void)//���ж��в�Ҫ˯��
{
  if(LowPower_Time != 0)
  {
    LowPower_Time--;
    if(LowPower_Time == 0)
    {
      IsLowPower = 1;
    }
  }
}

void LowPower_Refresh(uint16_t ms)
{
  LowPower_Time = ms;
}