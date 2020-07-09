#include "device_info.h"

Device_Typedef  dev_info;

void E2Init(void)
{
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

void ReadDeviceInfo(void)
{
  E2Init();//��ʼ��E2
  READ_DEVICE_INFO();
  if(dev_info.version != INTERNAL_VERSION)    //E2 û�н��г�ʼ��,��ʼ���г�ʼ��
  {
    dev_info.cnt = 0;
    dev_info.version = INTERNAL_VERSION;
    //���ó�ʼID
    dev_info.dev_id[0] = 0x00;                  //����ͨ���������
    dev_info.dev_id[1] = 0x00;
    dev_info.dev_id[2] = 0x00;
    dev_info.dev_id[3] = 0x00;
    dev_info.dev_id[4] = 0x00;
    dev_info.dev_id[5] = 0x00;
    
    //���ó�ʼ����
    dev_info.passwd[0] = 0x00;
    dev_info.passwd[1] = 0x00;
    dev_info.passwd[2] = 0x00;
    SAVE_DEVICE_INFO();
  }
  else                 //�Ѿ���ʼ���ˣ���д����+1
  {
    dev_info.cnt ++;
    SAVE_DEVICE_INFO();
  }
}
void E2Read(uint32_t addr,uint8_t *buf,uint16_t len)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);//����E2PROM
  while(len-- > 0)
  {
    *buf++ = FLASH_ReadByte(addr++);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void E2Write(uint32_t addr,uint8_t *buf,uint16_t len)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);//����E2PROM
  while(len-- > 0)
  {
    FLASH_ProgramByte(addr++,*buf++);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}
