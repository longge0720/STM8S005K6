#include "bluetooth.h"
#include "device_info.h"
#include "uart2.h"
#include <stdio.h>
#include <string.h>
#include "tim4.h"
#include "led.h"

void BlueTooth_EnterAT(void);
void BlueTooth_Test(void);//����ģʽ

//�뵥Ƭ�����
#define BLE_REC         data_uart2.rec          //�������ջ�����
#define BLE_REC_CNT     data_uart2.rec_cnt      //�������յ��ַ�������
#define BLE_REC_FLAG    data_uart2.flag         //�������յ����ݱ�־
#define BLE_SEND        Uart2_SendStr


//�����������ҵĲ�ͬ����
#if 0
//����������
#define BLE_RESET_CMD           "AT+RESET\r\n"   
#define BLE_GET_NAME            "AT+NAME\r\n"
#define BLE_ENTER_AT            "AT+DT=0\r\n"
#define BLE_EXIT_AT             "AT+DT=1\r\n"
#define BLE_TEST_CMD            "AT\r\n"
#define BLE_INIT()              Uart2_InitSetPar(115200,UART2_WORDLENGTH_8D,UART2_PARITY_NO)              //������ʼ��������
#define BLE_TEST()              BlueTooth_EnterAT()
#endif
#if 1
//С��ͨ����
#define BLE_RESET_CMD           "AT+REBOOT\r\n"  
#define BLE_GET_NAME            "AT+NAME\r\n"
#define BLE_ENTER_AT            "AT+QDMO\r\n"//����AT 
#define BLE_EXIT_AT             "AT+DMO=0\r\n"//�˳�AT ����͸��
#define BLE_TEST_CMD            "AT+TEST\r\n"
#define BLE_CMP_NAME            "\r\n+NAME="

#define BLE_INIT()               Uart2_InitSetPar(9600,UART2_WORDLENGTH_9D,UART2_PARITY_EVEN)              //������ʼ��������
#define BLE_TEST()              BlueTooth_Test()//�����Ƿ����ATģʽ
#endif

//��������


//����������
uint8_t BlueCnt = 0;//
//����ִ�гɹ����� 0,ʧ�ܷ��ظ���
//��Ҫ������ʱ���жϺ�ȫ���жϣ���Ϊdelay������ʹ�ö�ʱ���жϽ����ж�
int BlueTooth_SendCmd(uint8_t *cmd)
{
  int ret;//����ֵ
  int delay_cnt = 0;
  
  BLE_SEND(cmd);  //��������
  while(1)
  {
    if(data_uart2.flag == 1)//�յ����������ַ���
    {
      if(strstr((char*)&data_uart2.rec,"OK") != NULL)//�ҵ�"OK�ַ���"
      {
        ret = 0;                                
      }
      else
      {
        ret = -1;
      }
      BlueCnt = data_uart2.rec_cnt;
      data_uart2.flag = 0;//������ڽ��ձ�־
      data_uart2.rec_cnt = 0;//������ڽ��ո���
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
  BLE_INIT();//������ʼ��
  
//  BlueTooth_SendCmd(BLE_ENTER_AT);//����ATģʽ����
  BLE_TEST();           //�ϵ����ATģʽ
  BlueTooth_SendCmd(BLE_GET_NAME);//���Ͳ�ѯ��������
  
  namelen = sprintf(name,"WS%02x%02x%02x%02x%02x%02x",dev_info.dev_id[0],dev_info.dev_id[1],\
                    dev_info.dev_id[2],dev_info.dev_id[3],dev_info.dev_id[4],dev_info.dev_id[5]);//������Ҫ������
  
  if(strncmp((char*)data_uart2.rec,BLE_CMP_NAME,(sizeof(BLE_CMP_NAME)-1)) == 0)//�����Ӧ��������
  {
    if(strncmp((char*)&data_uart2.rec[sizeof(BLE_CMP_NAME)-1],name,namelen) == 0)//�������������ȷ�������д���
    {
      
    }
    else        //�������Ʋ���ȷ�������µ�����
    {
      BlueTooth_SetName(0);    
    }
  }
  else          //��Ӧ�����������ƣ����ִ��󣬽��д����ж�
  {
    
  }
  
  BlueTooth_SendCmd(BLE_EXIT_AT);  //�˳�ATģʽ 
}

//����ATģʽ
void BlueTooth_EnterAT(void)
{
  int tem = 0;
  do
  {
    tem = BlueTooth_SendCmd(BLE_ENTER_AT);//����ATģʽ
    
    if(tem >= 0)
      break;
    
    LED_REVERSE();
  }while(1);
  LED_BRIGHT();
}

void BlueTooth_Test(void)//����ģʽ
{
  int tem = 0;
  do
  {
    tem = BlueTooth_SendCmd(BLE_TEST_CMD);//���Ͳ�������
    
    if(tem >= 0)
      break;
    
    LED_REVERSE();
  }while(1);
  LED_BRIGHT();
}

//mode 0 :����Ҫ���½���AT�����˳�AT
//mode 1: ��Ҫ���½���AT ���˳�
void BlueTooth_SetName(uint8_t mode)
{
  char name[40];
  int retry = 3;
  
  sprintf(name,"AT+NAME=WS%02x%02x%02x%02x%02x%02x\r\n",dev_info.dev_id[5],dev_info.dev_id[4],\
                    dev_info.dev_id[3],dev_info.dev_id[2],dev_info.dev_id[1],dev_info.dev_id[0]);//������Ҫ������
  if(mode == 1)
  {
    BlueTooth_EnterAT();     //����ATģʽ
  }

  while(retry --)
  {
    if(BlueTooth_SendCmd((uint8_t*)name) == 0)//��������
    {
      break;
    }
  }
  BlueTooth_SendCmd(BLE_RESET_CMD);//��λ
  //Delay_MS(200);//��ʱ200ms
  BLE_TEST();//������û�����½���ATģʽ
  if(mode == 1)
  {
    BlueTooth_SendCmd(BLE_EXIT_AT);  //�˳�ATģʽ
  }


}