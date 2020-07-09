#ifndef _KEY_H
#define _KEY_H

#include "stm8s.h"

enum
{
  AutoMode = 0,
  ManuMode = 1,
};
extern uint8_t KeySta;

void Key_Init(void);
void Key_Scan(void);
void Key_Driver(void);

#endif
