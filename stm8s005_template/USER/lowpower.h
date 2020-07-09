#ifndef _LOWPOER_H
#define _LOWPOER_H

#include "stm8s.h"

#define ENABLE_SWITCH_POWER             GPIO_WriteHigh(GPIOD,GPIO_PIN_0)//使能开关电源
#define DISABLE_SWITCH_POWER            GPIO_WriteLow(GPIOD,GPIO_PIN_0)
#define SWITCH_POWER_STATE              GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) 

extern volatile uint8_t IsLowPower;

void EnterLowPower(void);
void LowPowerScan(void);
void LowPower_Refresh(uint16_t ms);
#endif
