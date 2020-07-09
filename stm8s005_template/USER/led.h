#ifndef _LED_H
#define _LED_H

#include "stm8s.h"

#define LED_REVERSE()     GPIO_WriteReverse(GPIOB,GPIO_PIN_4)
#define LED_BRIGHT()      GPIO_WriteHigh(GPIOB,GPIO_PIN_4)
#define LED_DARK()        GPIO_WriteLow(GPIOB,GPIO_PIN_4)

void Led1_Init(void);
void Led1_Flash(void);
void Led1_Reverse(void);
#endif