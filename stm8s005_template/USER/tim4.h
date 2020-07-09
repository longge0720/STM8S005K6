#ifndef _TIM4_H
#define _TIM4_H

#include "stm8s.h"

void TIM4_Config(void);
void Delay_MS(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
#endif