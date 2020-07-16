#ifndef _RTC8025T_H_
#define _RTC8025T_H_
#include "i2c.h"


enum eStatus
{
  False,
  True
};


    uint16_t i2c_write_RTC(uint8_t Addr,uint8_t Num,uint8_t *Buf);
    void i2c_read_RTC(uint8_t Addr,uint8_t Num,uint8_t *Buf);
    uint8_t  WeekChange(uint8_t  Week,uint8_t  kind);

    void api_InitOutClockPuls(uint8_t Type);
    uint8_t  EXRTC_ReadTime(uint8_t  *t);
    uint8_t  EXRTC_WriteTime(uint8_t   *t);
    void close_exrtc_sec(void);








#endif //(#ifndef _RTC8025T_H_)