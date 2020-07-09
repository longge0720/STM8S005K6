#ifndef _UART2_H
#define _UART2_H

#include "stm8s.h"

typedef struct 
{
  uint8_t rec[50];              //���ܻ�����
  uint8_t rec_cnt;              //���ո���
  uint8_t rec_cnt_bck;          //���ո������ݣ������ж�����֡
  uint16_t idletmr;              //����ʱ��
  uint8_t flag;
}UartData_Typedef;

extern UartData_Typedef data_uart2;

void Uart2_Init(void);
void Uart2_InitSetPar(uint32_t BaudRate, UART2_WordLength_TypeDef WordLength, UART2_Parity_TypeDef Parity);
void Uart_Driver(void);
void Uart2_SendStr(uint8_t *str);
void Uart2_SendStrLen(uint8_t *str,uint8_t len);
void Uart_Monitor(UartData_Typedef *uart);

#endif
