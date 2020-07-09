#include "uart2.h"
//#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "key.h"
#include "hall.h"
#include <stdio.h>
#include "dl645.h"
#include "bluetooth.h"
#include "tim4.h"
#define PUTCHAR int fputc(int ch,FILE *f)


PUTCHAR
{
   UART2_SendData8(ch);
   while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET); 
   return ch;
}

UartData_Typedef data_uart2;

//static uint8_t OpenCmd[] = "open";
//static uint8_t CloseCmd[] = "close";
static uint8_t Stop[] = "stop";


//�����ò�����,��żУ��
void Uart2_InitSetPar(uint32_t BaudRate, UART2_WordLength_TypeDef WordLength, UART2_Parity_TypeDef Parity)
{
    UART2_DeInit();
    UART2_Init(BaudRate, WordLength, 
                UART2_STOPBITS_1, Parity, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE);
    UART2_Cmd(ENABLE);
    memset(&data_uart2,0,sizeof(data_uart2));
}
//Ĭ�ϳ�ʼ��
void Uart2_Init(void)
{
    UART2_DeInit();
    UART2_Init(115200, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE);
    UART2_Cmd(ENABLE);
    memset(&data_uart2,0,sizeof(data_uart2));
}

void Uart2_SendStr(uint8_t *str)
{
  while(*str)
  {
    UART2_SendData8(*str++);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET); 
  }
  
}

void Uart2_SendStrLen(uint8_t *str,uint8_t len)
{
  while(len--)
  {
    UART2_SendData8(*str++);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET); 
  }
  
}
#if 0
void Uart_Receive(UartData_Typedef *uart)
{
  if(uart->rec_cnt < sizeof(uart->rec))
  {
    
  }
}
#endif 
//�ڶ�ʱ���н��м���ж��Ƿ��յ�һ֡����
void Uart_Monitor(UartData_Typedef *uart)
{
  if(uart->rec_cnt > 0)//���յ�����
  {
    if(uart->rec_cnt_bck != uart->rec_cnt)//���ݷ����仯
    {
      uart->rec_cnt_bck = uart->rec_cnt;
      uart->idletmr = 0;
    }
    else
    {
      if(uart->idletmr < 300)
      {
        uart->idletmr++;
        if(uart->idletmr >= 300)
        {
          uart->flag = 1;//�յ�����֡
        }
      }
    }
  }
  else
  {
    uart->rec_cnt_bck = 0;
  }
}

int action_flag = 0;
void Uart_Driver(void)
{
    if(data_uart2.flag )
    {
      action_flag = DL645_Protocol(data_uart2.rec,data_uart2.rec_cnt);
      //Uart2_SendStrLen(data_uart2.rec,data_uart2.rec_cnt);//���Դ�������
      
 //     if(memcmp(data_uart2.rec,OpenCmd,sizeof(OpenCmd)-1) == 0)//��բ����
      if(action_flag == global_open)
      {
          if(OPEN_STATE   == 1)//����û�з�բ����Ҫ��բ
          {
//              if(AutoMode == KeySta)//�Զ�ģʽ
//              {
                Motor_RunSta = Motor_Opening;
                Motor_Counterclockwise();
//              }
//              else                 //�ֶ�ģʽ
//              {
//                
//              }
              
          }
          else                                  //�Ѿ����ڷ�բ״̬�򲻶���
          {
  
           }
 //         Uart2_SendStr("receive OpenCmd \r\n");
       }

        
//      if(memcmp(data_uart2.rec,CloseCmd,sizeof(CloseCmd)-1) == 0)//��բ����
      if(action_flag == global_close)
      {
        if((CLOSE_STATE == 1) ||(CLOSE_STATE == 0 && OPEN_STATE == 0) )//������û�к�բ
        {
          if(AutoMode == KeySta)//�Զ�ģʽ
          {
            Motor_RunSta = Motor_AutoClosing;           //�Զ���բ
            Motor_Counterclockwise();
          }
          else                   //�ֶ�ģʽ
          {
            Motor_RunSta = Motor_ManualClosing;//�ֶ���բ
            Motor_Clockwise();
          }
        }
        else    //�Ѿ���բ������
        {
          
        }
 //       Uart2_SendStr("receive CloseCmd \r\n");
      }
      if(memcmp(data_uart2.rec,Stop,sizeof(Stop)-1) == 0)
      {
        Motor_Stop();
 //       Motor_RunSta = Motor_Stoped;
        Uart2_SendStr("receive Stopcmd \r\n");
      }
      
      if(memcmp("print",data_uart2.rec,5) == 0)//��ӡ����
      {
        if(KeySta)
        {
          Uart2_SendStr("KeySta = 1 \r\n");
        }
        else
        {
          Uart2_SendStr("KeySta = 0 \r\n");
        }
        
        if(OPEN_STATE)
        {
          Uart2_SendStr("U8 = H \r\n");
        }
        else
        {
          Uart2_SendStr("U8 = L \r\n");
        }
        
         if(CLOSE_STATE)
        {
          Uart2_SendStr("U9 = H \r\n");
        }
        else
        {
          Uart2_SendStr("U9 = L \r\n");
        }
      }
      action_flag = 0;
      //data_uart2.rec_cnt = 0;
      //data_uart2.flag = 0;   
      
      memset(&data_uart2,0,sizeof(data_uart2));//��մ���2��������������
      
      if(DL645_ChangeID)                //
      {
        DL645_ChangeID = 0;
        Delay_MS(500);
        memset(&data_uart2,0,sizeof(data_uart2));//��մ���2��������������
        BlueTooth_SetName(1);
      }
    }
}
