#ifndef _DL645_H
#define _DL645_H

#include "stm8s.h"

typedef enum
{
  TimeSet=0x08,//
  ReadData=0x11,//
  ReadDataLast=0x12,//
  ReadAddr=0x13,//
  WriteData=0x14,//
  WriteAddr=0x15,//
  ChangeBaud=0x17,//
  ChangePassWord=0x18,//
  DeviceWaring=0x19,//
  DeviceWaringResponse=0x1A,//
  EventClear=0x1B,//
  DeviceControl=0x1C,//
  WriteTM=0x1f,
}_App_DL645_Control_Code;//

typedef struct //
{
  unsigned char
      Command:5,			//控制命令
      FollowUpData:1,			//有无后续帧0:No 1:Yes
      ResponseMode:1,			//0:OK  1:Error
      TransmissionDirection:1;		//0:CMD 1:Response	
}_TransmissionControl;		
typedef union
{
  _TransmissionControl Detail;
  unsigned char All;
}Cmd_UnionTypedef;

typedef struct
{
  uint8_t A[6];
  Cmd_UnionTypedef CMD;
  uint8_t Len;
  uint8_t buf[50];
}DL645_Typedef;

enum//全局分合闸状态
{
  global_default = 0,//默认
  global_close = 1,
  global_open = 2,
};
extern int DL645_TxFlag;
extern int DL645_ChangeID;
extern DL645_Typedef DL645_Data;//645格式的数据

int DL645_Protocol(uint8_t *rec,uint16_t len);
void DL645_Send(DL645_Typedef *data);

#endif
