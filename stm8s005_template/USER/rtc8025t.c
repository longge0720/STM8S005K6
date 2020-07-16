#include "rtc8025t.h"

#define I2CTIMES	10
/*"*********************************************************
时钟芯片寄存器定义
*********************************************************"*/
#define R8025DEV    0X64        /*"8025t device address 01100100"*/

#define R8025SECA   0x00        /*"8025t SECOND register address"*/

#define R8025TCONA  0x0d        /*"8025t externsion register address"*/
#define R8025TCON1  0x00        /*"externsion register set "*/
                                /*"TEST   WADA  USEL   TE  FSEL1  FSEL0  TSEL1 TSEL0"*/
#define R8025TCON2  0x00        /*"flag register set "*/
                                /*"  o      o    UF    TF    AF     o     VLF  VDET"*/
#define R8025TCON3  0xe0        /*"control register set "*/
                                /*"CSEL1  CSEL0  UIE   TIE  AIE     o      o   RESET"*/
                                /*" CSEL1  CSEL0  补偿间隔  1 1 30秒     1 0  10秒     0 1  2秒（默认）   0 0 0.5秒"*/
                                /*" UIE  更新中断使能位"*/

//const uint8_t  contorl[3] = {R8025TCON1, R8025TCON2, R8025TCON3};
//cRtc8025t rtc;


//------------------------------------------------------------------------------
// Num---要写入数据的个数
// Buf---写入数据的起始地址
// Addr ---RTC起始寄存器地址
// 返回 TRUE   1-----成功
//      FALSE  0-----失败
uint16_t i2c_write_RTC(uint8_t Addr,uint8_t Num,uint8_t *Buf)
{
	uint8_t i,Flag;
	Flag = True;
	i2c_start_set();
	for(i=0;i<I2CTIMES;i++)
	{
    
		i2c_Start();
		delay_us(20);
		i2c_SendByte(0x64);//write a byte :slave device
		if(i2c_WaitAck()!=False)
			continue;
		i2c_SendByte(Addr);// write a byte :address
		if(i2c_WaitAck()!=True)
			break;
    		
	}
	
	if(i >= (I2CTIMES-1))
	{
		Flag = False;
	}
	
	for(i = 0;i<Num;i++)
	{
		i2c_SendByte(Buf[i]);
		if(i2c_WaitAck()!=False)
		{
			Flag = False;
		}
			
	}
		
	i2c_Stop();
	i2c_stop_set();	
	return Flag;
}

// Num---要读取数据的个数
// Buf---读取数据的起始地址
// Addr ---RTC起始寄存器地址
void i2c_read_RTC(uint8_t Addr,uint8_t Num,uint8_t *Buf)
{
	uint8_t i;
	//uint8_t IntStatus;
	
	//IntStatus = api_splx(0);
	i2c_start_set();
	for(i=0;i<I2CTIMES;i++)
	{
		i2c_Start();
		i2c_SendByte(0x64);
		
		if(i2c_WaitAck()!=False)
			continue;
		i2c_SendByte(Addr);
		if(i2c_WaitAck()!=False)
			continue;
		i2c_Start();
		i2c_SendByte(0x65);
		if(i2c_WaitAck()!=True)
			break;
		}	
 	for(i=0;i<Num;i++)
 	{
		Buf[i] = i2c_ReadByte();
		
		if(i<(Num-1))
		{
			i2c_Ack();
		}
	}
	i2c_NAck();
	i2c_Stop();
	i2c_stop_set();
}
//初始化秒脉冲输出方式
//Type -- 0			1HZ脉冲输出
//Type -- 不为零	禁止秒脉冲输出
void api_InitOutClockPuls(uint8_t Type)
{
	uint8_t temp;
	uint8_t Status;
	i2c_read_RTC(0x0f,1,&temp);
	if(temp == 0xff)
	{
		temp = 0x60;
	}
	if( Type == 0 )		//打开秒脉冲输出
	{
		temp |= 0x20;
	}
	else				//关闭秒脉冲输出
	{
		temp &= ~(0x20);
	}
	Status = i2c_write_RTC(0x0f,1,&temp);
    Status=Status;      //ccdebug
}


