#include "ht7017.h"
#include <string.h>
#include <stdio.h>
#include "uart2.h"
//系统冷复位需要20ms
//系统热复位需要2ms
//HT7017读写标志位
#define HT7017_READ_BIT         (~(0x01 << 7))
#define HT7017_WRITE_BIT        (0x01 << 7)
#define HT7017_ACK              0x54
#define HT7017_NAK              0x63


//extern UartRec_Typedef uart2Rec;
//extern UartData_Typedef data_uart2;

//SemaphoreHandle_t xHt7017Rec_Semphr = NULL;
//SemaphoreHandle_t xHt7017Mux_Semphr = NULL;//互斥信号量




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
//  cs = ht7017_checkCS(buf,4);//计算cs
//  buf[4] = cs;
//  
//  HT7017_WRITE(buf,5);//send buf
//  vTaskDelay(10);
//  //接收部分绑定FreeRTOS，使用二值信号量实现
//  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,50);//串口返回值等待50个tick
//  //if(xReturn == pdPASS)//等到串口接受的数据
//  {
//    if(uart1Rec.rec[0] == HT7017_ACK)//如果收到ACK标志
//    {
//      xReturn = pdPASS;
//    //DBG("ht write succ\r\n");
//
//    }
//    else if(uart1Rec.rec[0] ==HT7017_NAK)//收到的是NACK
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
  
  cs = ht7017_checkCS(buf,4);//计算cs
  buf[4] = cs;
  
  HT7017_WRITE(buf,5);//send buf
//  vTaskDelay(10);
  //接收部分绑定FreeRTOS，使用二值信号量实现
  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,50);//串口返回值等待50个tick
  //if(xReturn == pdPASS)//等到串口接受的数据
  {
    if(data_uart2.rec[0] == HT7017_ACK)//如果收到ACK标志
    {
//      xReturn = pdPASS;
    //DBG("ht write succ\r\n");
      ret = SUCCESS;

    }
    else if(data_uart2.rec[0] ==HT7017_NAK)//收到的是NACK
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
//  HT7017_WRITE(buf,2);//在这里发送
//  vTaskDelay(30);
//  //接收部分绑定FreeRTOS，使用二值信号量实现
//  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,100);//串口返回值等待50个tick
//  //if(xReturn == pdPASS)         //如果接收到数据
//  {
//    if(uart1Rec.recNum == 4)
//    {
//      for(int i=0;i<4;i++)
//      {
//        buf[i+2] = uart1Rec.rec[i];
//      }
//      cs = ht7017_checkCS(buf,5);//计算除最后一位校验和
//      if(cs == buf[5])//校验值正确
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
//    else                //如果接收到数据个数不正确
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
//  //在这里判断校验值
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
  
//  buf[0] = 0x07;
  buf[1] = (reg & HT7017_READ_BIT);
  buf[1] = reg;
  HT7017_WRITE(buf,2);//在这里发送
//  vTaskDelay(30);
  //接收部分绑定FreeRTOS，使用二值信号量实现
  //xReturn = xSemaphoreTake(xHt7017Rec_Semphr,100);//串口返回值等待50个tick
  //if(xReturn == pdPASS)         //如果接收到数据

//    if(data_uart2.rec_cnt == 4)
//    {
//      for(int i=0;i<4;i++)
//      {
//        buf[i+2] = data_uart2.rec[i];
//      }
//      cs = ht7017_checkCS(buf,5);//计算除最后一位校验和
//      if(cs == buf[5])//校验值正确
//      {
//        tmp = buf[2];
//        tmp = (tmp<<8) | buf[3];
//        tmp = (tmp<<8) | buf[4];
//        *dat = tmp;
//        ret = SUCCESS;
////        xReturn = pdPASS;
//        //DBG("ht read reg %x succ val %d val %#X \r\n",reg,tmp,tmp);
//        //DBG("OK\r\n");
//      }
//      
//    }
//    else                //如果接收到数据个数不正确
//    {
//      ret = ERROR;
////      xReturn = pdFAIL;
//      //DBG("ht err %d num \r\n",uart1Rec.recNum);
////      DBG("ENUM %d\r\n",uart1Rec.recNum);
//    }
//    data_uart2.rec_cnt = 0;
//    memset(data_uart2.rec,0,sizeof(data_uart2.rec));

  /*
  else
  {
    xReturn = pdFAIL; 
    DBG("tim\r\n");

  }*/

  //在这里判断校验值
//  xSemaphoreGive(xHt7017Mux_Semphr); 
  return ret;
}


//void ht7017_init(void)
//{
//  uint32_t tmp = 0;
//  
//  xHt7017Rec_Semphr = xSemaphoreCreateBinary();//创建二值信号量
//  uart1Rec.semphr = xHt7017Rec_Semphr;
//  xHt7017Mux_Semphr = xSemaphoreCreateMutex();
//  
//  Usart1_Init(4800);//4800 8 1 偶
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
  uint8_t writeProtect[] = {0x6a,0xb2,0x00,0xbc,0x27};
  HT7017_WRITE(writeProtect,sizeof(writeProtect));
  for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
    HT7017_WRITE(writeProtect,sizeof(writeProtect));
  for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
    HT7017_WRITE(writeProtect,sizeof(writeProtect));
  for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
    HT7017_WRITE(writeProtect,sizeof(writeProtect));
  for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}

  uint8_t enableADC[] = {0x6a,0xC3,0x00,0x07,0xCB};
  HT7017_WRITE(enableADC,sizeof(enableADC));
for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
  HT7017_WRITE(enableADC,sizeof(enableADC));
for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
  HT7017_WRITE(enableADC,sizeof(enableADC));
for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
  HT7017_WRITE(enableADC,sizeof(enableADC));
for(int j=0;j<50;j++){for(int i=0;i<0xffff;i++);}
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