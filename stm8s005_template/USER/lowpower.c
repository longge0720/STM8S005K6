#include "lowpower.h"
#include "uart2.h"
#include "config.h"

volatile uint8_t IsLowPower = 0;
uint16_t LowPower_Time = 0;//进入低功耗时间

void EnterLowPower(void)
{
    Uart2_SendStr("enter sleep\r\n");
    TIM4_Cmd(DISABLE);
    wfi();
    IsLowPower = 0;     //唤醒后取消低功耗模式
    TIM4_Cmd(ENABLE);
    Uart2_SendStr("wake up\r\n");
}

//唤醒后经过 xxx ms 无串口数据则睡眠
void LowPowerScan(void)//在中断中不要睡眠
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