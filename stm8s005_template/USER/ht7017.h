#ifndef _HT7017_H
#define _HT7017_H
#define HT7017_WRITE            Uart2_SendStrLen
//#include <stdint.h>
#include <stm8s.h>
void ht7017_init(void);
int ht7017_ReadReg(uint8_t reg,uint32_t *dat);


#endif
