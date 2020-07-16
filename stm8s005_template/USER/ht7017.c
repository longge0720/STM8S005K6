#include "ht7017.h"
#include <string.h>
#include <stdio.h>
#include "uart2.h"
//ϵͳ�临λ��Ҫ20ms
//ϵͳ�ȸ�λ��Ҫ2ms
//HT7017��д��־λ
#define HT7017_READ_BIT         (~(0x01 << 7))
#define HT7017_WRITE_BIT        (0x01 << 7)
#define HT7017_ACK              0x54
#define HT7017_NAK              0x63
#define HT7017_WRITE            Uart2_SendStrLen

//extern UartRec_Typedef uart2Rec;
//extern UartData_Typedef data_uart2;

//SemaphoreHandle_t xHt7017Rec_Semphr = NULL;
//SemaphoreHandle_t xHt7017Mux_Semphr = NULL;//�����ź���




static uint8_t ht7017_checkCS(uint8_t *dat,uint8_t len)
{
  uint8_t cs = 0;
  for(int i=0;i<len;i++)
  {
    cs += *dat++;
  }
  
  cs = ~cs;
  
  return cs;
}

//int ht7017_WriteReg(uint8_t reg,uint16_t dat)
//{
//  xSemaphoreTake(xHt7017Mux_Semphr,portMAX_DELAY);
//  
//  uint8_t buf[6] = {0};
//  uint8_t cs = 0;
//  BaseType_t xReturn = pdPASS;
//  
//  buf[0] = 0x6a;
//  buf[1] = (reg | HT7017_WRITE_BIT);
//  buf[2] = (dat >> 8);
//  buf[3] = (uint8_t)(dat & 0x00ff);
//  
//  cs = ht7017_checkCS(buf,4);//����cs
//  buf[4] = cs;
//  
//  HT7017_WRITE(buf,5);//send buf
//  vTaskDelay(10);
//  //���ղ��ְ�FreeRTOS��ʹ�ö�ֵ�ź���ʵ��
//  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,50);//���ڷ���ֵ�ȴ�50��tick
//  //if(xReturn == pdPASS)//�ȵ����ڽ��ܵ�����
//  {
//    if(uart1Rec.rec[0] == HT7017_ACK)//����յ�ACK��־
//    {
//      xReturn = pdPASS;
//    //DBG("ht write succ\r\n");
//
//    }
//    else if(uart1Rec.rec[0] ==HT7017_NAK)//�յ�����NACK
//    {
//      xReturn = pdFAIL;
//    //DBG("ht write err noack\r\n");
//
//    }
//    
//    uart1Rec.recNum = 0;
//    memset(uart1Rec.rec,0,sizeof(uart1Rec.rec));
//  }
///*
//  else
//  {
//    xReturn = pdFAIL;
//  }*/
//
//  xSemaphoreGive(xHt7017Mux_Semphr); 
//  return xReturn;
//}
//



int ht7017_WriteReg(uint8_t reg,uint16_t dat)
{
//  xSemaphoreTake(xHt7017Mux_Semphr,portMAX_DELAY);
  int ret = SUCCESS;
  uint8_t buf[6] = {0};
  uint8_t cs = 0;
//  BaseType_t xReturn = pdPASS;
  
  buf[0] = 0x6a;
  buf[1] = (reg | HT7017_WRITE_BIT);
  buf[2] = (dat >> 8);
  buf[3] = (uint8_t)(dat & 0x00ff);
  
  cs = ht7017_checkCS(buf,4);//����cs
  buf[4] = cs;
  
  HT7017_WRITE(buf,5);//send buf
//  vTaskDelay(10);
  //���ղ��ְ�FreeRTOS��ʹ�ö�ֵ�ź���ʵ��
  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,50);//���ڷ���ֵ�ȴ�50��tick
  //if(xReturn == pdPASS)//�ȵ����ڽ��ܵ�����
  {
    if(data_uart2.rec[0] == HT7017_ACK)//����յ�ACK��־
    {
//      xReturn = pdPASS;
    //DBG("ht write succ\r\n");
      ret = SUCCESS;

    }
    else if(data_uart2.rec[0] ==HT7017_NAK)//�յ�����NACK
    {
//      xReturn = pdFAIL;
    //DBG("ht write err noack\r\n");
      ret = ERROR;

    }
    
    data_uart2.rec_cnt = 0;
    memset(data_uart2.rec,0,sizeof(data_uart2.rec));
  }
/*
  else
  {
    xReturn = pdFAIL;
  }*/

//  xSemaphoreGive(xHt7017Mux_Semphr); 
  return ret;
}

