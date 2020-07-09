#include "bluetooth.h"
#include "device_info.h"
#include "uart2.h"
#include <stdio.h>
#include <string.h>
#include "tim4.h"
#include "led.h"

void BlueTooth_EnterAT(void);
void BlueTooth_Test(void);//测试模式

//与单片机相关
#define BLE_REC         data_uart2.rec          //蓝牙接收缓冲区
#define BLE_REC_CNT     data_uart2.rec_cnt      //蓝牙接收到字符串个数
#define BLE_REC_FLAG    data_uart2.flag         //蓝牙接收到数据标志
#define BLE_SEND        Uart2_SendStr


//各个蓝牙厂家的不同命令
#if 0
//富芮坤蓝牙
#define BLE_RESET_CMD           "AT+RESET\r\n"   
#define BLE_GET_NAME            "AT+NAME\r\n"
#define BLE_ENTER_AT            "AT+DT=0\r\n"
#define BLE_EXIT_AT             "AT+DT=1\r\n"
#define BLE_TEST_CMD            "AT\r\n"
#define BLE_INIT()              Uart2_InitSetPar(115200,UART2_WORDLENGTH_8D,UART2_PARITY_NO)              //蓝牙初始化波特率
#define BLE_TEST()              BlueTooth_EnterAT()
#endif
#if 1
//小博通蓝牙
#define BLE_RESET_CMD           "AT+REBOOT\r\n"  
#define BLE_GET_NAME            "AT+NAME\r\n"
#define BLE_ENTER_AT            "AT+QDMO\r\n"//进入AT 
#define BLE_EXIT_AT             "AT+DMO=0\r\n"//退出AT 进入透传
#define BLE_TEST_CMD            "AT+TEST\r\n"
#define BLE_CMP_NAME            "\r\n+NAME="

#define BLE_INIT()               Uart2_InitSetPar(9600,UART2_WORDLENGTH_9D,UART2_PARITY_EVEN)              //蓝牙初始化波特率
#define BLE_TEST()              BlueTooth_Test()//测试是否进入AT模式
#endif

//瑞昱蓝牙


//富芮坤蓝牙
uint8_t BlueCnt = 0;//
//命令执行成功返回 0,失败返回负数
//需要开启定时器中断和全局中断，因为delay函数是使用定时器中断进行判断
int BlueTooth_SendCmd(uint8_t *cmd)
{
  int ret;//返回值
  int delay_cnt = 0;
  
  BLE_SEND(cmd);  //发送命令
  while(1)
  {
    if(data_uart2.flag == 1)//收到蓝牙命令字符串
    {
      if(strstr((char*)&data_uart2.rec,"OK") != NULL)//找到"OK字符串"
      {
        ret = 0;                                
      }
      else
      {
        ret = -1;
      }
      BlueCnt = data_uart2.rec_cnt;
      data_uart2.flag = 0;//清除串口接收标志
      data_uart2.rec_cnt = 0;//清除串口接收个数
      break;
    }
    Delay_MS(1);
    delay_cnt++;
    
    if(delay_cnt >1000)
    {
      ret = -2;
      break;
    }
  }

  return ret;
}

void BlueTooth_Init(void)
{
  char name[40];
  int namelen = 0;
//  char tembuf[30];
  BLE_INIT();//蓝牙初始化
  
//  BlueTooth_SendCmd(BLE_ENTER_AT);//进入AT模式命令
  BLE_TEST();           //上电进入AT模式
  BlueTooth_SendCmd(BLE_GET_NAME);//发送查询名称命令
  
  namelen = sprintf(name,"WS%02x%02x%02x%02x%02x%02x",dev_info.dev_id[0],dev_info.dev_id[1],\
                    dev_info.dev_id[2],dev_info.dev_id[3],dev_info.dev_id[4],dev_info.dev_id[5]);//蓝牙需要的名称
  
  if(strncmp((char*)data_uart2.rec,BLE_CMP_NAME,(sizeof(BLE_CMP_NAME)-1)) == 0)//如果回应的是名字
  {
    if(strncmp((char*)&data_uart2.rec[sizeof(BLE_CMP_NAME)-1],name,namelen) == 0)//如果蓝牙名称正确，不进行处理
    {
      
    }
    else        //蓝牙名称不正确，设置新的名字
    {
      BlueTooth_SetName(0);    
    }
  }
  else          //回应不是蓝牙名称，出现错误，进行错误判断
  {
    
  }
  
  BlueTooth_SendCmd(BLE_EXIT_AT);  //退出AT模式 
}

//进入AT模式
void BlueTooth_EnterAT(void)
{
  int tem = 0;
  do
  {
    tem = BlueTooth_SendCmd(BLE_ENTER_AT);//进入AT模式
    
    if(tem >= 0)
      break;
    
    LED_REVERSE();
  }while(1);
  LED_BRIGHT();
}

void BlueTooth_Test(void)//测试模式
{
  int tem = 0;
  do
  {
    tem = BlueTooth_SendCmd(BLE_TEST_CMD);//发送测试命令
    
    if(tem >= 0)
      break;
    
    LED_REVERSE();
  }while(1);
  LED_BRIGHT();
}

//mode 0 :不需要重新进入AT，和退出AT
//mode 1: 需要重新进入AT 和退出
void BlueTooth_SetName(uint8_t mode)
{
  char name[40];
  int retry = 3;
  
  sprintf(name,"AT+NAME=WS%02x%02x%02x%02x%02x%02x\r\n",dev_info.dev_id[5],dev_info.dev_id[4],\
                    dev_info.dev_id[3],dev_info.dev_id[2],dev_info.dev_id[1],dev_info.dev_id[0]);//蓝牙需要的名称
  if(mode == 1)
  {
    BlueTooth_EnterAT();     //进入AT模式
  }

  while(retry --)
  {
    if(BlueTooth_SendCmd((uint8_t*)name) == 0)//设置名字
    {
      break;
    }
  }
  BlueTooth_SendCmd(BLE_RESET_CMD);//复位
  //Delay_MS(200);//延时200ms
  BLE_TEST();//测试有没有重新进入AT模式
  if(mode == 1)
  {
    BlueTooth_SendCmd(BLE_EXIT_AT);  //退出AT模式
  }


}