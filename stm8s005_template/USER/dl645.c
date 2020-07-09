#include "dl645.h"
//645 ��Լ�ȴ����ֽڣ��ٴ����ֽ�
#include "device_info.h"
#include "uart2.h"
#include <stdio.h>
#include "bluetooth.h"
#include "hall.h"

DL645_Typedef DL645_Data;//645��ʽ������

int DL645_ChangeID = 0;
int DL645_TxFlag = 0;//645��Ҫ�����־

int DL645_Process(DL645_Typedef *data);

uint8_t Check_CS(uint8_t *rec,uint16_t len)
{
  uint16_t result = 0;
  while(len--)
  {
    result += *rec++;
  }
  return (uint8_t)result;
}
int DL645_Protocol(uint8_t *rec,uint16_t len)
{
  uint8_t cs = 0;
  uint8_t i = 0;
  uint8_t FE_Flag = 0;
  uint8_t tem = 0;
  
  int ret = 0;
  for(i=0;i<4;i++)
  {
     if(rec[i] == 0xFE)
     {
       FE_Flag++;
     }
  }
  //����645 ֡ͷ��֡βҪ�󣬿�ʼ�ж�У���
  if(rec[FE_Flag+0] == 0x68 && rec[FE_Flag+7] == 0x68 && rec[len-1] == 0x16 )
  {
    cs = rec[len-2];
    tem = Check_CS(&rec[FE_Flag],len-2-FE_Flag);
    if(cs == tem)//����У���
    {
      for(i=0;i<6;i++)
      {
        DL645_Data.A[i] = rec[FE_Flag+1+i];//ȡ��ַ
      }
      DL645_Data.CMD.All = rec[FE_Flag+8];
      DL645_Data.Len = rec[FE_Flag+9];
      for(i=0;i<DL645_Data.Len;i++)
      {
        DL645_Data.buf[i] = (uint8_t)(rec[FE_Flag+10+i] -0x33);
      }
      ret = DL645_Process(&DL645_Data);
    }
  }
  return ret;
}

void DL645_Send(DL645_Typedef *data)
{
  uint8_t i = 0;
  uint8_t cnt = 0;

  uint8_t tembuf[50];
  uint16_t cs = 0;

  for(i=0;i<4;i++)
  {
    tembuf[cnt++] = 0xfe;       //ǰ���ַ�
  }
  tembuf[cnt++] = 0x68;
  for(i=0;i<6;i++)
  {
    tembuf[cnt++] = dev_info.dev_id[i];
  }
  tembuf[cnt++] = 0x68;
  tembuf[cnt++] = data->CMD.All;
  tembuf[cnt++] = data->Len;
  for(i=0;i<data->Len;i++)
  {
    tembuf[cnt++] = data->buf[i] + 0x33;
  }
  for(i=4;i<cnt;i++)
  {
    cs += tembuf[i];
  }
  tembuf[cnt++] = (uint8_t) cs;
  tembuf[cnt++] = 0x16;
  Uart2_SendStrLen(tembuf,cnt);
}
void DL645_WriteAddr(DL645_Typedef *data)
{
  uint8_t i;
  if(data->A[0] == 0xAA &&
     data->A[1] == 0xAA &&
     data->A[1] == 0xAA &&
     data->A[3] == 0xAA &&
     data->A[4] == 0xAA &&
     data->A[5] == 0xAA )
  {
    for(i=0;i<6;i++)
    {
      dev_info.dev_id[i] = data->buf[i];
    }
    SAVE_DEVICE_INFO();
    DL645_ChangeID = 1;
    
    data->Len = 0;
    data->CMD.All = 0x95;
    DL645_Send(data);
    //BlueTooth_SetName(1);//����������ַ  ���ڴ��ڽ��ձ�־λ��û�����������ʱ���ܸ�
  }
}

void DL645_ReadAddr(DL645_Typedef *data)
{
  uint8_t i = 0;
  if(data->A[0] == 0xAA &&
     data->A[1] == 0xAA &&
     data->A[1] == 0xAA &&
     data->A[3] == 0xAA &&
     data->A[4] == 0xAA &&
     data->A[5] == 0xAA )
  {
    for(i=0;i<6;i++)
    {
       data->buf[i] = dev_info.dev_id[i];
    }
    data->CMD.All = 0x93;
    data->Len = 0x06;
    DL645_Send(data);
  }
}
//����-1 �ǵ�ַ��ƥ�䣬����0��ַƥ��
int DL645_Check_Addr(DL645_Typedef *data)
{
  uint8_t i;
  for(i=0;i<6;i++)
  {
    if(data->A[i] != dev_info.dev_id[i])
    {
      return -1;
    }
  }
  return 0;
}

int DL645_Device_Control(DL645_Typedef *data)
{
  int ret = 0;
  if(DL645_Check_Addr(data) == -1)//��ַ��ƥ����ִ�ж���
  {
    return -1;
  }
  
  if(data->Len < 16)    //���ݳ���С��16��
  {
    return -1;
  }
  //data buf
 if(data->buf[8] == 0x1a ) //��բ
 {
 //  printf("open");
   DL645_TxFlag = 1;//��Ҫ���ͷֺ�բ״̬
   ret = global_open;
   data->CMD.All = 0x9c;
   data->Len = 0x00;
   DL645_Send(data);
  
 }
 else if(data->buf[8] == 0x1c)//ֱ�Ӻ�բ
 {
//   printf("close");
   DL645_TxFlag = 1;//��Ҫ���ͷֺ�բ״̬
   ret = global_close;
   data->CMD.All = 0x9c;
   data->Len = 0x00;
   DL645_Send(data);
 }
 return ret;
}

void DL645_ReadData(DL645_Typedef *data)
{
  uint8_t DI[4];
  uint8_t i = 0;
  uint8_t tem = 0;
  
  if(DL645_Check_Addr(data) < 0)
  {
    return;
  }
  for(i=0;i<4;i++)
  {
    DI[i] = data->buf[i];
  }
  
  if(DI[0]==0x3D&&DI[1]==0x38&&DI[2]==0x33&&DI[3]==0x37)
  {
    if(OPEN_STATE==0 && CLOSE_STATE==1)//��բ״̬
    {
      tem &= ~(0x01 <<5);
    }
    else if(OPEN_STATE==1 && CLOSE_STATE==0)//��բ״̬
    {
      tem |= (0x01 <<5);
    }
    else                                    //����״̬
    {
      
    }
    
    data->CMD.All = 0x91;
    data->Len = 0x05;
    data->buf[4] = tem;
    DL645_Send(data);
  }
}
int DL645_Process(DL645_Typedef *data)
{
  int ret = 0;
  switch(data->CMD.Detail.Command)
  {
  case ReadData:DL645_ReadData(data);
    break;
  case ReadDataLast:
    break;
  case ReadAddr:DL645_ReadAddr(data);
    break;
  case WriteData:
    break;
  case WriteAddr:DL645_WriteAddr(data);
    break;
  case DeviceControl:ret=DL645_Device_Control(data);
    break;
  default:
    break;
  }
  return ret;
}