//int ht7017_ReadReg(uint8_t reg,uint32_t *dat)
//{
//  xSemaphoreTake(xHt7017Mux_Semphr,portMAX_DELAY);
//  
//  uint8_t buf[6] = {0};
//  uint32_t tmp = 0;
//  BaseType_t xReturn = pdPASS;
//  uint8_t cs = 0;
//  
//  buf[0] = 0x6a;
//  buf[1] = (reg & HT7017_READ_BIT);
//  HT7017_WRITE(buf,2);//�����﷢��
//  vTaskDelay(30);
//  //���ղ��ְ�FreeRTOS��ʹ�ö�ֵ�ź���ʵ��
//  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,100);//���ڷ���ֵ�ȴ�50��tick
//  //if(xReturn == pdPASS)         //������յ�����
//  {
//    if(uart1Rec.recNum == 4)
//    {
//      for(int i=0;i<4;i++)
//      {
//        buf[i+2] = uart1Rec.rec[i];
//      }
//      cs = ht7017_checkCS(buf,5);//��������һλУ���
//      if(cs == buf[5])//У��ֵ��ȷ
//      {
//        tmp = buf[2];
//        tmp = (tmp<<8) | buf[3];
//        tmp = (tmp<<8) | buf[4];
//        *dat = tmp;
//        xReturn = pdPASS;
//        //DBG("ht read reg %x succ val %d val %#X \r\n",reg,tmp,tmp);
//        //DBG("OK\r\n");
//      }
//      
//    }
//    else                //������յ����ݸ�������ȷ
//    {
//      xReturn = pdFAIL;
//      //DBG("ht err %d num \r\n",uart1Rec.recNum);
//      DBG("ENUM %d\r\n",uart1Rec.recNum);
//    }
//    uart1Rec.recNum = 0;
//    memset(uart1Rec.rec,0,sizeof(uart1Rec.rec));
//  }
//  /*
//  else
//  {
//    xReturn = pdFAIL; 
//    DBG("tim\r\n");
//
//  }*/
//
//  //�������ж�У��ֵ
//  xSemaphoreGive(xHt7017Mux_Semphr); 
//  return xReturn;
//}


int ht7017_ReadReg(uint8_t reg,uint32_t *dat)
{
//  xSemaphoreTake(xHt7017Mux_Semphr,portMAX_DELAY);
  int ret = SUCCESS;
  uint8_t buf[6] = {0};
  uint32_t tmp = 0;
//  BaseType_t xReturn = pdPASS;
  uint8_t cs = 0;
  
  buf[0] = 0x6a;
  buf[1] = (reg & HT7017_READ_BIT);
  HT7017_WRITE(buf,2);//�����﷢��
//  vTaskDelay(30);
  //���ղ��ְ�FreeRTOS��ʹ�ö�ֵ�ź���ʵ��
  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,100);//���ڷ���ֵ�ȴ�50��tick
  //if(xReturn == pdPASS)         //������յ�����
  {
    if(data_uart2.rec_cnt == 4)
    {
      for(int i=0;i<4;i++)
      {
        buf[i+2] = data_uart2.rec[i];
      }
      cs = ht7017_checkCS(buf,5);//��������һλУ���
      if(cs == buf[5])//У��ֵ��ȷ
      {
        tmp = buf[2];
        tmp = (tmp<<8) | buf[3];
        tmp = (tmp<<8) | buf[4];
        *dat = tmp;
        ret = SUCCESS;
//        xReturn = pdPASS;
        //DBG("ht read reg %x succ val %d val %#X \r\n",reg,tmp,tmp);
        //DBG("OK\r\n");
      }
      
    }
    else                //������յ����ݸ�������ȷ
    {
      ret = ERROR;
//      xReturn = pdFAIL;
      //DBG("ht err %d num \r\n",uart1Rec.recNum);
//      DBG("ENUM %d\r\n",uart1Rec.recNum);
    }
    data_uart2.rec_cnt = 0;
    memset(data_uart2.rec,0,sizeof(data_uart2.rec));
  }
  /*
  else
  {
    xReturn = pdFAIL; 
    DBG("tim\r\n");

  }*/

  //�������ж�У��ֵ
//  xSemaphoreGive(xHt7017Mux_Semphr); 
  return ret;
}


//void ht7017_init(void)
//{
//  uint32_t tmp = 0;
//  
//  xHt7017Rec_Semphr = xSemaphoreCreateBinary();//������ֵ�ź���
//  uart1Rec.semphr = xHt7017Rec_Semphr;
//  xHt7017Mux_Semphr = xSemaphoreCreateMutex();
//  
//  Usart1_Init(4800);//4800 8 1 ż
//  ht7017_ReadReg(0x1b,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_ReadReg(0x1c,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_ReadReg(0x02,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_ReadReg(0x30,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_ReadReg(0x00,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_ReadReg(0x30,&tmp);
//  for(int i=0;i<0xffff;i++);
//  ht7017_WriteReg(0x30,tmp);
//  for(int i=0;i<0xffff;i++);
//}
//

void ht7017_init(void)
{
  uint32_t tmp = 0;
  
  
  ht7017_ReadReg(0x1b,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_ReadReg(0x1c,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_ReadReg(0x02,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_ReadReg(0x30,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_ReadReg(0x00,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_ReadReg(0x30,&tmp);
  for(int i=0;i<0xffff;i++);
  ht7017_WriteReg(0x30,tmp);
  for(int i=0;i<0xffff;i++);
}




//void ht7017_init(void)
//{
//  uint32_t tmp = 0;
//  
//  
//  ht7017_ReadReg(0x1b,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_ReadReg(0x1c,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_ReadReg(0x02,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_ReadReg(0x30,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_ReadReg(0x00,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_ReadReg(0x30,&tmp);
//  for(int i=0;i<0xfff;i++);
//  ht7017_WriteReg(0x30,tmp);
//  for(int i=0;i<0xfff;i++);
//}