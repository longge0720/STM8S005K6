#ifndef _LED_H
#define _LED_H

#include "stm8s.h"

#define LED_REVERSE()     GPIO_WriteReverse(GPIOD,GPIO_PIN_3)
#define LED_BRIGHT()      GPIO_WriteHigh(GPIOD,GPIO_PIN_3)
#define LED_DARK()        GPIO_WriteLow(GPIOD,GPIO_PIN_3)

#define LED2_REVERSE()     GPIO_WriteReverse(GPIOD,GPIO_PIN_4)
#define LED2_BRIGHT()      GPIO_WriteHigh(GPIOD,GPIO_PIN_4)
#define LED2_DARK()        GPIO_WriteLow(GPIOD,GPIO_PIN_4)

void Led1_Init(void);
void Led1_Flash(void);
void Led1_Reverse(void);


void Led2_Init(void);
void Led2_Flash(void);
void Led2_Reverse(void);


#endif