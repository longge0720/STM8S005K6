#ifndef _DEVICE_INFO
#define _DEVICE_INFO

#include "stm8s.h"

//��Ƭ���ڲ�E2PROM ��ʼ��ַ�ͳ���
#define INTERNAL_E2PROM_ADDR_START      0x004000    
#define INTERNAL_E2PROM_ADDR_END        0x00407F 
#define INTERNAL_E2PROM_LEN             128 

//�豸��Ϣ
#define INTERNAL_VERSION  0xAABBCCDD
#define DEVICE_INFO_ADDR                INTERNAL_E2PROM_ADDR_START
typedef struct
{
  uint8_t dev_id[6]; //�豸ID
  uint8_t passwd[3]; //�豸����
  int cnt;  //�ϵ����
  uint32_t version;
}Device_Typedef;
extern Device_Typedef  dev_info;

void E2Init(void);
void E2Read( uint32_t addr,uint8_t *buf,uint16_t len);
void E2Write(uint32_t addr,uint8_t *buf,uint16_t len);
void ReadDeviceInfo(void);

#define SAVE_DEVICE_INFO()        E2Write(DEVICE_INFO_ADDR,(uint8_t *)&dev_info,sizeof(dev_info))
#define READ_DEVICE_INFO()        E2Read(DEVICE_INFO_ADDR,(uint8_t *)&dev_info,sizeof(dev_info))

#endif