/*"宏定义时间格式数据存储顺序，根据应用层需求的顺序调整输出时间格式顺序"*/
#define SEC   5
#define MIN   4
#define HOUR  3
#define DAY   2
#define MON   1
#define YEAR  0
#define WEEK  6
/*"*****************************************************************************"*/
/*"  Function:       EXRTC_Read"*/
/*"  Description:    读外置时钟芯片时钟"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output          t:时钟接收数据缓存指针  存放顺序低字节到高字节为年 月 日 周次 时 分 秒"*/
/*"  Return:         TRUE      读时钟成功       FALSE    读时钟失败"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
uint8_t  EXRTC_ReadTime(uint8_t  *t)
{
    //uint8_t  uni = 0;
    uint8_t  result;
    uint8_t  DateWeekTime[7];

    i2c_read_RTC(R8025SECA, sizeof(DateWeekTime), DateWeekTime);
    result = i2c_WaitAck();
    if( result == True )
    {
        *(t + YEAR) = DateWeekTime[6];
        *(t + MON) = DateWeekTime[5] & 0x1f;
        *(t + DAY) = DateWeekTime[4] & 0x3f;
        *(t + HOUR) = DateWeekTime[2] & 0x3f;
        *(t + MIN) = DateWeekTime[1] & 0x7f;
        *(t + SEC) = DateWeekTime[0] & 0x7f;
        *(t + WEEK) = WeekChange(DateWeekTime[3], 1);
    }
    else
    {
    }

    return (result);
}

/*"*****************************************************************************"*/
/*"  Function:       EXRTC_Write"*/
/*"  Description:    写外置时钟芯片时钟"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output          t:时钟写数据缓存指针  存放顺序低字节到高字节为年 月 日 周次 时 分 秒"*/
/*"  Return:         TRUE      写时钟成功       FALSE    写时钟失败"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
//uint8_t  EXRTC_WriteTime(uint8_t   *t)
//{
//    uint8_t  result;
//    uint8_t  weekTmp;
//    uint8_t  DateWeekTime[7];
//
//    weekTmp = LIB_CalWeekBCD((ST_6TIME_FORM *)t);
//    DateWeekTime[6] = *(t + YEAR);
//    DateWeekTime[5] = *(t + MON);
//    DateWeekTime[4] = *(t + DAY);
//    DateWeekTime[2] = *(t + HOUR);
//    DateWeekTime[1] = *(t + MIN);
//    DateWeekTime[0] = *(t + SEC);
//    DateWeekTime[3] = WeekChange(weekTmp, 0);
//    i2c_write_RTC(R8025SECA, sizeof(DateWeekTime),DateWeekTime);
//    result = i2c_WaitAck();
//    return (result);
//}




void close_exrtc_sec(void)
{
	uint8_t  count,result;
	uint8_t  ctrl_C0[3]={0x00,0x00,0xc0};	
	
	i2c_read_RTC(R8025TCONA + 1, sizeof(count),(uint8_t  *)&count); /*"判断芯片失电标志"*/
    result = i2c_WaitAck();
    if(True == result)
    {      
        count &= 0x02;
        if(count)
        {
            //mRtcErr.bit.b0 = 1;todo
        }
        else
        {
        }
		
       i2c_write_RTC(R8025TCONA, sizeof(ctrl_C0),ctrl_C0);
    }
}

/*"*****************************************************************************"*/
/*"  Function:       WeekChange"*/
/*"  Description:    周次格式转换函数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Week 输入周次"*/
/*"                  kind 转换方向  0 softdate --> rtcdata   1 rtcdata  --> softdata"*/
/*"  Output          "*/
/*"  Return:         返回转换后周次格式"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
uint8_t WeekChange(uint8_t  Week,uint8_t  kind)
{
    uint8_t  ret_week = 0;
    uint8_t  uni = 0;
    if(kind == 0) /*"write in to 8025T"*/
    {
        ret_week = 1;
        ret_week <<= Week;
    }
    else /*"read from 8025T"*/
    {
        for(uni = 0; uni < 7; uni++)
        {
            if((Week & 0x01) == 1)
            {
                ret_week = uni;
                break;
            }
            Week >>= 0x01;
        }
    }

    return (ret_week);
}














